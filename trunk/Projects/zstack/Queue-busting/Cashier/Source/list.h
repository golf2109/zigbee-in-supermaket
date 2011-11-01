#ifndef LIST_H
#define LIST_H

#include <hal_types.h>
#include "common.h"

/*********************************************************************
 * COMMON VARIABLES
 */
typedef struct List{
  struct  List *next;
  uint8 pData[BASKET_ID_LEN];
  uint8 len;
} Buf;
/*********************************************************************
 * COMMON FUNCTIONS
 */
Buf* bufPush(Buf* l, uint8* data, uint8 len);
Buf* bufPop(Buf* l, uint8* data, uint8* len);
uint8 bufCount(Buf* l);

 
#endif