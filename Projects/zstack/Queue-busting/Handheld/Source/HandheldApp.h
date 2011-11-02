/**************************************************************************************************
  Filename:       HANDHELDAPP.h
  Revised:        $Date: 2007-10-27 17:22:23 -0700 (Sat, 27 Oct 2007) $
  Revision:       $Revision: 15795 $

  Description:    This file contains the Handheld Application definitions.
**************************************************************************************************/

#ifndef HANDHELDAPP_H
#define HANDHELDAPP_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "ZComDef.h"

/*********************************************************************
 * CONSTANTS
 */

// These constants are only for example and should be changed to the
// device's needs
#define HANDHELDAPP_ENDPOINT           Q_BUSTING_ENDPOINT

#define HANDHELDAPP_PROFID             Q_BUSTING_PROFID
#define HANDHELDAPP_DEVICEID           Q_BUSTING_DEVICEID
#define HANDHELDAPP_DEVICE_VERSION     Q_BUSTING_DEVICE_VERSION
#define HANDHELDAPP_FLAGS              Q_BUSTING_FLAGS

#define HANDHELDAPP_MAX_CLUSTERS       Q_BUSTING_MAX_CLUSTERS
#define HANDHELDAPP_CLUSTERID          Q_BUSTING_CLUSTERID

// Send Message Timeout
#define HANDHELDAPP_SEND_MSG_TIMEOUT   Q_BUSTING_TIMEOUT     // Every 5 seconds

// Application Events (OSAL) - These are bit weighted definitions.
#define HANDHELDAPP_SEND_MSG_EVT       0x0001

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */

/*
 * Task Initialization for the Generic Application
 */
extern void HandheldApp_Init( byte task_id );

/*
 * Task Event Processor for the Generic Application
 */
extern UINT16 HandheldApp_ProcessEvent( byte task_id, UINT16 events );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* HANDHELDAPP_H */
