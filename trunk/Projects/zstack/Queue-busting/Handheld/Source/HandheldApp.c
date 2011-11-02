/******************************************************************************
  Filename:       HandheldApp.c
  Revised:        $Date: 2010-12-21 10:27:34 -0800 (Tue, 21 Dec 2010) $
  Revision:       $Revision: 24670 $

  Description:    Handheld Application (no Profile).

******************************************************************************/


/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"
#include "AF.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "ZDProfile.h"

#include "HandheldApp.h"
#include "DebugTrace.h"

#if !defined( WIN32 )
  #include "OnBoard.h"
#endif

/* HAL */
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_uart.h"

#include "buffer.h"
#include "Serialize.h"
#include "dataflash.h"
#include "BC_Scanner.h"
#include "xdata_handle.h"
#include "scanner_handle.h"
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

// This list should be filled with Application specific Cluster IDs.
const cId_t HandheldApp_ClusterList[HANDHELDAPP_MAX_CLUSTERS] =
{
  HANDHELDAPP_CLUSTERID
};

const SimpleDescriptionFormat_t HandheldApp_SimpleDesc =
{
  HANDHELDAPP_ENDPOINT,              //  int Endpoint;
  HANDHELDAPP_PROFID,                //  uint16 AppProfId[2];
  HANDHELDAPP_DEVICEID,              //  uint16 AppDeviceId[2];
  HANDHELDAPP_DEVICE_VERSION,        //  int   AppDevVer:4;
  HANDHELDAPP_FLAGS,                 //  int   AppFlags:4;
  HANDHELDAPP_MAX_CLUSTERS,          //  byte  AppNumInClusters;
  (cId_t *)HandheldApp_ClusterList,  //  byte *pAppInClusterList;
  HANDHELDAPP_MAX_CLUSTERS,          //  byte  AppNumInClusters;
  (cId_t *)HandheldApp_ClusterList   //  byte *pAppInClusterList;
};

// This is the Endpoint/Interface description.  It is defined here, but
// filled-in in HandheldApp_Init().  Another way to go would be to fill
// in the structure here and make it a "const" (in code space).  The
// way it's defined in this sample app it is define in RAM.
endPointDesc_t HandheldApp_epDesc;
//
ringBuf_t ScannerBuf;
uint8 sentBasketID[BASKET_ID_LEN]="";
/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
byte HandheldApp_TaskID;   // Task ID for internal task/event processing
                          // This variable will be received when
                          // HandheldApp_Init() is called.
devStates_t HandheldApp_NwkState;


byte HandheldApp_TransID;  // This is the unique message ID (counter)

afAddrType_t HandheldApp_DstAddr;

/*********************************************************************
 * LOCAL FUNCTIONS
 */
void HandheldApp_ProcessZDOMsgs( zdoIncomingMsg_t *inMsg );
void HandheldApp_HandleKeys( byte shift, byte keys );
void HandheldApp_MessageMSGCB( afIncomingMSGPacket_t *pckt );
void HandheldApp_SendTheMessage( void );

/*********************************************************************
 * NETWORK LAYER CALLBACKS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      HandheldApp_Init
 *
 * @brief   Initialization function for the Generic App Task.
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
void HandheldApp_Init( byte task_id )
{
  /*Buffer*/
  bufInit(&ScannerBuf);
  /*SPI*/
  DF_spiInit(NULL);
  /*Scanner*/
  ScannerHandleInit();
  BC_ScannerInit();
  
  HandheldApp_TaskID = task_id;
  HandheldApp_NwkState = DEV_INIT;
  HandheldApp_TransID = 0;

  // Device hardware initialization can be added here or in main() (Zmain.c).
  // If the hardware is application specific - add it here.
  // If the hardware is other parts of the device add it in main().

  HandheldApp_DstAddr.addrMode = (afAddrMode_t)AddrNotPresent;
  HandheldApp_DstAddr.endPoint = 0;
  HandheldApp_DstAddr.addr.shortAddr = 0;

  // Fill out the endpoint description.
  HandheldApp_epDesc.endPoint = HANDHELDAPP_ENDPOINT;
  HandheldApp_epDesc.task_id = &HandheldApp_TaskID;
  HandheldApp_epDesc.simpleDesc
            = (SimpleDescriptionFormat_t *)&HandheldApp_SimpleDesc;
  HandheldApp_epDesc.latencyReq = noLatencyReqs;

  // Register the endpoint description with the AF
  afRegister( &HandheldApp_epDesc );

  // Register for all key events - This app will handle all key events
  RegisterForKeys( HandheldApp_TaskID );

  // Update the display
#if defined ( LCD_SUPPORTED )
    HalLcdWriteString( "HandheldApp", HAL_LCD_LINE_1 );
#endif

  ZDO_RegisterForZDOMsg( HandheldApp_TaskID, End_Device_Bind_rsp );
  ZDO_RegisterForZDOMsg( HandheldApp_TaskID, Match_Desc_rsp );
}

/*********************************************************************
 * @fn      HandheldApp_ProcessEvent
 *
 * @brief   Generic Application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id  - The OSAL assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  none
 */
UINT16 HandheldApp_ProcessEvent( byte task_id, UINT16 events )
{
  afIncomingMSGPacket_t *MSGpkt;
  afDataConfirm_t *afDataConfirm;

  // Data Confirmation message fields
  byte sentEP;
  ZStatus_t sentStatus;
  byte sentTransID;       // This should match the value sent
  (void)task_id;  // Intentionally unreferenced parameter

  if ( events & SYS_EVENT_MSG )
  {
    MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( HandheldApp_TaskID );
    while ( MSGpkt )
    {
      switch ( MSGpkt->hdr.event )
      {
        case ZDO_CB_MSG:
          //HandheldApp_ProcessZDOMsgs( (zdoIncomingMsg_t *)MSGpkt );
          break;

        case KEY_CHANGE:
          //HandheldApp_HandleKeys( ((keyChange_t *)MSGpkt)->state, ((keyChange_t *)MSGpkt)->keys );
          break;

        case AF_DATA_CONFIRM_CMD:
          // This message is received as a confirmation of a data packet sent.
          // The status is of ZStatus_t type [defined in ZComDef.h]
          // The message fields are defined in AF.h
          afDataConfirm = (afDataConfirm_t *)MSGpkt;
          sentEP = afDataConfirm->endpoint;
          sentStatus = afDataConfirm->hdr.status;
          sentTransID = afDataConfirm->transID;
          (void)sentEP;
          (void)sentTransID;

          // Action taken when confirmation is received.
          if ( sentStatus != ZSuccess )
          {
            // The data wasn't delivered -- Do something
          }else{
            ;
          }
          break;

        case AF_INCOMING_MSG_CMD:
          HandheldApp_MessageMSGCB( MSGpkt );
          break;

        case ZDO_STATE_CHANGE:
          HandheldApp_NwkState = (devStates_t)(MSGpkt->hdr.status);
          if ( (HandheldApp_NwkState == DEV_ZB_COORD)
              || (HandheldApp_NwkState == DEV_ROUTER)
              || (HandheldApp_NwkState == DEV_END_DEVICE) )
          {
            // Start sending "the" message in a regular interval.
            //osal_start_timerEx( HandheldApp_TaskID,
            //                    HANDHELDAPP_SEND_MSG_EVT,
             //                   HANDHELDAPP_SEND_MSG_TIMEOUT );
          }else if(HandheldApp_NwkState== DEV_NWK_ORPHAN)
             printText("Disconnect",11);
          break;

        default:
          break;
      }

      // Release the memory
      osal_msg_deallocate( (uint8 *)MSGpkt );

      // Next
      MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( HandheldApp_TaskID );
    }

    // return unprocessed events
    return (events ^ SYS_EVENT_MSG);
  }

  // Send a message out - This event is generated by a timer
  //  (setup in HandheldApp_Init()).
  if ( events & HANDHELDAPP_SEND_MSG_EVT )
  {
    ScannerHandleInput(&ScannerBuf);
    // return unprocessed events
    return (events ^ HANDHELDAPP_SEND_MSG_EVT);
  }

  // Discard unknown events
  return 0;
}

/*********************************************************************
 * Event Generation Functions
 */

/*********************************************************************
 * @fn      HandheldApp_ProcessZDOMsgs()
 *
 * @brief   Process response messages
 *
 * @param   none
 *
 * @return  none
 */
void HandheldApp_ProcessZDOMsgs( zdoIncomingMsg_t *inMsg )
{
  switch ( inMsg->clusterID )
  {
    case End_Device_Bind_rsp:
      if ( ZDO_ParseBindRsp( inMsg ) == ZSuccess )
      {
        // Light LED
        HalLedSet( HAL_LED_4, HAL_LED_MODE_ON );
      }
#if defined( BLINK_LEDS )
      else
      {
        // Flash LED to show failure
        HalLedSet ( HAL_LED_4, HAL_LED_MODE_FLASH );
      }
#endif
      break;

    case Match_Desc_rsp:
      {
        ZDO_ActiveEndpointRsp_t *pRsp = ZDO_ParseEPListRsp( inMsg );
        if ( pRsp )
        {
          if ( pRsp->status == ZSuccess && pRsp->cnt )
          {
            HandheldApp_DstAddr.addrMode = (afAddrMode_t)Addr16Bit;
            HandheldApp_DstAddr.addr.shortAddr = pRsp->nwkAddr;
            // Take the first endpoint, Can be changed to search through endpoints
            HandheldApp_DstAddr.endPoint = pRsp->epList[0];

            // Light LED
            HalLedSet( HAL_LED_4, HAL_LED_MODE_ON );
          }
          osal_mem_free( pRsp );
        }
      }
      break;
  }
}

/*********************************************************************
 * @fn      HandheldApp_HandleKeys
 *
 * @brief   Handles all key events for this device.
 *
 * @param   shift - true if in shift/alt.
 * @param   keys - bit field for key events. Valid entries:
 *                 HAL_KEY_SW_4
 *                 HAL_KEY_SW_3
 *                 HAL_KEY_SW_2
 *                 HAL_KEY_SW_1
 *
 * @return  none
 */
void HandheldApp_HandleKeys( byte shift, byte keys )
{
  zAddrType_t dstAddr;

  // Shift is used to make each button/switch dual purpose.
  if ( shift )
  {
    if ( keys & HAL_KEY_SW_1 )
    {
    }
    if ( keys & HAL_KEY_SW_2 )
    {
    }
    if ( keys & HAL_KEY_SW_3 )
    {
    }
    if ( keys & HAL_KEY_SW_4 )
    {
    }
  }
  else
  {
    if ( keys & HAL_KEY_SW_1 )
    {
      // Since SW1 isn't used for anything else in this application...
#if defined( SWITCH1_BIND )
      // we can use SW1 to simulate SW2 for devices that only have one switch,
      keys |= HAL_KEY_SW_2;
#elif defined( SWITCH1_MATCH )
      // or use SW1 to simulate SW4 for devices that only have one switch
      keys |= HAL_KEY_SW_4;
#endif
    }

    if ( keys & HAL_KEY_SW_2 )
    {
      HalLedSet ( HAL_LED_4, HAL_LED_MODE_OFF );

      // Initiate an End Device Bind Request for the mandatory endpoint
      dstAddr.addrMode = Addr16Bit;
      dstAddr.addr.shortAddr = 0x0000; // Coordinator
      ZDP_EndDeviceBindReq( &dstAddr, NLME_GetShortAddr(),
                            HandheldApp_epDesc.endPoint,
                            HANDHELDAPP_PROFID,
                            HANDHELDAPP_MAX_CLUSTERS, (cId_t *)HandheldApp_ClusterList,
                            HANDHELDAPP_MAX_CLUSTERS, (cId_t *)HandheldApp_ClusterList,
                            FALSE );
    }

    if ( keys & HAL_KEY_SW_3 )
    {
    }

    if ( keys & HAL_KEY_SW_4 )
    {
      HalLedSet ( HAL_LED_4, HAL_LED_MODE_OFF );
      // Initiate a Match Description Request (Service Discovery)
      dstAddr.addrMode = AddrBroadcast;
      dstAddr.addr.shortAddr = NWK_BROADCAST_SHORTADDR;
      ZDP_MatchDescReq( &dstAddr, NWK_BROADCAST_SHORTADDR,
                        HANDHELDAPP_PROFID,
                        HANDHELDAPP_MAX_CLUSTERS, (cId_t *)HandheldApp_ClusterList,
                        HANDHELDAPP_MAX_CLUSTERS, (cId_t *)HandheldApp_ClusterList,
                        FALSE );
    }
  }
}

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * @fn      HandheldApp_MessageMSGCB
 *
 * @brief   Data message processor callback.  This function processes
 *          any incoming data - probably from other devices.  So, based
 *          on cluster ID, perform the intended action.
 *
 * @param   none
 *
 * @return  none
 */
void HandheldApp_MessageMSGCB( afIncomingMSGPacket_t *pkt )
{
 Basket *pBasket;
  switch ( pkt->clusterId )
  {
    case HANDHELDAPP_CLUSTERID:
      if(pkt->cmd.Data[0]== REQUEST_BASKET)
      {
        pBasket =ReadBasket(pkt->cmd.Data+1);
        if(pBasket)
        {
          if ( AF_DataRequest( &pkt->srcAddr, &HandheldApp_epDesc,
                         HANDHELDAPP_CLUSTERID,BASKET_ID_LEN+PRODS_NUM_SIZE+
                         (pBasket->len)*(sizeof(Product)),
                         (byte *)pBasket,
                         &HandheldApp_TransID,
                         AF_DISCV_ROUTE|AF_ACK_REQUEST, AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
          {
            // Successfully requested to be sent.
          }
          else
          {
            // Error occurred in request to send.
          }
        }
      }else if(pkt->cmd.Data[0]== DEL_BASKET)
      {
        if(IsSameString(pkt->cmd.Data+1,ALL_BASKET,ALL_BASKET_SIZE))
          //Erase all basket
          FlashReset();
        else//Erase the Basket
          EraseBasket(pkt->cmd.Data+1);
      }
      // "the" message
#if defined( LCD_SUPPORTED )
      HalLcdWriteScreen( (char*)pkt->cmd.Data, "rcvd" );
#elif defined( WIN32 )
      WPRINTSTR( pkt->cmd.Data );
#endif
      break;
  }
}

/*********************************************************************
 * @fn      HandheldApp_SendTheMessage
 *
 * @brief   Send "the" message.
 *
 * @param   none
 *
 * @return  none
 */
void HandheldApp_SendTheMessage( void )
{
  char theMessageData[] = "Hello World";

  if ( AF_DataRequest( &HandheldApp_DstAddr, &HandheldApp_epDesc,
                       HANDHELDAPP_CLUSTERID,
                       (byte)osal_strlen( theMessageData ) + 1,
                       (byte *)&theMessageData,
                       &HandheldApp_TransID,
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
*********************************************************************/
