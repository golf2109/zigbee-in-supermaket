#include <hal_types.h>
#include "hal_led.h"
#include "buffer.h"
#include "common.h"
#include "OSAL_Memory.h"
#include "OSAL.h"
#include "xdata_handle.h"
#include "scanner_handle.h"

/**df*/
#define BASKET_ID_NULL  "--------"
/**Store Basket scanning*/
static Basket  *CurrentBasket;
/**Point to Product scanning*/
static Product *CurrentProduct;
/***
* return 0: error, other: the number of bytes of string
*/
static uint8 getString(ringBuf_t *pBuf, uint8 *output, const uint8 max_length)
{
  uint8 i,count;
  if(output ==0)
    return 0;
  i=0;
  while(bufSize(pBuf) && i < max_length)
  {
     count = bufGet(pBuf,&output[i],1);
     if(count ==0 || output[i]==0x00)
          break;
    i++;
  }
  return i;
}
/*
* return 1: true, 0: false
*/
static uint8 IsBasketID(uint8 *input, const uint8 length){
  if(input[0] == BASKET_ID_FORMAT && length == BASKET_ID_LEN)
    return 1;
  else
    return 0;
}
/*
* return 1: true, 0: false
*/
static uint8 IsProductID(uint8 *input, const uint8 length){
  if(input[0] != BASKET_ID_FORMAT && length == PRODS_ID_LEN)
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
    if(IsSameString(prodID,(uint8*)(basket->prods[i].id),PRODS_ID_LEN))
    {
      pProduct = (Product*)(&basket->prods[i]);
      return pProduct;
    }
  return NULL;
}
/*
* 
*/
static Basket * NewBasket(uint8 * id)
{
  //Clear Current Basket ID
  //CopyString((uint8*)a->id,id,BASKET_ID_LEN);
  Basket * pk =(Basket *)osal_mem_alloc(sizeof(Basket));
  osal_memcpy((uint8*)pk->id,(uint8*)id,BASKET_ID_LEN);
  //Clear len
  pk->len =0;
  return pk;
}
void ScannerHandleInit(void)
{
  CurrentBasket=NULL;
  CurrentProduct = NULL;
}

void ScannerHandleInput(ringBuf_t *pBuf)
{
  uint8 tmp[24];
  uint8 count;
  while(bufSize(pBuf))
  {
    count=getString(pBuf,tmp,24);
    if(IsBasketID(tmp,count))
    {
      //Basket ID is not null
      if(CurrentBasket)
      {
        if(CurrentBasket->len > 0)
        {
          //Store to Flash
          WriteBasket(CurrentBasket);
        }
        if(IsSameString((uint8*)(CurrentBasket->id),tmp,BASKET_ID_LEN))
        { 
          //Close Basket
          osal_mem_free((uint8*)CurrentBasket);
          CurrentBasket=NULL;
          //Signal to User: turn off LED
          HalLedSet( HAL_LED_4, HAL_LED_MODE_OFF );
        }else{
          //New Basket;
          osal_memcpy((uint8*)CurrentBasket->id,tmp,BASKET_ID_LEN);
          // Signal to User: turn on LED
          HalLedSet( HAL_LED_4, HAL_LED_MODE_ON );
        }
      }else//null
      {
        if(FindBasket(tmp)==0)
        {
          //New Basket;
          CurrentBasket= NewBasket(tmp);
          // Signal to User: turn on LED
          HalLedSet( HAL_LED_4, HAL_LED_MODE_ON );
        }else
          HalLedSet( HAL_LED_4,HAL_LED_MODE_BLINK);
      }
    }
    else if(IsProductID(tmp, count) &&CurrentBasket)//Basket is opened
    {
      CurrentProduct=FindProductInBasket(tmp,count,CurrentBasket);
      if(CurrentProduct!=NULL)//Exist Product in Current Basket
      {
        CurrentProduct->num++;
      }else//New Product in Current Basket
      {
        CurrentProduct=(Product*)(&CurrentBasket->prods[CurrentBasket->len]);
        CopyString(CurrentProduct->id,tmp,PRODS_ID_LEN);
        CurrentProduct->num=1;
        CurrentBasket->len++;
      }
      HalLedSet( HAL_LED_2,HAL_LED_MODE_BLINK);

    }else if(IsSameString("@FlashReset",tmp,11))//Reset Flash
    {
      FlashReset();
    }else if(IsSameString("@FlashRead",tmp,10)){
      ReadBasket("#8864352");//Drope
    }
  }
}