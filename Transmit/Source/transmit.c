/**************************************************************************************************
  Filename:       transmit.c
  Revised:        $Date: 2009-03-18 15:56:27 -0700 (Wed, 18 Mar 2009) $
  Revision:       $Revision: 19453 $

  Description:    Generic Application (no Profile).


  Copyright 2004-2009 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
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
**************************************************************************************************/

/*********************************************************************
  Key control:
    SW1:
    SW2:
    SW3:
    SW4:
    SW5:  join or leave group
*********************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"
#include "AF.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "ZDProfile.h"

//for contact device in the group
#include "aps_groups.h"

#include "transmit.h"
#include "DebugTrace.h"

#if !defined( WIN32 )
  #include "OnBoard.h"
#endif

/* HAL */
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_uart.h"

/* USER CODE */
#include "buffer.h"

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
const cId_t transmit_ClusterList[transmit_MAX_CLUSTERS] =
{
  transmit_CLUSTERID
};

const SimpleDescriptionFormat_t transmit_SimpleDesc =
{
  transmit_ENDPOINT,              //  int Endpoint;
  transmit_PROFID,                //  uint16 AppProfId[2];
  transmit_DEVICEID,              //  uint16 AppDeviceId[2];
  transmit_DEVICE_VERSION,        //  int   AppDevVer:4;
  transmit_FLAGS,                 //  int   AppFlags:4;
  transmit_MAX_CLUSTERS,          //  byte  AppNumInClusters;
  (cId_t *)transmit_ClusterList,  //  byte *pAppInClusterList;
  transmit_MAX_CLUSTERS,          //  byte  AppNumInClusters;
  (cId_t *)transmit_ClusterList   //  byte *pAppInClusterList;
};

// This is the Endpoint/Interface description.  It is defined here, but
// filled-in in transmit_Init().  Another way to go would be to fill
// in the structure here and make it a "const" (in code space).  The
// way it's defined in this sample app it is define in RAM.
endPointDesc_t transmit_epDesc;

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
byte transmit_TaskID;   // Task ID for internal task/event processing
                          // This variable will be received when
                          // transmit_Init() is called.
devStates_t transmit_NwkState;

//declare a group for device
aps_Group_t transmit_Group;

byte transmit_TransID;  // This is the unique message ID (counter)

afAddrType_t transmit_DstAddr;
afAddrType_t transmit_grpAddr;

/*********************************************************************
 * LOCAL FUNCTIONS
 */
void transmit_ProcessZDOMsgs( zdoIncomingMsg_t *inMsg );
void transmit_HandleKeys( byte shift, byte keys );
void transmit_MessageMSGCB( afIncomingMSGPacket_t *pckt );
void transmit_SendTheMessage( char* content );

/*********************************************************************
 * NETWORK LAYER CALLBACKS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      transmit_Init
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
void transmit_Init( byte task_id )
{
  transmit_TaskID = task_id;
  transmit_NwkState = DEV_INIT;
  transmit_TransID = 0;

  // Device hardware initialization can be added here or in main() (Zmain.c).
  // If the hardware is application specific - add it here.
  // If the hardware is other parts of the device add it in main().

  transmit_DstAddr.addrMode = (afAddrMode_t)AddrBroadcast;
  transmit_DstAddr.endPoint = transmit_ENDPOINT;
  transmit_DstAddr.addr.shortAddr = 0xffff;
  
  // Setup for the destination address - Group 1
  transmit_grpAddr.addrMode = (afAddrMode_t)afAddrGroup;
  transmit_grpAddr.endPoint = transmit_ENDPOINT;
  transmit_grpAddr.addr.shortAddr = transmit_GROUP;

  // Fill out the endpoint description.
  transmit_epDesc.endPoint = transmit_ENDPOINT;
  transmit_epDesc.task_id = &transmit_TaskID;
  transmit_epDesc.simpleDesc
            = (SimpleDescriptionFormat_t *)&transmit_SimpleDesc;
  transmit_epDesc.latencyReq = noLatencyReqs;

  // Register the endpoint description with the AF
  afRegister( &transmit_epDesc );

  // Register for all key events - This app will handle all key events
  RegisterForKeys( transmit_TaskID );
  
  // By default, all devices start out in Group 1
  transmit_Group.ID = 0x0001;
  osal_memcpy( transmit_Group.name, "Group 1", 7  );
  aps_AddGroup( transmit_ENDPOINT, &transmit_Group );

  // Update the display
#if defined ( LCD_SUPPORTED )
    HalLcdWriteString( "transmit", HAL_LCD_LINE_1 );
#endif
    
  ZDO_RegisterForZDOMsg( transmit_TaskID, End_Device_Bind_rsp );
  ZDO_RegisterForZDOMsg( transmit_TaskID, Match_Desc_rsp );
}

/*********************************************************************
 * @fn      transmit_ProcessEvent
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
UINT16 transmit_ProcessEvent( byte task_id, UINT16 events )
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
    MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( transmit_TaskID );
    while ( MSGpkt )
    {
      switch ( MSGpkt->hdr.event )
      {
        case ZDO_CB_MSG:
          transmit_ProcessZDOMsgs( (zdoIncomingMsg_t *)MSGpkt );
          break;
          
        case KEY_CHANGE:
          transmit_HandleKeys( ((keyChange_t *)MSGpkt)->state, ((keyChange_t *)MSGpkt)->keys );
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
          }
          break;

        case AF_INCOMING_MSG_CMD:
          transmit_MessageMSGCB( MSGpkt );
          break;

        case ZDO_STATE_CHANGE:
          transmit_NwkState = (devStates_t)(MSGpkt->hdr.status);
          if ( (transmit_NwkState == DEV_ZB_COORD)
              || (transmit_NwkState == DEV_ROUTER)
              || (transmit_NwkState == DEV_END_DEVICE) )
          {
            // Start sending "the" message in a regular interval.
            osal_start_timerEx( transmit_TaskID,
                                transmit_SEND_MSG_EVT,
                              transmit_SEND_MSG_TIMEOUT );
          }
          break;

        default:
          break;
      }

      // Release the memory
      osal_msg_deallocate( (uint8 *)MSGpkt );

      // Next
      MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( transmit_TaskID );
    }

    // return unprocessed events
    return (events ^ SYS_EVENT_MSG);
  }

  // Send a message out - This event is generated by a timer
  //  (setup in transmit_Init()).
  if ( events & transmit_SEND_MSG_EVT )
  {
    // Send "the" message
    transmit_SendTheMessage(parseMessage(MSGpkt));

    // Setup to send message again
    osal_start_timerEx( transmit_TaskID,
                        transmit_SEND_MSG_EVT,
                      transmit_SEND_MSG_TIMEOUT );

    // return unprocessed events
    return (events ^ transmit_SEND_MSG_EVT);
  }

  // Discard unknown events
  return 0;
}

/*********************************************************************
 * Event Generation Functions
 */

/*********************************************************************
 * @fn      transmit_ProcessZDOMsgs()
 *
 * @brief   Process response messages
 *
 * @param   none
 *
 * @return  none
 */
void transmit_ProcessZDOMsgs( zdoIncomingMsg_t *inMsg )
{
  switch ( inMsg->clusterID )
  {
    case End_Device_Bind_rsp:
      if ( ZDO_ParseBindRsp( inMsg ) == ZSuccess )
      {
        // Light LED
        HalLedSet( HAL_LED_4, HAL_LED_MODE_ON );
      }
#if defined(BLINK_LEDS)
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
            transmit_DstAddr.addrMode = (afAddrMode_t)Addr16Bit;
            transmit_DstAddr.addr.shortAddr = pRsp->nwkAddr;
            // Take the first endpoint, Can be changed to search through endpoints
            transmit_DstAddr.endPoint = pRsp->epList[0];

            // Light LED
            HalLedSet( HAL_LED_4, HAL_LED_MODE_ON );
            //Send reponse message
            
          }
          osal_mem_free( pRsp );
        }
      }
      break;
  }
}

/*********************************************************************
 * @fn      transmit_HandleKeys
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
void transmit_HandleKeys( byte shift, byte keys )
{
  //zAddrType_t dstAddr;
  
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
      transmit_SendTheMessage("A Nguyen Van");
    }

    if ( keys & HAL_KEY_SW_2 )
    {
      transmit_SendTheMessage("B Nguyen Van");
    }

    if ( keys & HAL_KEY_SW_3 )
    {
      transmit_SendTheMessage("C Nguyen Van");
    }

    if ( keys & HAL_KEY_SW_4 )
    {
      transmit_SendTheMessage("D Nguyen Van");
    }
    
    if ( keys & HAL_KEY_SW_5 )
    {
      aps_Group_t *grp;
      grp = aps_FindGroup( transmit_ENDPOINT, transmit_GROUP );
      if ( grp )
      {
        // Remove from the group
        aps_RemoveGroup( transmit_ENDPOINT, transmit_GROUP );
      }
      else
      {
        // Add to the flash group
        aps_AddGroup( transmit_ENDPOINT, &transmit_Group );
      }
    }
    
  }
}

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * @fn      transmit_MessageMSGCB
 *
 * @brief   Data message processor callback.  This function processes
 *          any incoming data - probably from other devices.  So, based
 *          on cluster ID, perform the intended action.
 *
 * @param   none
 *
 * @return  none
 */
void transmit_MessageMSGCB( afIncomingMSGPacket_t *pkt )
{
  switch ( pkt->clusterId )
  {
    case transmit_CLUSTERID:
      printText((char*)pkt->cmd.Data,"");
      break;
  }
}

char* parseMessage( afIncomingMSGPacket_t *pkt ){
  char* ret = NULL;
  if(pkt->clusterId == transmit_CLUSTERID){
    char cmd = pkt->cmd.Data[0];
    switch(cmd){
    case 'A': ret = "Custom A";break;
    case 'B': ret = "Custom B";break;
    case 'C': ret = "Custom C";break;
    case 'D': ret = "Custom D";break;
    default:
      ret = "Unknow Custom";
      break;
    }
  }
  return ret;
}

/*********************************************************************
 * @fn      transmit_SendTheMessage
 *
 * @brief   Send "the" message.
 *
 * @param   none
 *
 * @return  none
 */
void transmit_SendTheMessage( char* content )
{
  char theMessageData[80];
  osal_memcpy(theMessageData,content,osal_strlen(content));

  if ( AF_DataRequest( &transmit_DstAddr, &transmit_epDesc,
                       transmit_CLUSTERID,
                       (byte)osal_strlen( theMessageData ) + 1,
                       (byte *)&theMessageData,
                       &transmit_TransID,
                       AF_DISCV_ROUTE, AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
  {
    // Successfully requested to be sent.
  }
  else
  {
    // Error occurred in request to send.
  }
}

//*********************************************************************
void printText(char* text1, char* text2)
{
#if defined( LCD_SUPPORTED )
  HalLcdWriteScreen(text1, text2);
#elif defined( WIN32 )
  WPRINTSTR(text1);
#endif
}

char* int2char(int num){
  unsigned char i=0,j=0;
  int tmp = num;
  while(1){
    tmp = tmp/10;
    i++;
    if(tmp<=0) break;
  }
  char* ret = (char*)osal_mem_alloc(sizeof(char)*i);
  for(j=i;j>0;j--){
    *(ret+j) = num%10 + 48;
    num=num/10;
  }
  *(ret+j) = '\0';
  return ret;
}
//*********************************************************************
