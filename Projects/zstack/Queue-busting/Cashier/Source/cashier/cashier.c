/*********************************************************************
 * When check out a basket, HAL_LED_4 is on. HAL_LED_4 is off when done
 * When add/del product, HAL_LED_3 is on. HAL_LED_3 is off when done
 * When del basket, HAL_LED_2 is on. HAL_LED_2 is off when done, blink when error
 * When check status, HAL_LED_1 is on. HAL_LED_1 is off when done
 **********************************************************************/



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

uint8 add_remove_mode;    //add or remove products
uint8* basket_id_sent;
uint8* pc_cmd_buff;
uint8 basket_recv[BASKET_ID_LEN];
uint8 basket_sent[BASKET_ID_LEN];

Basket_Del BDel[MAX_BASKET_STORE];
uint8 basket_del_index;
byte basket_del_trans;

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
 * @brief   Get source address of basket_id
 *
 * @param   basket_id
 *
 * @return  short address
 */
afAddrType_t Get_src_addr(uint8* id){
  uint8 i;
  if(basket_del_index){
    for(i=0;i<MAX_BASKET_STORE;i++){
      if(IsSameString(id,BDel[i].id,BASKET_ID_LEN))
        return BDel[i].addr;
    }
  }
  return BrdAddr;
}

/*********************************************************************
 * @brief   check if basket is sent or not
 *
 * @param   basket_id
 *
 * @return  true if already have
 */
bool have_basket(uint8* id){
  uint8 i;
  if(basket_del_index){
    for(i=0;i<MAX_BASKET_STORE;i++){
      if(IsSameString(id,BDel[i].id,BASKET_ID_LEN))
        return true;
    }
  }
  return false;
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
  basket_del_index = 0;
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
  uint8 bsk_not_found[BASKET_ID_LEN+2];
  uint8 bsk_cant_del[BASKET_ID_LEN+2];
  uint8 bsk_del_buf[BASKET_ID_LEN];

  switch(events){
    case TIMER_EVENT:
      //'E'+[Error ID]+[Basket ID]      
      bsk_not_found[0]='E';
      bsk_not_found[1]=2;
      osal_memcpy(bsk_not_found+2,basket_id_sent+1,BASKET_ID_LEN);
      HalUARTWrite(UART_PC_PORT, bsk_not_found, BASKET_ID_LEN+2);
      HalLedSet ( HAL_LED_4, HAL_LED_MODE_OFF );
      break;
    case UART_SCANNER_EVENT:

      basket_buff = bufPop(basket_buff, basket_id_sent, &len);
      
      //Process for data_in is add/remove products      
      if(len==2){
        add_remove_mode = Check_utilize_products((char*)basket_id_sent);
      }
      if(add_remove_mode){
        HalLedSet ( HAL_LED_3, HAL_LED_MODE_ON );
        if(len!=(PRODS_ID_LEN+1)) break;
        uint8 pid[PRODS_ID_LEN+2];
        if(add_remove_mode == 1) pid[0] = ADD_PRODS_CODE;
        if(add_remove_mode == 2) pid[0] = DEL_PRODS_CODE;
        osal_memcpy(pid+1,basket_id_sent,PRODS_ID_LEN);
        pid[PRODS_ID_LEN+1] = 0x01 ;
        HalUARTWrite(UART_PC_PORT, pid, PRODS_ID_LEN+2);
        HalLedSet ( HAL_LED_3, HAL_LED_MODE_OFF );
        break;
      }
      
      //Process for data_in is basket
      if(Check_basket_id_format((char*)basket_id_sent)){
        *(basket_id_sent+BASKET_ID_LEN+1) = 0x0d;        
        if(have_basket(basket_id_sent)) break;
        i = len;
        while(i){
          *(basket_id_sent+i) = *(basket_id_sent+i-1);
          i=i-1;
        }
        *(basket_id_sent) = REQUEST_BASKET;
        HalLedSet ( HAL_LED_4, HAL_LED_MODE_ON );
        SendMessage(BrdAddr, (char*)basket_id_sent, BASKET_ID_LEN+1);
        //Start Timer
        osal_start_timerEx(Cashier_TaskID, TIMER_EVENT, TIMER_TIME_OUT);
      }
      
      //Delete all baket in all handhled
      if((*basket_id_sent) == RESET_FLASH){
        uint8* reset = "FlashReset";
        uint8 rs_len = osal_strlen((char*)reset);
        if(IsSameString((basket_id_sent+1),reset,rs_len)){
          //HalLedSet ( HAL_LED_2, HAL_LED_MODE_ON );
          SendMessage(BrdAddr, (char*)basket_id_sent, rs_len+1);        
        }
      }
      
      //Get status of all device
      if((*basket_id_sent) == GET_STAT){
        uint8* status = "tatus";
        uint8 stat_len = osal_strlen((char*)status);
        if(IsSameString((basket_id_sent+1),status,stat_len)){
          HalLedSet ( HAL_LED_1, HAL_LED_MODE_ON );
          SendMessage(BrdAddr, "S", 1);        
        }
      }

      break;
    case UART_PC_EVENT:
      //Get command from pc to buffer
      pc_buff = bufPop(pc_buff, pc_cmd_buff, &len);
      //HalUARTWrite(UART_PC_PORT, pc_cmd_buff, len);

      //Delete basket
      //'^'+[num of Baskets]+[Basket ID]
      if(*(pc_cmd_buff)== DEL_BASKET){
        uint8 idx = (*(pc_cmd_buff+1))-48,i;
        (*(pc_cmd_buff+1)) = idx;
        if(idx == 0){
          SendMessage(BrdAddr,(char*)pc_cmd_buff,2);
          break;
        }
        
        for(i=0;i<idx;i++){
          osal_memcpy(bsk_del_buf,(pc_cmd_buff+2),BASKET_ID_LEN);
          afAddrType_t addr = Get_src_addr(bsk_del_buf);
          HalLedSet ( HAL_LED_2, HAL_LED_MODE_ON );
          SendMessage(addr,(char*)pc_cmd_buff, BASKET_ID_LEN+2);
          basket_del_trans = Cashier_TransID-1;
        }
      }
      //Get status of the device
      if(*(pc_cmd_buff)== GET_STAT){
        HalLedSet ( HAL_LED_1, HAL_LED_MODE_ON );
        SendMessage(BrdAddr,(char*)pc_cmd_buff, 1);
      }

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
          HalLedSet ( HAL_LED_ALL, HAL_LED_MODE_OFF );
          if(sentTransID == basket_del_trans && basket_del_trans!=0)
            basket_del_index--;
          /*if(sentTransID == basket_del_trans && basket_del_trans!=0){
            HalUARTWrite(UART_PC_PORT, "Basket Deleted",osal_strlen("Basket deleted"));
          }*/
          if(sentStatus != ZSuccess && sentTransID == basket_del_trans && basket_del_trans!=0){
            // The data wasn't delivered -- Do something
            bsk_cant_del[0]='E';
            bsk_cant_del[1]=1;
            osal_memcpy(bsk_cant_del+2,pc_cmd_buff+2,BASKET_ID_LEN);
            HalUARTWrite(UART_PC_PORT, bsk_cant_del, BASKET_ID_LEN+2);
            HalLedBlink ( HAL_LED_2, 0, 50, 500 );
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
    }

    if ( keys & HAL_KEY_SW_3 ){
    }

    if ( keys & HAL_KEY_SW_4 ){
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
void SendMessage(afAddrType_t dstAddr, char* message, int len){
  
  char theMessageData[80];
  osal_memcpy(theMessageData,message, len);
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
  uint8 plen,blen;
  
  char ch = (*MSGpkt->cmd.Data);
  plen = MSGpkt->cmd.DataLength;
  switch (ch){    
  case 'S':
    HalUARTWrite(UART_PC_PORT, MSGpkt->cmd.Data, plen);
    HalLedSet ( HAL_LED_1, HAL_LED_MODE_OFF );
    break;
  case 'H':
    blen = *(MSGpkt->cmd.Data+7);
    
    if(blen==BASKET_ID_LEN){
      osal_memcpy(basket_recv,(MSGpkt->cmd.Data)+9,blen);
      osal_memcpy(basket_sent,(basket_id_sent+1),BASKET_ID_LEN);
      
      //if basket_id received is basket_id sent, then send it to pc 
      if(IsSameString(basket_recv,basket_sent,BASKET_ID_LEN)){
        //store basket and handheld address
        osal_memcpy(BDel[basket_del_index].id,basket_recv,BASKET_ID_LEN);
        BDel[basket_del_index].addr = MSGpkt->srcAddr;
        basket_del_index++;
        if(basket_del_index>MAX_BASKET_STORE){
          basket_del_index = 0;
        }
        //send data to PC
        HalUARTWrite(UART_PC_PORT, MSGpkt->cmd.Data, plen);
        osal_stop_timerEx(Cashier_TaskID, TIMER_EVENT);
      }
      
      //turn of the light to signal that cashier ready to process the next basket_id
      HalLedSet ( HAL_LED_4, HAL_LED_MODE_OFF ); 
    }
    break;
  default:
    break;
  }
}