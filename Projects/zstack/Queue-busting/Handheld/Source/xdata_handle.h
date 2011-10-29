#ifndef  _EXTERNAL_DATA_H_
#define  _EXTERNAL_DATA_H_
#include "common.h"
void FlashReset(void);
Basket* ReadBasket(uint8 *pID);
uint8 WriteBasket(Basket *pBasket);
uint8 EraseBasket(uint8 *pID);
#endif //_EXTERNAL_DATA_H_