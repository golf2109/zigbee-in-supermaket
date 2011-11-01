#ifndef CASHIER_H
#define CASHIER_H

#include <ZComDef.h>
#include <AF.h>
#include "list.h"

/*********************************************************************
 *   GLOBAL VARIABLES
 */

extern byte Cashier_TaskID;
extern uint8 __xdata ScannerRxBuffer[20]; 
extern uint8 __xdata ScannerRxIndex; 
extern uint8 ready_to_bcast;

/*********************************************************************
 *   FUNCTION
 */
//For data retrieve
static bool Check_basket_id_format(char* id);
static bool Check_basket_id(char* id);
//For Barcode Scanner
void UART_Scanner_Init(void);
//For PC
void UART_PC_Init(void);
static void UART_PC_process_evt(uint8 port, uint8 event);

/*********************************************************************
 * DIGI ME 9210
 */

/*********************************************************************
 * MAIN (ZIGBEE)
 */
void cashier_Init( byte task_id );
uint16 cashier_ProcessEvent( byte task_id, UINT16 events );
void HandleKeys( byte shift, byte keys );
void SendMessage(afAddrType_t dstAddr, char* message);
#endif