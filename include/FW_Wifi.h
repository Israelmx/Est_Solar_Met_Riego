#ifndef FW_WIFI_H /* { */
#define FW_WIFI_H
/*===========================================================================*\
 * FILE: name: FW_Wifi.h
 *===========================================================================
 * Copyright 2021 IXGL Electronics., All Rights Reserved.
 * IXGL Electronics
 *---------------------------------------------------------------------------
 * version: 1
 * derived_by: Israel Gomez
 * date_modified: 08/03/2021
 *----------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains the Wifi features for Control_Acuario program.
 *
 * ABBREVIATIONS:
 *   None.
 *
 * TRACEABILITY INFO:
 *   Design Document(s):
 *
 *   Requirements Document(s):
 *
 *   Applicable Standards (in order of precedence: highest first):
 *   C Coding Standards SW REF 264.15D SW
 *
 * DEVIATIONS FROM STANDARDS:
 *
\*===========================================================================*/

/*============================================================================*\
 * Standard Header Files
\*============================================================================*/
#include <ESP8266WiFi.h>

/*============================================================================*\
 * Other Header Files
\*============================================================================*/
#include "FW_MQTT.h"

/*============================================================================*\
 * Exported Preprocessor #define Constants
\*============================================================================*/

/*============================================================================*\
 * Exported Preprocessor #define MACROS
\*============================================================================*/

/*============================================================================*\
 * Exported Type Declarations
\*============================================================================*/
typedef enum Connection_Status_Tag
{
  WIFI_DOWN_MQTT_DOWN,
  WIFI_UP_MQTT_DOWN,
  WIFI_UP_MQTT_UP
}
Connection_Status_T;

/*============================================================================*\
 * Exported Object Declarations
\*============================================================================*/
extern Connection_Status_T Connection_Status;   // Connection status for WiFi and MQTT:

/*============================================================================*\
 * Exported Function Prototypes
 * ----------------------------------------------------------------------------
 * The header information for each of the following function prototypes is 
 * located in the source file
\*============================================================================*/
extern void Fun_WifiConnect(void);
extern void Fun_Checa_MQTT_Con(void);

/*============================================================================*\
 * Exported In-line Function Definitions (#define function-like macros)
\*============================================================================*/

/*============================================================================*\
 * Exported Preprocessor Declaration
\*============================================================================*/
extern WiFiClient clientnetwf;
extern MQTTClient clientmqttc;

/*============================================================================*\
 * File Revision History (top to bottom: first revision to last revision)
 *==============================================================================
 *
 * Date         userid    (Description on following lines: SCR #, etc.)
 * -----------  --------
 * 11-FEB-2021  Israel Gomez
 * - Created initial file.
 *
\*============================================================================*/
#endif /* } FW_WIFI_H */