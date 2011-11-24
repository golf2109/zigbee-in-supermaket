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
#include "common.h"
#include "cashier.h"
#include "list.h"
#include "uart.h"

/*********************************************************************
 * ADDRESS
 */
afAddrType_t BrdAddr;	//broadcast addr
afAddrType_t DesAddr;	//destination addr

volatile uint8 have_basket;   //take role as signal
volatile uint8 have_pccmd;   //take role as signal
volatile uint8 ready_bcast;   //take role as signal
volatile uint8 to_com;   //take role as signal
uint8 add_remove_mode;    //add or remove products
uint8* basket_id_sent;
uint8* pc_cmd_buff;
uint8 basket_recv[BASKET_ID_LEN];
uint8 basket_sent[BASKET_ID_LEN];

byte pc_cmd_trans;

/*********************************************************************
 * @brief   Check if it is basket_id format
 *
 * @param   basket_id
 *
 * @return  TRUE if it's basket id
 */
static bool Check_basket_id_format(char* id){
  if(*(id) == BASKET_ID_FORMAT)  return TRUE;
  else return FALSE;
}

/*********************************************************************
 * @brief   Check if it is add or remove data
 *
 * @param   data
 *
 * @return  1 if it's add
 *          2 if it's remove
 *          3 if it's basket
 *          0 if none of above
 */
static uint8 Check_utilize_products(char* data){
  if(*(data) == ADD_PRODS_CODE)  return 1;
  else if(*(data) == DEL_PRODS_CODE)  return 2;
  //else if(*(data) == REQUEST_BASKET)  return 3;
  else return 0;
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

const cId_t ClusterList[Q_BUSTING_MAX_CLUSTERS] = {

  Q_BUSTING_CLUSTERID
};

const SimpleDescriptionFormat_t SimpleDesc = {

  Q_BUSTING_ENDPOINT,              //  int Endpoint;
  Q_BUSTING_PROFID,                //  uint16 AppProfId[2];
  Q_BUSTING_DEVICEID,              //  uint16 AppDeviceId[2];
  Q_BUSTING_DEVICE_VERSION,        //  int   AppDevVer:4;
  Q_BUSTING_FLAGS,                 //  int   AppFlags:4;
  Q_BUSTING_MAX_CLUSTERS,          //  byte  AppNumInClusters;
  (cId_t *)ClusterList,  //  byte *pAppInClusterList;
  Q_BUSTING_MAX_CLUSTERS,          //  byte  AppNumInClusters;
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
  
  // Setup for the broadcast address
  BrdAddr.addrMode = (afAddrMode_t)AddrBroadcast;
  BrdAddr.endPoint = Q_BUSTING_ENDPOINT;
  BrdAddr.addr.shortAddr = 0xffff;
  
  // Setup for the broadcast address
  DesAddr.addrMode = (afAddrMode_t)Addr16Bit;
  DesAddr.endPoint = Q_BUSTING_ENDPOINT;
  DesAddr.addr.shortAddr = 0x0000;
  
  // Fill out the endpoint description.
  epDesc.endPoint = Q_BUSTING_ENDPOINT;
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
  
  // Ready to Work
  HalLedSet ( HAL_LED_1, HAL_LED_MODE_OFF );
  
  // create user buffer for use
  basket_id_sent = osal_mem_alloc(sizeof(uint8)*(BASKET_ID_LEN*2));
  pc_cmd_buff = osal_mem_alloc(sizeof(uint8)*(BASKET_ID_LEN*5));
  add_remove_mode = 0;
  // Start timer 
  /*have_basket = 0;
  have_pccmd = 0;
  ready_bcast = 0;
  to_com = 0;
  osal_start_reload_timer(Cashier_TaskID, TIMER_EVENT, TIMER_TIME_OUT );*/
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
  uint8 len = 0,i=0;
  
  switch(events){
    case TIMER_EVENT:
      to_com = 1;
      break;
    case UART_SCANNER_EVENT:
        HalLedSet ( HAL_LED_4, HAL_LED_MODE_ON );
        basket_buff = bufPop(basket_buff, basket_id_sent, &len);
        
        if(len==2){
          add_remove_mode = Check_utilize_products((char*)basket_id_sent);
        }
        
        //Process for data_in is add/remove products
        if(add_remove_mode){
          //uint8* pid = osal_mem_alloc(sizeof(uint8)*(PRODS_ID_LEN+2));
          if(len!=(PRODS_ID_LEN+1)) break;
          uint8 pid[PRODS_ID_LEN+2];
          if(add_remove_mode == 1) pid[0] = ADD_PRODS_CODE;
          if(add_remove_mode == 2) pid[0] = DEL_PRODS_CODE;
          osal_memcpy(pid+1,basket_id_sent,PRODS_ID_LEN);
          pid[PRODS_ID_LEN+1] = 0x01 ;
          HalUARTWrite(UART_PC_PORT, pid, PRODS_ID_LEN+2);
          //osal_mem_free(pid);
          break;
        }
        
        
        //Process for data_in is basket
        *(basket_id_sent+BASKET_ID_LEN+1) = 0x0d;        
        if(Check_basket_id_format((char*)basket_id_sent)){
          i = len;
          while(i){
            *(basket_id_sent+i) = *(basket_id_sent+i-1);
            i=i-1;
          }
          *(basket_id_sent) = REQUEST_BASKET;
          SendMessage(BrdAddr, (char*)basket_id_sent);
        }
        HalLedSet ( HAL_LED_4, HAL_LED_MODE_BLINK );
        break;
    case UART_PC_EVENT:
      HalLedSet ( HAL_LED_4, HAL_LED_MODE_ON );
      pc_buff = bufPop(pc_buff, pc_cmd_buff, &len);
      HalUARTWrite(UART_PC_PORT, (char*)pc_cmd_buff, len);
      //*(pc_cmd_buff+len) = 0x0d;
      
      if(*(pc_cmd_buff) == PC_CMD_P2P){
        byte saddr1 = *(pc_cmd_buff+1);
        byte saddr2 = *(pc_cmd_buff+2);
        DesAddr.addr.shortAddr = saddr1<<2 | saddr2;
        HalUARTWrite(UART_PC_PORT, (char*)DesAddr.addr.shortAddr, 2);
        HalUARTWrite(UART_PC_PORT, (pc_cmd_buff+3), len-3);
        //SendMessage(DesAddr, (char*)(pc_cmd_buff+3));
      }
      else if(*(pc_cmd_buff) == PC_CMD_BRD){
        SendMessage(BrdAddr, (char*)(pc_cmd_buff+1));
      }
      else{}
      
      break;
      
    default:
      break;
  }
  
  if ( events & SYS_EVENT_MSG ){
      
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
          if(sentTransID == pc_cmd_trans)
            HalUARTWrite(UART_PC_PORT, "Delete OK", osal_strlen("Delete OK"));
          if ( sentStatus != ZSuccess ){
            // The data wasn't delivered -- Do something
            HalUARTWrite(UART_PC_PORT, "Cant Delete", osal_strlen("Cant Delete"));
          }
          break;

        case AF_INCOMING_MSG_CMD: // Incoming MSG type message
          SendBasketToPC(MSGpkt);          
          break;

        case ZDO_STATE_CHANGE: // ZDO has changed the device's network state
          //printText("ZDO_STATE_CHANGE","");
          Cashier_NwkState = (devStates_t)(MSGpkt->hdr.status);
          if ( (Cashier_NwkState == DEV_ZB_COORD)
              || (Cashier_NwkState == DEV_ROUTER)
              || (Cashier_NwkState == DEV_END_DEVICE) ){
                
            // Start sending "the" message in a regular interval.
            osal_start_timerEx( Cashier_TaskID, 0x001, Q_BUSTING_TIMEOUT );
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
                            Q_BUSTING_PROFID,
                            Q_BUSTING_MAX_CLUSTERS, (cId_t *)ClusterList,
                            Q_BUSTING_MAX_CLUSTERS, (cId_t *)ClusterList,
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
                        Q_BUSTING_PROFID,
                        Q_BUSTING_MAX_CLUSTERS, (cId_t *)ClusterList,
                        Q_BUSTING_MAX_CLUSTERS, (cId_t *)ClusterList,
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
  if ( AF_DataRequest( &dstAddr, &epDesc, Q_BUSTING_CLUSTERID,
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

/*********************************************************************
 * @brief   Send basket receive from handhled to PC
 *
 * @param   MSGpkt  - Message packet received.
 * @return  none
 */
void SendBasketToPC(afIncomingMSGPacket_t *MSGpkt){
  
  osal_memcpy(basket_recv,MSGpkt->cmd.Data,BASKET_ID_LEN);
  osal_memcpy(basket_sent,(basket_id_sent+1),BASKET_ID_LEN);
  
  //if basket_id received is basket_id sent, then send it to pc 
  if(IsSameString(basket_recv,basket_sent,BASKET_ID_LEN)){
    
    uint8 len = MSGpkt->cmd.DataLength+3;
    uint8* data = osal_mem_alloc(sizeof(uint8)*len);
    //padding header
    *(data) = 'H';
    *(data+1) = BASKET_ID_LEN;
    *(data+2) = PRODS_ID_LEN;
    //contents of packet
    osal_memcpy((data+3),MSGpkt->cmd.Data, MSGpkt->cmd.DataLength);
    //send data to PC
    HalUARTWrite(UART_PC_PORT, data, len);
    //get address of handheld
    osal_mem_free(data);
  }
  //turn of the light to signal that cashier ready to process the next basket_id
  HalLedSet ( HAL_LED_4, HAL_LED_MODE_OFF ); 
}