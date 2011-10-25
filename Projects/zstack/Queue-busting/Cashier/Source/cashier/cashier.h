#ifndef CASHIER_H
#define CASHIER_H

/*********************************************************************
 * BARCODE SCANNER
 */
#define UART_SCANNER_PORT		0x00		//UART0
#define UART_SCANNER_BAUD		0x00		//speed of UART
/*9600   0x00
 *19200  0x01
 *38400  0x02
 *57600  0x03
 *115200 0x04*/
 
#define UART_SCANNER_RX_SIZE	24*BASKET_ID_LEN	//max size of rx_buff
#define UART_SCANNER_TX_SIZE	0			//max size of tx_buff
#define UART_SCANNER_THRESH	1//BASKET_ID_LEN	//thresh of rx_buff, call back func will be call
#define UART_SCANNER_TIMEOUT	100			//time rx_buff doesnt receive, call back func will be call

void UART_Init( uint8 port );
static void read_scanner(void);
static void UART_SCANNER_process_evt(uint8 port, uint8 event);
static void UART_SCANNER_print(void);
static bool Check_basket_id_format(char* id);
static bool Check_basket_id(char* id);
void HandleKeys( byte shift, byte keys );
void SendMessage(afAddrType_t dstAddr, char* message);

/*********************************************************************
 * DIGI ME 9210
 */

/*********************************************************************
 * MAIN (ZIGBEE)
 */
void cashier_Init( byte task_id );
UINT16 cashier_ProcessEvent( byte task_id, UINT16 events );
#endif