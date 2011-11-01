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
#include "list.h"
#include "uart.h"

/*********************************************************************
 * ADDRESS
 */
afAddrType_t BrdAddr;	//broadcast addr
afAddrType_t DesAddr;	//destination addr
afAddrType_t SrcAddr;	//source addr

uint8 ready_to_bcast;   //take role as signal
/*********************************************************************
 * @brief   Check if it is basket_id format
 *
 * @param   basket_id
 *
 * @return  TRUE if it's basket id
 */
static bool Check_basket_id_format(char* id){
  if(*(id)=='#')  return TRUE;
  else return FALSE;
}

/*********************************************************************
 * @brief   Check if this device have this basket_id
 *
 * @param   basket_id
 *
 * @return  TRUE if have
 */
static bool Check_basket_id(char* id){
  if(*(id)=='#')  return TRUE;
  else return FALSE;
}

 /*********************************************************************
 * DIGI ME 9210
 */
 
/*********************************************************************
 * ZIGBEE
 */
 
devStates_t Cashier_NwkState;
byte Cashier_TaskID;
byte Cashier_TransID;

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
  
  Cashier_NwkState = DEV_INIT;
  Cashier_TaskID = task_id;
  Cashier_TransID = 0;

  //Set up for UART
  UART_PC_Init();
  UART_Scanner_Init();
  
  // Setup for the destination address - Group 1
  BrdAddr.addrMode = (afAddrMode_t)AddrBroadcast;
  BrdAddr.endPoint = ENDPOINT;
  BrdAddr.addr.shortAddr = 0xffff;
  
  // Fill out the endpoint description.
  epDesc.endPoint = ENDPOINT;
  epDesc.task_id = &Cashier_TaskID;
  epDesc.simpleDesc
            = (SimpleDescriptionFormat_t *)&SimpleDesc;
  epDesc.latencyReq = noLatencyReqs;

  // Register the endpoint description with the AF
  afRegister( &epDesc );

  // Register for all key events - This app will handle all key events
  RegisterForKeys( Cashier_TaskID );
  
  ZDO_RegisterForZDOMsg( Cashier_TaskID, End_Device_Bind_rsp );
  ZDO_RegisterForZDOMsg( Cashier_TaskID, Match_Desc_rsp );
  
  //Ready to Work
  ready_to_bcast = 1;
  HalLedSet ( HAL_LED_1, HAL_LED_MODE_ON );
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
  
  if(events == UART_SCANNER_EVENT){
    uint8 len;
    uint8 _tmp[16];
    char tmp[16];
    basket_buff = bufPop(basket_buff, _tmp, &len);
    osal_memcpy(tmp,_tmp,16);
    if(ready_to_bcast){
      SendMessage(BrdAddr, tmp);
      ready_to_bcast = 0;
    }
    HalUARTWrite(UART_PC_PORT, (char*)tmp, len);
  }
  else if(events == UART_PC_EVENT){
    //Data transfer from computer stor in pc_buff
    uint8 len;
    uint8 _tmp[16];
    char tmp[16];
    pc_buff = bufPop(pc_buff, _tmp, &len);
    osal_memcpy(tmp,_tmp,16);
    if(ready_to_bcast && Check_basket_id_format(tmp)){
      SendMessage(BrdAddr, tmp);
      ready_to_bcast = 0;
    }
    HalUARTWrite(UART_PC_PORT, (char*)tmp, len);

    //check if basket_buff is have data
    //uint8 have_data = bufCount(basket_buff);
    //osal_set_event(Cashier_TaskID, UART_SCANNER_EVENT);
    uint8 have_data = bufCount(pc_buff);
    if(have_data>0)
      osal_set_event(Cashier_TaskID, UART_PC_EVENT);
  }

  else if ( events & SYS_EVENT_MSG ){
      
    MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( task_id );
    while ( MSGpkt ){
	
      switch ( MSGpkt->hdr.event ){
	  
        case ZDO_CB_MSG: // ZDO incoming message callback
          printText("ZDO_CB_MSG",3);
          break;
          
        case KEY_CHANGE: // Key Events
          HandleKeys( ((keyChange_t *)MSGpkt)->state, ((keyChange_t *)MSGpkt)->keys );
          break;

        case AF_DATA_CONFIRM_CMD: // Data confirmation
          afDataConfirm = (afDataConfirm_t *)MSGpkt;
          sentEP = afDataConfirm->endpoint;
          sentStatus = afDataConfirm->hdr.status;
          sentTransID = afDataConfirm->transID;
          (void)sentEP;
          (void)sentTransID;          
          // Action taken when confirmation is received.
          if ( sentStatus != ZSuccess ){
            // The data wasn't delivered -- Do something
          }
          break;

        case AF_INCOMING_MSG_CMD: // Incoming MSG type message
          //print data of this message for debug
          printText((char*)MSGpkt->cmd.Data,3);
#ifdef  CLIENT
          //if i have this basket_id
          HalLedSet ( HAL_LED_3, HAL_LED_MODE_ON );
          if(Check_basket_id((char*)MSGpkt->cmd.Data)==TRUE){
            char* prod = "123456789 123456789 123456789 ";
            DesAddr = MSGpkt->srcAddr;
            //SendMessage(DesAddr,(char*)MSGpkt->cmd.Data);
            SendMessage(DesAddr,prod);
          }
          HalLedSet ( HAL_LED_3, HAL_LED_MODE_OFF );
#endif
#ifdef  CASHIER
          /*if(Check_basket_id_format((char*)MSGpkt->cmd.Data)==TRUE)
            start_receive_prods = 1;
          if(start_receive_prods){
            HalUARTWrite(UART_PC_PORT, MSGpkt->cmd.Data, MSGpkt->cmd.DataLength);
          }*/
          unsigned char tmp[80];
          osal_memset(tmp,0,80);
          osal_memcpy(tmp,MSGpkt->cmd.Data,MSGpkt->cmd.DataLength);
          HalUARTWrite(UART_PC_PORT, tmp, MSGpkt->cmd.DataLength);
          ready_to_bcast = 1;
#endif
          break;

        case ZDO_STATE_CHANGE: // ZDO has changed the device's network state
          //printText("ZDO_STATE_CHANGE","");
          Cashier_NwkState = (devStates_t)(MSGpkt->hdr.status);
          if ( (Cashier_NwkState == DEV_ZB_COORD)
              || (Cashier_NwkState == DEV_ROUTER)
              || (Cashier_NwkState == DEV_END_DEVICE) ){
                
            // Start sending "the" message in a regular interval.
            osal_start_timerEx( Cashier_TaskID, 0x001, TIMEOUT );
          }
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

/*********************************************************************
 * @brief   Handle all key 
 *
 * @param   task_id  - The OSAL assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  none
 */
void HandleKeys( byte shift, byte keys ){
  zAddrType_t dstAddr;

  // Shift is used to make each button/switch dual purpose.
  if ( shift ){
    if ( keys & HAL_KEY_SW_1 ){
    }
    if ( keys & HAL_KEY_SW_2 ){
    }
    if ( keys & HAL_KEY_SW_3 ){
    }
    if ( keys & HAL_KEY_SW_4 ){
    }
  }
  else{
    if ( keys & HAL_KEY_SW_1 ){
    }
    if ( keys & HAL_KEY_SW_2 ){
      HalLedSet ( HAL_LED_4, HAL_LED_MODE_OFF );

      // Initiate an End Device Bind Request for the mandatory endpoint
      dstAddr.addrMode = Addr16Bit;
      dstAddr.addr.shortAddr = 0x0000; // Coordinator
      ZDP_EndDeviceBindReq( &dstAddr, NLME_GetShortAddr(),
                            epDesc.endPoint,
                            PROFID,
                            MAX_CLUSTERS, (cId_t *)ClusterList,
                            MAX_CLUSTERS, (cId_t *)ClusterList,
                            FALSE );
    }

    if ( keys & HAL_KEY_SW_3 ){
    }

    if ( keys & HAL_KEY_SW_4 ){
      HalLedSet ( HAL_LED_4, HAL_LED_MODE_OFF );
      // Initiate a Match Description Request (Service Discovery)
      dstAddr.addrMode = AddrBroadcast;
      dstAddr.addr.shortAddr = NWK_BROADCAST_SHORTADDR;
      ZDP_MatchDescReq( &dstAddr, NWK_BROADCAST_SHORTADDR,
                        PROFID,
                        MAX_CLUSTERS, (cId_t *)ClusterList,
                        MAX_CLUSTERS, (cId_t *)ClusterList,
                        FALSE );
    }
  }
}

/*********************************************************************
 * @brief   Send the message 
 *
 * @param   task_id  - The OSAL assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  none
 */
void SendMessage(afAddrType_t dstAddr, char* message){
  
  char theMessageData[80];
  osal_memcpy(theMessageData,message,osal_strlen(message));
  if ( AF_DataRequest( &dstAddr, &epDesc, CLUSTERID,
                       (byte)(osal_strlen( theMessageData ) + 1),
                       (byte *)&theMessageData,
                       &Cashier_TransID,
                       AF_DISCV_ROUTE, AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
  {
    // Successfully requested to be sent.
  }
  else
  {
    // Error occurred in request to send.
  }
}