#include <hal_types.h>
#include "Serialize.h"
#include "dataflash.h"
#include "common.h"
#include "xdata_handle.h"
/*
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
/*
* return: Addr != 0 if found
*/
#define  START_ADDRESS    0x000080
#define  FLASH_HEADER_SIZE  0x03
typedef   struct{
  uint8   num;
  uint16  size;
}FlashHeader;
typedef   struct{
  uint8   flag;
  Basket  data;
}FlashMember;
static FlashMember   tmpMember;
static FlashHeader  InfoBaskets;

/*
* return NULL: not found, other:found
*/
Basket* ReadBasket(uint8 *pID)
{
  uint8   i;
  FlashRead(START_ADDRESS,(uint8*) &InfoBaskets,FLASH_HEADER_SIZE);
  i=0;
  while(i<InfoBaskets.num)
  {
    FlashRead((ST_uint32)START_ADDRESS+FLASH_HEADER_SIZE+InfoBaskets.size*i,
              (ST_uint8*)&tmpMember,InfoBaskets.size);
    if(tmpMember.flag && IsSameString((uint8*)tmpMember.data.basket_id,pID,BASKET_ID_LEN))
      return &tmpMember.data;
    i++;
  }
  return NULL;
}
//return 0: success, 1: fail, not exist
uint8 EraseBasket(uint8 *pID)
{
  uint8   i;
  FlashRead(START_ADDRESS,(uint8*) &InfoBaskets,FLASH_HEADER_SIZE);
  i=0;
  while(i<InfoBaskets.num)
  {
    FlashRead((ST_uint32)START_ADDRESS+FLASH_HEADER_SIZE+InfoBaskets.size*i,
              (ST_uint8*)&tmpMember,InfoBaskets.size);
    if(tmpMember.flag && IsSameString((uint8*)tmpMember.data.basket_id,pID,BASKET_ID_LEN))
    {
      i =0;
      FlashWrite((ST_uint32)START_ADDRESS+FLASH_HEADER_SIZE+InfoBaskets.size*i,
              &i,1);
      return 0;
    }
    i++;
  }
  return 1;
}
uint8 WriteBasket(Basket *pBasket)
{
  uint8   i,flag=0;
  FlashRead(START_ADDRESS,(uint8*) &InfoBaskets,FLASH_HEADER_SIZE);
  i=0;
  while(i<InfoBaskets.num)
  {
    FlashRead((ST_uint32)START_ADDRESS+FLASH_HEADER_SIZE+InfoBaskets.size*i,
              (ST_uint8*)&flag,1);
    if(flag==0)
      break;
  }
  //Write to flash
  FlashWrite((ST_uint32)START_ADDRESS+FLASH_HEADER_SIZE+InfoBaskets.size*i,
              "&",1);
  FlashWrite((ST_uint32)START_ADDRESS+FLASH_HEADER_SIZE+InfoBaskets.size*i+1,
              (uint8*)pBasket,1);
  //Update flash header
  InfoBaskets.num++;
  FlashWrite(START_ADDRESS,(uint8*) &InfoBaskets,FLASH_HEADER_SIZE);
  return 0;
}
