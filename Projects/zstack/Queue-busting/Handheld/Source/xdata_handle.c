#include <hal_types.h>
#include "Serialize.h"
#include "dataflash.h"
#include "OSAL_Memory.h"
#include "OSAL.h"
#include "common.h"
#include "xdata_handle.h"
/*******Flash Layout*************
- Quantiy Of Basket         1B
- Basket Length in Bytes    2B : N + 1B
- Bakset 1                  
 + EmptyFlag                1B
 + struct Basket            N
- Bakset 2                    
 + EmptyFlag                1B
 + struct Basket            N
...
*/
#define  START_ADDRESS    0x000080
#define  FLASH_HEADER_SIZE  0x03
#define  BASKET_FLAG        "&"
#define  BASKET_FLAG_SIZE   0x01
/**
*/
typedef   struct{
  uint8   num;
  uint16  size;
}FlashHeader;
/**
*/
typedef   struct{
  uint8   flag;
  Basket  data;
}FlashMember;
static FlashMember   *pMember;
static FlashHeader  InfoBaskets;
/**
* @fn       FlashReset
* @brief    Clear all Basket, init header
* @param    none
* @return   none
*/
void FlashReset(void){
  InfoBaskets.num=0;
  InfoBaskets.size=sizeof(Basket)+1;
  FlashWrite(START_ADDRESS,(uint8*) &InfoBaskets,FLASH_HEADER_SIZE);
}
/**
* @fn       FindBasket
* @brief	
* @param    pID, pMember!=NULL
* @return:  address of FlashMember, else 0
 */
ST_uint32 FindBasket(uint8 *pID)
{
  uint8   i;
  ST_uint32 addr,rt=0;
  if(pMember==NULL)
    pMember=(FlashMember *)osal_mem_alloc(sizeof(FlashMember));
  FlashRead(START_ADDRESS,(uint8*) &InfoBaskets,FLASH_HEADER_SIZE);
  i=0;
  while(i<InfoBaskets.num)
  {
    addr=(ST_uint32)(START_ADDRESS+FLASH_HEADER_SIZE+InfoBaskets.size*i);
    FlashRead(addr,(ST_uint8*)pMember,BASKET_FLAG_SIZE+BASKET_ID_LEN+PRODS_NUM_SIZE);
    if(pMember->flag && IsSameString((uint8*)pMember->data.id,pID,BASKET_ID_LEN))
      rt=addr;
    i++;
  }
  osal_mem_free((uint8*)pMember);
  pMember=NULL;
  return rt;
}
/**
* @fn     ReadBasket
* @brief
* @param  pID, note: pMember
* @return NULL: not found, other:found
*/
Basket* ReadBasket(uint8 *pID)
{
  ST_uint32  addr;
  addr = FindBasket(pID);
  if(addr){
	if(pMember==NULL)
		pMember=(FlashMember *)osal_mem_alloc(sizeof(FlashMember));
    FlashRead((ST_uint32)addr+BASKET_FLAG_SIZE+BASKET_ID_LEN+PRODS_NUM_SIZE,
              (ST_uint8*)(pMember->data.prods),(pMember->data.len)*(sizeof(Product)));
    return &pMember->data;
  }else
    return NULL;
}
/**
* @return 0: success, 1: fail, not exist
*/
uint8 EraseBasket(uint8 *pID)
{
  uint8   i=0,rt;
  ST_uint32  addr;
  addr = FindBasket(pID);
  if(addr)
  {
    /// Clear Flag of FlashMember (Basket)
     FlashWrite((ST_uint32)(addr),&i,1);
     //Update flash header
     InfoBaskets.num--;
     FlashWrite(START_ADDRESS,(uint8*) &InfoBaskets,FLASH_HEADER_SIZE);
     rt= 0;
  }else
    rt= 1;
  if(pMember!=NULL)
  {
    osal_mem_free((uint8*)pMember);
    pMember=NULL;
  }
  return rt;
}
/**
* @return 0: Success
*/
uint8 WriteBasket(Basket *pBasket)
{
  uint8   i,flag;
  ST_uint32 addr;
  if(pBasket==NULL)
    return 1;
  //Read Flash Header
  FlashRead(START_ADDRESS,(uint8*) &InfoBaskets,FLASH_HEADER_SIZE);
  //Find blank Basket
  i=0;
  while(i<InfoBaskets.num)
  {
    addr=(ST_uint32)(START_ADDRESS+FLASH_HEADER_SIZE+InfoBaskets.size*i);
    FlashRead(addr,(ST_uint8*)&flag,1);
    if(flag==0)
      break;
    i++;
  }
  addr=(ST_uint32)(START_ADDRESS+FLASH_HEADER_SIZE+InfoBaskets.size*i);
  //Turn on FLag
  FlashWrite(addr,BASKET_FLAG,1);
  //Write Basket to flash
  FlashWrite(addr+BASKET_FLAG_SIZE,(uint8*)pBasket,
             BASKET_ID_LEN+PRODS_NUM_SIZE+(pBasket->len)*(sizeof(Product)));
  //Update flash header
  if(i >= InfoBaskets.num){
    InfoBaskets.num++;
    FlashWrite(START_ADDRESS,(uint8*) &InfoBaskets,FLASH_HEADER_SIZE);
  }
  return 0;
}
