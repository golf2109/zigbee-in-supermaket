/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"
#include "AF.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "ZDProfile.h"
#include "DebugTrace.h"

/* HAL */
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_uart.h"

/*USER CODE*/
#include "../common.h"
#include "client.h"

/*********************************************************************
 * SECURITY
 */

/*********************************************************************
 * CONNECT TO HANDHELD SCANNER
 */
 
 /*********************************************************************
 * FLASH MEMOMRY
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
 * THIS FUNCTION IS FOR REF. CHANGE IF NEED
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
void client_Init( byte task_id ){

  TaskID = task_id;
  
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
 * THIS FUNCTION IS FOR REF. CHANGE IF NEED
 *
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
UINT16 client_ProcessEvent( byte task_id, UINT16 events ){

  afIncomingMSGPacket_t *MSGpkt;
  afDataConfirm_t *afDataConfirm;

  // Data Confirmation message fields
  byte sentEP;
  ZStatus_t sentStatus;
  byte sentTransID;       // This should match the value sent
  (void)task_id;  // Intentionally unreferenced parameter

  if ( events & SYS_EVENT_MSG ){ 
  
    MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( TaskID );
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
      MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( TaskID );
    }

    // return unprocessed events
    return (events ^ SYS_EVENT_MSG);
  }

  return 0;
}