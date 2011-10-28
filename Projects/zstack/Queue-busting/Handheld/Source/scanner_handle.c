#include <hal_types.h>
#include "hal_led.h"
#include "buffer.h"
#include "common.h"
#include "xdata_handle.h"
#include "scanner_handle.h"
/***
* return 0: error, other: the number of bytes of string
*/
static uint8 getString(ringBuf_t *pBuf, uint8 *output, const uint8 max_length)
{
  uint8 tmp[24],i,count;
  if(output ==0)
    return 0;
  i=0;
  while(bufSize(pBuf) && i < max_length)
  {
     count = bufGet(pBuf,&tmp[i],1);
     if(count ==0 || tmp[i]==0)
          break;
    i++;
  }
  return i;
}
/*
* return 1: true, 0: false
*/
static uint8 IsBasketID(uint8 *input, const uint8 length){
  if(input[0] == BASKET_ID_SIGN && length == BASKET_ID_LEN)
    return 1;
  else
    return 0;
}
/*
* return 1: true, 0: false
*/
static uint8 IsProductID(uint8 *input, const uint8 length){
  if(input[0] != BASKET_ID_SIGN && length <= PROD_ID_MAX_LEN && length >= PROD_ID_MIN_LEN)
    return 1;
  else
    return 0;
}

/*
* return 0: not found, other: index
*/
static Product* FindProductInBasket(uint8 *prodID,const uint8 length,Basket * basket)
{
  Product* pProduct = NULL;
  uint8 i;
  for(i=0;i<basket->len;i++)
    if(IsSameString(prodID,(uint8*)basket->prods_id+(basket->offset)*i,length))
    {
      pProduct->id = (uint8*)basket->prods_id+(basket->offset)*i;
      pProduct->num = pProduct->id[basket->offset-1];
      return pProduct;
    }
  return NULL;
}
//===============
#define BASKET_ID_NULL  "-"
static Basket  CurrentBasket;
static Product *CurrentProduct;
uint8 tmp[24];
static void NewBasket(Basket *a, uint8 * id, const uint8 length)
{
  //Clear Current Basket ID
  CopyString((uint8*)a->basket_id,id,length);
  //Clear len
  a->len =0;
  a->offset = OFFSET;
}
void ScannerHandleInit(void)
{
  NewBasket(&CurrentBasket,BASKET_ID_NULL,5);
  CurrentProduct = NULL;
}

void ScannerHandleInput(ringBuf_t *pBuf)
{
  uint8 count,i;
  while(bufSize(pBuf))
  {
    count=getString(pBuf,tmp,24);
    if(IsBasketID(tmp,count))
    {
      //Basket ID is not null
      if(IsSameString((uint8*)CurrentBasket.basket_id,BASKET_ID_NULL,BASKET_ID_LEN)==0)
      {
        //Store to Flash
        WriteBasket(&CurrentBasket);
        if(IsSameString((uint8*)CurrentBasket.basket_id,tmp,BASKET_ID_LEN))
        { 
          //Close Basket
          NewBasket(&CurrentBasket,BASKET_ID_NULL,1);
          //Signal to User: turn off LED
          HalLedSet( HAL_LED_4, HAL_LED_MODE_OFF );
        }else{
          //New Basket;
          NewBasket(&CurrentBasket,tmp,BASKET_ID_LEN);
          //TODO Signal to User: turn on LED
          HalLedSet( HAL_LED_4, HAL_LED_MODE_ON );
        }
      }
    }
    else if(IsProductID(tmp, count))
    {
      //Basket is opened
      if(IsSameString((uint8*)CurrentBasket.basket_id,BASKET_ID_NULL,BASKET_ID_LEN)==0)
      {
        CurrentProduct=FindProductInBasket(tmp,count,&CurrentBasket);
        if(i!=0)//Exist Product in Current Basket
        {
          CurrentProduct->num++;
        }else//New Product in Current Basket
        {
          CurrentProduct=(Product*)CurrentBasket.prods_id+(CurrentBasket.offset)*(CurrentBasket.len);
          CopyString(CurrentProduct->id,tmp,PRODS_ID_LEN);
          CurrentBasket.len++;
        }
      }else
        ;//Drop
    }else{
      //Drop
    }
  }
}