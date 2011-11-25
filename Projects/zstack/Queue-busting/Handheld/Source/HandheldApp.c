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

void HandheldApp_MessageMSGCB( afIncomingMSGPacket_t *pckt );


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
            HalLedSet( HAL_LED_1, HAL_LED_MODE_ON );
          }else{
            ;
          }
          break;

        case AF_INCOMING_MSG_CMD:
          HandheldApp_MessageMSGCB( MSGpkt );
          break;

        case ZDO_STATE_CHANGE:
          HandheldApp_NwkState = (devStates_t)(MSGpkt->hdr.status);
          /**Device joined network*/
          if ( (HandheldApp_NwkState == DEV_ZB_COORD)
              || (HandheldApp_NwkState == DEV_ROUTER)
              || (HandheldApp_NwkState == DEV_END_DEVICE) )
          {
            //! Turn off the network status LED
            HalLedSet( HAL_LED_2, HAL_LED_MODE_OFF );
          }
          /**Device is discovering network*/
          else if(HandheldApp_NwkState == DEV_NWK_DISC)
          {
            //! Toggle the network status LED
            HalLedSet( HAL_LED_2,HAL_LED_MODE_TOGGLE);
          }
          /**Device isn't joined*/
          else
          {
            //! Turn on the network status LED
            HalLedSet( HAL_LED_2, HAL_LED_MODE_ON );
          }
          break;

        default:
          break;
      }

      //! Release the memory
      osal_msg_deallocate( (uint8 *)MSGpkt );

      //! Next
      MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( HandheldApp_TaskID );
    }

    /** return unprocessed events*/
    return (events ^ SYS_EVENT_MSG);
  }
  /** Send a message out - This event is generated by a timer
  (setup in HandheldApp_Init()).*/
  if ( events & HANDHELDAPP_SEND_MSG_EVT )
  {
    ScannerHandleInput(&ScannerBuf);
    /** return unprocessed events*/
    return (events ^ HANDHELDAPP_SEND_MSG_EVT);
  }

  /** Discard unknown events*/
  return 0;
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
 byte *pBasketFrame;
 uint16 len,uiTmp;
 uint8 status_resp[STATUS_RESP_LEN];
 uint8 i;
  switch ( pkt->clusterId )
  {
    case HANDHELDAPP_CLUSTERID:
      /**Receiving the Basket request*/
      if(pkt->cmd.Data[0]== REQUEST_BASKET)
      {
        /**Read the Basket from external flash*/
        pBasketFrame =ReadBasket(pkt->cmd.Data+1,&len);
        if(pBasketFrame)
        {
          /**Add prefix to the Basket Frame*/
          pBasketFrame[0]=REPONSE_BASKET;
          uiTmp=NLME_GetShortAddr();
          pBasketFrame[1]=(uint8)((uiTmp>>8)&0xff);
          pBasketFrame[2]=(uint8)(uiTmp&0xff);
          pBasketFrame[3]=BASKET_ID_LEN;
          pBasketFrame[4]=PRODS_ID_LEN;
          /**Send the Basket Frame*/
          if ( AF_DataRequest( &pkt->srcAddr, &HandheldApp_epDesc,
                         HANDHELDAPP_CLUSTERID,len,
                         (byte *)pBasketFrame,
                         &HandheldApp_TransID,
                         AF_DISCV_ROUTE|AF_ACK_REQUEST, AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
          {
            /** Successfully requested to be sent.*/
          }
          else
          {
            /** Error occurred in request to send.*/
          }
        }
      }/**Receiving the deleting Basket request*/
      else if(pkt->cmd.Data[0]== DEL_BASKET)
      {
        if(pkt->cmd.DataLength == 1)
          break;
        /**Erase all basket*/
        if(pkt->cmd.Data[1]==0)
        {
          FlashReset();
        }
        /**Erase the Baskets*/
        else
        {
          if(pkt->cmd.DataLength < (pkt->cmd.Data[1]*BASKET_ID_LEN+2))
            break;
          i=0;
          while(i<pkt->cmd.Data[1])
          {
            EraseBasket(pkt->cmd.Data+2+i*(BASKET_ID_LEN));
            i++;
          }
        }
      }
      /**Receiving the status request*/
      else if(pkt->cmd.Data[0]== STATUS_REQ && pkt->cmd.DataLength == 1)
      {
        status_resp[0]=STATUS_REQ;
        //! MAC addr
        CopyString(&status_resp[1],(uint8*)NLME_GetExtAddr(),6);
        //! Short Addr (Network Addr)
        uiTmp=NLME_GetShortAddr();
        status_resp[7]=(uint8)((uiTmp>>8)&0xff);
        status_resp[8]=(uint8)((uiTmp)&0xff);
        //! Short Addr of its parent
        uiTmp=NLME_GetCoordShortAddr();
        status_resp[9]=(uint8)((uiTmp>>8)&0xff);
        status_resp[10]=(uint8)((uiTmp)&0xff);
        /**Send the status*/
        if ( AF_DataRequest( &pkt->srcAddr, &HandheldApp_epDesc,
                         HANDHELDAPP_CLUSTERID,STATUS_RESP_LEN,
                         (byte *)status_resp,
                         &HandheldApp_TransID,
                         AF_DISCV_ROUTE|AF_ACK_REQUEST, AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
          {
            //! Successfully requested to be sent.
          }
          else
          {
            //! Error occurred in request to send.
          }
      }
      break;
  }
}

/*********************************************************************
*********************************************************************/
