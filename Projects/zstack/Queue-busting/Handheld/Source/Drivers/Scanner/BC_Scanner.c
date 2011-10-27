#include "hal_uart.h"
#include "BC_Scanner.h"
//Configure UART port fot BC Scanner
#define BC_SCANNER_PORT      0
#define BC_SCANNER_BAUD      HAL_UART_BR_9600
#define BC_SCANNER_THRESH    64
#define BC_SCANNER_RX_SZ     128
#define BC_SCANNER_TX_SZ     128
#define BC_SCANNER_IDLE      6
#define BC_SCANNER_LOOPBACK  FALSE
#define BC_SCANNER_TX_MAX    80
#define BC_SCANNER_RSP_CNT   4
static void BC_Scanner_CallBack(uint8 port, uint8 event);
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
 * @fn      BC_Scanner
 * @brief   Process all event form UART that connect to barcode scannner
 *
 * @param   - port of the UART
 *          - event code
 *
 * @return  none
 */
static void BC_Scanner_CallBack(uint8 port, uint8 event){
  //if rx_buffer have data, read it and push to basket_buff
  if (event && (HAL_UART_RX_FULL | HAL_UART_RX_ABOUT_FULL)){
    //Copy to Ring Buffer
    //Send a Event to Handheld Task;
  }
}