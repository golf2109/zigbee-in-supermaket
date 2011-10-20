/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"
#include "AF.h"
#include "OnBoard.h"
#include "OSAL_Tasks.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "ZDProfile.h"
#include "DebugTrace.h"

/* HAL */
#include "hal_drivers.h"
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_uart.h"

/*USER CODE*/
#include "../common.h"
#include "cashier.h"
#include "buffer.h"

/*********************************************************************
 * SECURITY
 */

/*********************************************************************
 * BARCODE SCANNER
 */
 ringBuf_t basket_buff;
/*********************************************************************
 * @brief   Initialization UART use for receive data from barcode scanner
 *
 * @param   0 or 1
 *
 * @return  none
 */
void UART_Init( uint8 port ){
 
  halUARTCfg_t uartConfig;
  
  bufInit(&basket_buff);
    
  uartConfig.configured           = TRUE;
  uartConfig.baudRate             = UART_SCANNER_BAUD;
  uartConfig.flowControl          = TRUE;
  uartConfig.flowControlThreshold = UART_SCANNER_THRESH;
  uartConfig.rx.maxBufSize        = UART_SCANNER_RX_SIZE;
  uartConfig.tx.maxBufSize        = UART_SCANNER_TX_SIZE;
  uartConfig.idleTimeout          = UART_SCANNER_TIMEOUT;
  uartConfig.intEnable            = TRUE;
  uartConfig.callBackFunc         = UART_SCANNER_process_evt;
  if(!port)
	HalUARTOpen (HAL_UART_PORT_0, &uartConfig);
  else
	HalUARTOpen (HAL_UART_PORT_1, &uartConfig);
}

/*********************************************************************
 * @brief   read every basket_id from rx_buff and put it to ringBuf_t
 *
 * @param   none
 *
 * @return  none
 */
static void read_scanner(void){
	uint8 tmp_buf[BASKET_ID_LEN];
	HalUARTRead(UART_SCANNER_PORT, tmp_buf, BASKET_ID_LEN);
	bufPut(&basket_buff,tmp_buf,BASKET_ID_LEN);
}
/*********************************************************************
 * @brief   Process all event form UART that connect to barcode scannner
 *
 * @param   - port of the UART
 *			- event code
 *
 * @return  none
 */
static void UART_SCANNER_process_evt(uint8 port, uint8 event){
	if (event & (HAL_UART_RX_FULL | HAL_UART_RX_ABOUT_FULL)){
		uint16 rx_buf_len = Hal_UART_RxBufLen (UART_SCANNER_PORT);
		uint8 num_basketId = rx_buf_len/BASKET_ID_LEN,i;
		for(i=0;i<num_basketId;i++)
			read_scanner();
	}
}

 /*********************************************************************
 * DIGI ME 9210
 */
 
/*********************************************************************
 * ZIGBEE
 */
 
afAddrType_t BrdAddr;	//broadcast addr
afAddrType_t DesAddr;	//destination addr
afAddrType_t SrcAddr;	//source addr

const cId_t ClusterList[MAX_CLUSTERS] = {

  CLUSTERID
};

const SimpleDescriptionFormat_t SimpleDesc = {

  ENDPOINT,              //  int Endpoint;
  PROFID,                //  uint16 AppProfId[2];
  DEVICEID,              //  uint16 AppDeviceId[2];
  DEVICE_VERSION,        //  int   AppDevVer:4;
  FLAGS,                 //  int   AppFlags:4;
  MAX_CLUSTERS,          //  byte  AppNumInClusters;
  (cId_t *)ClusterList,  //  byte *pAppInClusterList;
  MAX_CLUSTERS,          //  byte  AppNumInClusters;
  (cId_t *)ClusterList   //  byte *pAppInClusterList;
};

endPointDesc_t epDesc;
 
/*********************************************************************
 * @brief   Initialization function for the Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notificaiton ... ).
 *
 * @param   task_id - the ID assigned by OSAL.  This ID should be
 *                    used to send messages and set timers.
 *
 * @return  none
 */
void cashier_Init( byte task_id ){

  byte TaskID = task_id;
  
  //Set up for UART
  UART_Init(UART_SCANNER_PORT);
  
  // Setup for the destination address - Group 1
  BrdAddr.addrMode = (afAddrMode_t)AddrBroadcast;
  BrdAddr.endPoint = ENDPOINT;
  BrdAddr.addr.shortAddr = 0xffff;
  
  // Fill out the endpoint description.
  epDesc.endPoint = ENDPOINT;
  epDesc.task_id = &TaskID;
  epDesc.simpleDesc
            = (SimpleDescriptionFormat_t *)&SimpleDesc;
  epDesc.latencyReq = noLatencyReqs;

  // Register the endpoint description with the AF
  afRegister( &epDesc );

  // Register for all key events - This app will handle all key events
  RegisterForKeys( TaskID );
  
  ZDO_RegisterForZDOMsg( TaskID, End_Device_Bind_rsp );
  ZDO_RegisterForZDOMsg( TaskID, Match_Desc_rsp );
}

/*********************************************************************
 * @brief   Client Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id  - The OSAL assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  none
 */
UINT16 cashier_ProcessEvent( byte task_id, UINT16 events ){

  afIncomingMSGPacket_t *MSGpkt;
  afDataConfirm_t *afDataConfirm;

  // Data Confirmation message fields
  byte sentEP;
  ZStatus_t sentStatus;
  byte sentTransID;       // This should match the value sent
  (void)task_id;  // Intentionally unreferenced parameter

  if ( events & SYS_EVENT_MSG ){ 
  
    MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( task_id );
    while ( MSGpkt ){
	
      switch ( MSGpkt->hdr.event ){
	  
        case ZDO_CB_MSG: // ZDO incoming message callback
          break;
          
        case KEY_CHANGE: // Key Events
          break;

        case AF_DATA_CONFIRM_CMD: // Data confirmation
          break;

        case AF_INCOMING_MSG_CMD: // Incoming MSG type message
          break;

        case ZDO_STATE_CHANGE: // ZDO has changed the device's network state
          break;

        default:
          break;
      }

      // Release the memory
      osal_msg_deallocate( (uint8 *)MSGpkt );

      // Next
      MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( task_id );
    }

    // return unprocessed events
    return (events ^ SYS_EVENT_MSG);
  }

  return 0;
}