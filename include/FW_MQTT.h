#ifndef FW_MQTT_H /* { */
#define FW_MQTT_H
/*===========================================================================*\
 * FILE: name: FW_MQTT.h
 *===========================================================================
 * Copyright 2021 IXGL Electronics., All Rights Reserved.
 * IXGL Electronics
 *---------------------------------------------------------------------------
 * version: 1
 * derived_by: Israel Gomez
 * date_modified: m/d/y 03/08/2021
 *----------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains the MQTT features for Control_Acuario_Rellenador program.
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
#include <MQTTClient.h>
#include <CircularBuffer.h>

/*============================================================================*\
 * Other Header Files
\*============================================================================*/
#include "FW_Wifi.h"

/*============================================================================*\
 * Exported Preprocessor #define Constants
\*============================================================================*/
#define CIRCULARBUFFERSIZE 5

/*============================================================================*\
 * Exported Preprocessor #define MACROS
\*============================================================================*/

/*============================================================================*\
 * Exported Type Declarations
\*============================================================================*/

/*============================================================================*\
 * Exported Object Declarations
\*============================================================================*/
//MQTT Topics
extern const char* MQTT_TOPIC;
//MQTT Pub Topics
extern const char* MQTT_PUBTOPIC_NIVHUMEDAD;
extern const char* MQTT_PUBTOPIC_INA3221VCS;
extern const char* MQTT_PUBTOPIC_INA3221ICS;
extern const char* MQTT_PUBTOPIC_INA3221VBL;
extern const char* MQTT_PUBTOPIC_INA3221IBL;
extern const char* MQTT_PUBTOPIC_INA3221VLD;
extern const char* MQTT_PUBTOPIC_INA3221ILD;
extern const char* MQTT_PUBTOPIC_ESLR_TEMP;
extern const char* MQTT_PUBTOPIC_ESLR_HUM;
extern const char* MQTT_PUBTOPIC_ESLR_LUX;
extern const char* MQTT_PUBTOPIC_REGANDO;
//MQTT Sub Topic
extern const char* MQTT_SUBTOPICGRAL;
extern const char* MQTT_SUBTOPIC;
//MQTT SSubs Topics
extern const char* MQTT_SSUBTOPIC_REGAR;
extern const char* MQTT_SSUBTOPIC_ACTDESFUNRGR;
//MQTT Pub state Topic
extern const char* MQTT_STATE;

extern const char HOST_MQTT_DEVICE_ID[];

extern const char MQTT_STATE_ON[];
extern const char MQTT_STATE_OFF[];

extern const char MQTT_IPSERVER[];
extern const char MQTT_PORT[];

extern bool MsjMQTTRcb;

/*============================================================================*\
 * Exported Function Prototypes
 * ----------------------------------------------------------------------------
 * The header information for each of the following function prototypes is 
 * located in the source file
\*============================================================================*/
extern void Fun_MessageReceived(String &topic, String &payload);
extern void Fun_Analizar_MsjRc(void);

/*============================================================================*\
 * Exported In-line Function Definitions (#define function-like macros)
\*============================================================================*/
#define MQTTPUBHUMEDAD(HUMEDAD_EN_PORCIENTO) clientmqttc.publish(String(MQTT_TOPIC) + String(MQTT_PUBTOPIC_NIVHUMEDAD), String(HUMEDAD_EN_PORCIENTO).c_str())
#define MQTTPUBREGAR_ST(RGR_ST) clientmqttc.publish(String(MQTT_TOPIC) + String(MQTT_SSUBTOPIC_REGAR) + String(MQTT_STATE), RGR_ST)
#define MQTTPUBREGANDO_ST(RDO_ST) clientmqttc.publish(String(MQTT_TOPIC) + String(MQTT_PUBTOPIC_REGANDO), RDO_ST)

/*============================================================================*\
 * Exported Preprocessor Declaration
\*============================================================================*/
extern CircularBuffer<String, CIRCULARBUFFERSIZE> Mqtt_PL_Rcd_Bfr;

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
#endif /* } FW_MQTT_H */