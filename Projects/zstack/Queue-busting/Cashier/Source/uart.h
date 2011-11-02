#ifndef   UART_H
#define   UART_H

#include "common.h"
#include "list.h"

/*********************************************************************
 * BARCODE SCANNER
 */
#define UART_SCANNER_PORT		HAL_UART_PORT_1
#define UART_SCANNER_BAUD		0x00		//speed of UART
/*9600   0x00
 *19200  0x01
 *38400  0x02
 *57600  0x03
 *115200 0x04*/
 
#define UART_SCANNER_RX_SIZE	24*BASKET_ID_LEN	//max size of rx_buff
#define UART_SCANNER_TX_SIZE	0			//max size of tx_buff
#define UART_SCANNER_THRESH	1//BASKET_ID_LEN	//thresh of rx_buff, call back func will be call
#define UART_SCANNER_TIMEOUT	6			//time rx_buff doesnt receive, call back func will be call
#define UART_SCANNER_EVENT      0x0001
/*********************************************************************
 * PC
 */
#define UART_PC_PORT		HAL_UART_PORT_0
#define UART_PC_BAUD		0x00		            //9600
#define UART_PC_RX_SIZE         10*PC_CMD_LEN               //max size of rx_buff
#define UART_PC_TX_SIZE         MAX_PRODS*(PRODS_ID_LEN+1)  //max size of tx_buff
#define UART_PC_THRESH          1//BASKET_ID_LEN	    //thresh of rx_buff, call back func will be call
#define UART_PC_TIMEOUT         6			    //time rx_buff doesnt receive, call back func will be call
#define UART_PC_EVENT           0x0002

/*********************************************************************
 *   GLOBAL VARIALES
 */
extern Buf* basket_buff;
extern Buf* data_buff;
extern Buf* pc_buff;

/*********************************************************************
 *   FUNCTION
 */
//For Barcode Scanner
void UART_Scanner_Init(void);
extern uint8 __xdata ScannerRxBuffer[20]; 
extern uint8 __xdata ScannerRxIndex; 
//For PC
void UART_PC_Init(void);
static void UART_PC_process_evt(uint8 port, uint8 event);

#endif