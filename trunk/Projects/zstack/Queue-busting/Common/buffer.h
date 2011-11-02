#ifndef BUFFER_H
#define BUFFER_H

/***********************************************************************************
* INCLUDES
*/
#include <hal_types.h>

/***********************************************************************************
* CONSTANTS AND DEFINES
*/
#ifndef BUF_SIZE
#define BUF_SIZE	32
#endif

#if (BUF_SIZE > 256)
#error "Buffer size too big"
#endif

/***********************************************************************************
* TYPEDEFS
*/
typedef struct {
    volatile uint8 pData[BUF_SIZE];
    volatile uint8 nBytes;
    volatile uint8 pHead;
    volatile uint8 pTail;
} ringBuf_t;

/***********************************************************************************
* MACROS
*/
#define bufFlush(pBuf)  bufInit(pBuf)

/***********************************************************************************
* GLOBAL FUNCTIONS
*/
void  bufInit(ringBuf_t *pBuf);
uint8 bufPut(ringBuf_t *pBuf, const uint8 *pData, uint8 n);
uint8 bufGet(ringBuf_t *pBuf, uint8 *pData, uint8 n);
uint8 bufPeek(ringBuf_t *pBuf, uint8 *pData, uint8 nBytes);
uint8 bufSize(ringBuf_t *pBuf);

#endif
