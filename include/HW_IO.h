#ifndef HW_IO_H /* { */
#define HW_IO_H
/*===========================================================================*\
 * FILE: name: Sonda_PH.h
 *===========================================================================
 * Copyright 2021 IXGL Electronics., All Rights Reserved.
 * IXGL Electronics
 *---------------------------------------------------------------------------
 * version: 1
 * derived_by: Israel Gomez
 * date_modified: 11/02/2021
 *----------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   Archico que contiene los headers para las I/Os del acuario
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
 * Hardware Pins Declaration
\*============================================================================*/
// #define GPO 16   // Este pin no se puede usar si usas DEEP SLEEP
#define GPOBOMBA 12 //Diver Bomba
#define GPIBTNGR 13 //Boton General
#define GPOPWRSH 14 //Señal para energizar los sensores de 3.3VDC
#define GPAHUSOL A0 //Señal An del sensor de humedad de suelo

/*============================================================================*\
 * Exported Preprocessor #define Constants
\*============================================================================*/
#define LED_ESP_ON  LOW
#define LED_ESP_OFF HIGH

/*============================================================================*\
 * Exported Function Prototypes
 * ----------------------------------------------------------------------------
 * The header information for each of the following function prototypes is 
 * located in the source file
\*============================================================================*/

/*============================================================================*\
 * Exported In-line Function Definitions (#define function-like macros)
\*============================================================================*/
#define PRENDER_GPO(CH_OUTPUT) digitalWrite(CH_OUTPUT, HIGH)
#define APAGARF_GPO(CH_OUTPUT) digitalWrite(CH_OUTPUT, LOW)

#define APAGAR_BOMBA() if(digitalRead(GPOBOMBA)){             \
                        digitalWrite(GPOBOMBA, LOW);          \
                        MQTTPUBREGAR_ST(MQTT_STATE_OFF);      \
                        MQTTPUBREGANDO_ST(MQTT_STATE_OFF);    \
                      }
#define PRENDER_BOMBA() if(!digitalRead(GPOBOMBA)){           \
                         digitalWrite(GPOBOMBA, HIGH);        \
                         MQTTPUBREGAR_ST(MQTT_STATE_ON);      \
                         MQTTPUBREGANDO_ST(MQTT_STATE_ON);    \
                      }

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
#endif /* } HW_IO_H */