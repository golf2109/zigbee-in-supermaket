/******************************************************************************
  Filename:       HandheldApp.c
  Revised:        $Date: 2010-12-21 10:27:34 -0800 (Tue, 21 Dec 2010) $
  Revision:       $Revision: 24670 $

  Description:    Generic Application (no Profile).


  Copyright 2004-2010 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product. Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
******************************************************************************/

/*********************************************************************
  This application isn't intended to do anything useful, it is
  intended to be a simple example of an application's structure.

  This application sends "Hello World" to another "Generic"
  application every 15 seconds.  The application will also
  receive "Hello World" packets.

  The "Hello World" messages are sent/received as MSG type message.

  This applications doesn't have a profile, so it handles everything
  directly - itself.

  Key control:
    SW1:
    SW2:  initiates end device binding
    SW3:
    SW4:  initiates a match description request
*********************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"
#include "AF.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "ZDProfile.h"

#define BUF_SIZE  64
#include "buffer.h"

#include "Handheld.h"
#include "DebugTrace.h"

#if !defined( WIN32 )
  #include "OnBoard.h"
#endif

/* HAL */
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_uart.h"

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
byte HandheldApp_TaskID;   // Task ID for internal task/event processing
                          // This variable will be received when
                          // HandheldApp_Init() is called.
ringBuf_t ScannerBuf;
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

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

devStates_t HandheldApp_NwkState;
uint8   sentBasketID[BASKET_ID_LEN];

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
  //---
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
          HandheldApp_ProcessZDOMsgs( (zdoIncomingMsg_t *)MSGpkt );
          break;

        case KEY_CHANGE:
          HandheldApp_HandleKeys( ((keyChange_t *)MSGpkt)->state, ((keyChange_t *)MSGpkt)->keys );
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
            //========Erase Basket========
            EraseBasket(sentBasketID);
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
            /*osal_start_timerEx( HandheldApp_TaskID,
                                HANDHELDAPP_SEND_MSG_EVT,
                                HANDHELDAPP_SEND_MSG_TIMEOUT );
            */
          }
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
  //=====================Process a data from barcode scanner
  //- This event is generated by UART
  //callback of DMA
  if ( events & HANDHELDAPP_SCANNER_EVT )
  {
    //Process input
    ScannerHandleInput(&ScannerBuf);
    // return unprocessed events
    return (events ^ HANDHELDAPP_SCANNER_EVT);
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
  ;
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
      pBasket =ReadBasket(pkt->cmd.Data);
      if(pBasket)
      {
        if ( AF_DataRequest( &pkt->srcAddr, &HandheldApp_epDesc,
                       HANDHELDAPP_CLUSTERID,
                       (pBasket->len)*(pBasket->offset)+2+BASKET_ID_LEN,
                       (byte *)pBasket,
                       &HandheldApp_TransID,
                       AF_DISCV_ROUTE, AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
        {
          // Successfully requested to be sent.
        }
        else
        {
          // Error occurred in request to send.
        }
        CopyString(sentBasketID, (uint8*)pBasket->basket_id,BASKET_ID_LEN);
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
 ;
}

/*********************************************************************
*********************************************************************/
