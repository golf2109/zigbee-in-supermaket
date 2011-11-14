#include "OSAL.h"
/* HAL */
#include "hal_drivers.h"
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_uart.h"
#include "uart.h"
#include "cashier/cashier.h"

uint8 __xdata ScannerRxBuffer[20]; 
uint8 __xdata ScannerRxIndex; 

Buf* basket_buff = NULL;
Buf* data_buff = NULL;
Buf* pc_buff = NULL;
uint8 start_receive_prods = 0;
 
/*********************************************************************
 * @brief   Initialization UART use for receive data from barcode scanner
 *
 * @param   0 or 1
 *
 * @return  none
 */
void UART_Scanner_Init(void){ 
  
  ScannerRxIndex=0;
  
  URX1IF = 0;
  U1CSR = 0x80;     //CSR_MODE;   //0x80
  PERCFG |= 0x02;   //Alternative 2
  P1SEL |= 0xf0;    //P1.7 -> P1.4
  
  // Initialize bitrate 9600
  U1BAUD = 59; 
  U1GCR = 8;
  
  U1UCR =0x80;      //Flush
  URX1IE = 1;       //Enable Interrupt
  U1CSR |= 0x40;    //Ready to receive data
}

/*********************************************************************
 * @brief   Initialization UART use for receive data from barcode scanner
 *
 * @param   0 or 1
 *
 * @return  none
 */
void UART_PC_Init(void){ 
  
  halUARTCfg_t uConfig;
  
  uConfig.configured           = TRUE;
  uConfig.baudRate             = UART_PC_BAUD;
  uConfig.flowControl          = FALSE;
  uConfig.flowControlThreshold = UART_PC_THRESH;
  uConfig.rx.maxBufSize        = UART_PC_RX_SIZE;
  uConfig.tx.maxBufSize        = UART_PC_TX_SIZE;
  uConfig.idleTimeout          = UART_PC_TIMEOUT;
  uConfig.intEnable            = TRUE;
  uConfig.callBackFunc         = UART_PC_process_evt;
  HalUARTOpen (UART_PC_PORT, &uConfig);
}

/*********************************************************************
 * @brief   Process all event form UART that connect to barcode scannner
 *
 * @param   - port of the UART
 *          - event code
 *
 * @return  none
 */
HAL_ISR_FUNCTION(ReadScanner,URX1_VECTOR){
  
  URX1IF = 0;
  ScannerRxBuffer[ScannerRxIndex++] = U1DBUF;
  
  if (ScannerRxIndex >= UART_SCANNER_RX_SIZE){
    ScannerRxIndex = 0;
    IEN0 &= ~0x08;
  }
  if(ScannerRxBuffer[ScannerRxIndex-1]==0x0d){
    basket_buff = bufPush(basket_buff, ScannerRxBuffer, ScannerRxIndex);
    osal_set_event(Cashier_TaskID, UART_SCANNER_EVENT);
    ScannerRxIndex = 0;
  }
  U1CSR |= 0x40; 
  IEN0 |= 0x08;
}

/*********************************************************************
 * @brief   Process all event form UART that connect to PC
 *
 * @param   - port of the UART
 *          - event code
 *
 * @return  none
 */
static void UART_PC_process_evt(uint8 port, uint8 event){
  
  (void) port;
  //read command from PC and then push it to pc_buff
  //if (event && (HAL_UART_RX_FULL | HAL_UART_RX_ABOUT_FULL)){
  uint16 len = Hal_UART_RxBufLen (UART_PC_PORT);
  if(len){
    uint8 tmp[80];
    HalUARTRead(UART_PC_PORT, tmp, len);
    pc_buff = bufPush(pc_buff, tmp, len);
    osal_set_event(Cashier_TaskID, UART_PC_EVENT);
  }
}