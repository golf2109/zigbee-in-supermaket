#ifndef  _EXTERNAL_DATA_H_
#define  _EXTERNAL_DATA_H_
#include "common.h"
#include "ZComDef.h"
void FlashReset(void);
uint32 FindBasket(uint8 *pID);
byte * ReadBasket(uint8 *pID,uint16 *len);
uint8 WriteBasket(Basket *pBasket);
uint8 EraseBasket(uint8 *pID);
#endif //_EXTERNAL_DATA_H_