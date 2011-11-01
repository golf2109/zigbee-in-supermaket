#include <OSAL.h>
#include "list.h"

Buf* bufPush(Buf* l, uint8* data, uint8 len){
  Buf* new_Buf = osal_mem_alloc(sizeof(Buf));
  new_Buf->next = NULL;
  new_Buf->len = len;
  osal_memcpy(new_Buf->pData,data,len);
  if(l==NULL){
    l = new_Buf;
  }
  else{
    Buf* tmp = l;
    while(tmp->next)
      tmp = tmp->next;
    tmp->next = new_Buf;
  }
  return l;
}

Buf* bufPop(Buf* l, uint8* data, uint8* len){
  if(l){
    osal_memcpy(data,l->pData,l->len);
    *len = l->len;
    Buf* tmp = l;
    l = l->next;
    osal_mem_free(tmp);
  }
  return l;
}

uint8 bufCount(Buf* l){
  if(!l) return 0;
  uint8 ret = 1;
  Buf* tmp = l;
  while(tmp->next){
    ret++;
    tmp = tmp->next;
  }
  return ret;
}