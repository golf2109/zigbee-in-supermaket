/***********************************************************************************
* INCLUDES
*/
#include <hal_types.h>
#include "buffer.h"
#include "hal_board.h"
#include "hal_int.h"
#include "hal_assert.h"
//#include <stdio.h>
/***********************************************************************************
* GLOBAL FUNCTIONS
*/

/***********************************************************************************
* @fn      bufInit
*
* @brief   Initialise a ringbuffer. The buffer must be allocated by the
*          application.
*
* @param   pBuf - pointer to the ringbuffer
*
* @return  none
*/
void bufInit(ringBuf_t *pBuf)
{
    uint16 s;

    // Critical section start
    s = halIntLock();

    pBuf->nBytes= 0;
    pBuf->pHead= 0;
    pBuf->pTail= 0;

    // Critical section end
    halIntUnlock(s);
}


/***********************************************************************************
* @fn      bufPut
*
* @brief   Add bytes to the buffer.
*
* @param   pBuf - pointer to the ringbuffer
*          pData - pointer to data to be appended to the buffer
*          nBytes - number of bytes
*
* @return  Number of bytes copied to the buffer
*/
uint8 bufPut(ringBuf_t *pBuf, const uint8 *pData, uint8 nBytes)
{
    uint8 i;
    uint16 s;

    // Critical section start
    s = halIntLock();
    
    if (pBuf->nBytes+nBytes < BUF_SIZE) {

        i= 0;
        while(i<nBytes) {
            pBuf->pData[pBuf->pTail]= pData[i];
            pBuf->pTail++;
            if ((pBuf->pTail+1)==BUF_SIZE)
                pBuf->pTail= 0;
            i++;
        }
        pBuf->nBytes+= i;
    } else {
        i= 0;
    }

    // Critical section end
    halIntUnlock(s);

    return i;
}


/***********************************************************************************
* @fn      bufGet
*
* @brief   Extract bytes from the buffer.
*
* @param   pBuf   - pointer to the ringbuffer
*          pData  - pointer to data to be extracted
*          nBytes - number of bytes
*
* @return  Bytes actually returned
*/
uint8 bufGet(ringBuf_t *pBuf, uint8 *pData, uint8 nBytes)
{
    uint8 i;
    uint16 s;

    // Critical section start
    s = halIntLock();

    i= 0;
    while(i<nBytes && i<pBuf->nBytes) {
        pData[i]= pBuf->pData[pBuf->pHead];
        pBuf->pHead++;
        if ((pBuf->pHead+1)==BUF_SIZE)
            pBuf->pHead= 0;
        i++;
    }
    pBuf->nBytes-= i;

    // Critical section end
    halIntUnlock(s);
    return i;
}


/***********************************************************************************
* @fn      bufPeek
*
* @brief   Read bytes from the buffer but leave them in the queue.
*
* @param   pBuf   - pointer to the ringbuffer
*          pData  - pointer to data to be extracted
*          nBytes - number of bytes
*
* @return  Bytes actually returned
*/
uint8 bufPeek(ringBuf_t *pBuf, uint8 *pData, uint8 nBytes)
{
    uint8 i,j;
    uint16 s;

    // Critical section start
    s = halIntLock();

    i= 0;
    j= pBuf->pHead;
    while(i<nBytes && i<pBuf->nBytes) {
        pData[i]= pBuf->pData[j];
        j++;
        if ((j+1)==BUF_SIZE)
            j= 0;
        i++;
    }

    // Critical section end
    halIntUnlock(s);
    return i;
}


/***********************************************************************************
* @fn      bufNumBytes
*
* @brief   Return the byte count for the ring buffer.
*
* @param   pBuf- pointer to the buffer
*
* @return  Number of bytes present.
*/
uint8 bufSize(ringBuf_t *pBuf)
{
    return pBuf->nBytes;
}
