/**
 * @file BC_Scanner.c
 *
 */
#include "hal_uart.h"
#include "OSAL.h"
#include "buffer.h"
#include "common.h"
#include "HandheldApp.h" //For HandheldApp_TaskID,task event
#include "BC_Scanner.h"


//Configure UART port fot BC Scanner
#define BC_SCANNER_PORT      HAL_UART_PORT_0
#define BC_SCANNER_BAUD      HAL_UART_BR_9600
#define BC_SCANNER_THRESH    64
#define BC_SCANNER_RX_SZ     128
#define BC_SCANNER_TX_SZ     128
#define BC_SCANNER_IDLE      6
#define BC_SCANNER_LOOPBACK  FALSE
#define BC_SCANNER_TX_MAX    80
#define BC_SCANNER_RSP_CNT   4

extern byte HandheldApp_TaskID;
extern ringBuf_t ScannerBuf;
/*
* @fn       BC_ScannerInit
* @brief    initialize barcode scanner UART
*
* @param    none
* @return   none
*/
void BC_ScannerInit(void){
  halUARTCfg_t uartConfig;
  uartConfig.configured           = TRUE;              // 2x30 don't care - see uart driver.
  uartConfig.baudRate             = BC_SCANNER_BAUD;
  uartConfig.flowControl          = TRUE;
  uartConfig.flowControlThreshold = BC_SCANNER_THRESH; // 2x30 don't care - see uart driver.
  uartConfig.rx.maxBufSize        = BC_SCANNER_RX_SZ;  // 2x30 don't care - see uart driver.
  uartConfig.tx.maxBufSize        = BC_SCANNER_TX_SZ;  // 2x30 don't care - see uart driver.
  uartConfig.idleTimeout          = BC_SCANNER_IDLE;   // 2x30 don't care - see uart driver.
  uartConfig.intEnable            = TRUE;              // 2x30 don't care - see uart driver.
  uartConfig.callBackFunc         = BC_Scanner_CallBack;
  HalUARTOpen (BC_SCANNER_PORT, &uartConfig);
}
/*********************************************************************
 * @fn      BC_Scanner_CallBack
 * @brief   Process all event form UART that connect to barcode scannner
 *
 * @param   - port of the UART
 *          - event code
 *
 * @return  none
 */
void BC_Scanner_CallBack(uint8 port, uint8 event){
  
  //if rx_buffer have data, read it and push to basket_buff
  if (event && (HAL_UART_RX_FULL | HAL_UART_RX_ABOUT_FULL)){
    //Copy to Ring Buffer
    uint16 rx_buf_len = Hal_UART_RxBufLen (BC_SCANNER_PORT);
    uint8 tmp_buf[8],count;
    while(rx_buf_len !=0){
      count = HalUARTRead(BC_SCANNER_PORT, tmp_buf, 8);
      if(count ==0){
        //underflow;
        break;
      }
      if(rx_buf_len <= 8 && tmp_buf[count-1]==0x0d)
      {
        tmp_buf[count-1]=0;
        //Send a Event to Handheld Task;
        osal_set_event(HandheldApp_TaskID, 1);
      }
      count = bufPut(&ScannerBuf,tmp_buf,count);
      if(count ==0){
        //buffer overflow; 
        break;
      }
      rx_buf_len -=count;
    }
  }
}