/*===========================================================================*\
 * FILE: name: FW_MQTT.cpp
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

/*============================================================================*\
 * Other Header Files
\*============================================================================*/
#include "FW_MQTT.h"
#include "FW_Wifi.h"
#include "FW_Debug.h"
#include "HW_IO.h"

/*============================================================================*\
 * Local Preprocessor #define Constants
\*============================================================================*/
//TODO: Cambiar si se cambia el topico principal publicar RcbCmmn
#define STARTINDEXSUBTOPIC 42   // Topic: IsraXav/Jardin/Ctrl_Riego/Gardenia/RcbCmmn 42 chars

/*============================================================================*\
 * Local Preprocessor #define MACROS
\*============================================================================*/

/*============================================================================*\
 * Local Type Declarations
\*============================================================================*/
typedef enum El_SsubTopic_Es_Tag
{
  SSUBTOPIC_REGAR,
  SSUBTOPIC_ACTDESFUNRGR,   //Activar / Desactivar Funcion Regar
  SSUBTOPIC_NO_DEFINIDO
}
El_SsubTopic_Es_T;

/*============================================================================*\
 * Exported Object Definitions
\*============================================================================*/
//MQTT Topics
//TODO: Cambiar si es para un nuevo topico principal
const char* MQTT_TOPIC               = "IsraXav/Jardin/Ctrl_Riego/Gardenia";
//MQTT Pub Topics
const char* MQTT_PUBTOPIC_NIVHUMEDAD = "/NivDeHumedad";
const char* MQTT_PUBTOPIC_INA3221VCS = "/INA3221VCS";
const char* MQTT_PUBTOPIC_INA3221ICS = "/INA3221ICS";
const char* MQTT_PUBTOPIC_INA3221VBL = "/INA3221VBL";
const char* MQTT_PUBTOPIC_INA3221IBL = "/INA3221IBL";
const char* MQTT_PUBTOPIC_INA3221VLD = "/INA3221VLD";
const char* MQTT_PUBTOPIC_INA3221ILD = "/INA3221ILD";
const char* MQTT_PUBTOPIC_ESLR_TEMP  = "/ESLR_TEMP";
const char* MQTT_PUBTOPIC_ESLR_HUM   = "/ESLR_HUM";
const char* MQTT_PUBTOPIC_ESLR_LUX   = "/ESLR_LUX";
const char* MQTT_PUBTOPIC_REGANDO    = "/Regando";
//MQTT Sub Topic
const char* MQTT_SUBTOPICGRAL        = "/RcbCmmn/#"; //Topico a suscribirse
const char* MQTT_SUBTOPIC            = "/RcbCmmn";   //Topico a suscribirse
//MQTT SSubs Topics
const char* MQTT_SSUBTOPIC_REGAR     = "/Regar";
const char* MQTT_SSUBTOPIC_ACTDESFUNRGR = "/ActDesFunRgr";
//MQTT Pub state Topic
const char* MQTT_STATE               = "/state";     //Agrega en el topico la palabra state

//TODO Cambiar segun ID que se vaya generando por ahora es 02
const char HOST_MQTT_DEVICE_ID[] = "ESP8266_V01_ROAOLI";   //ID del dispositivo Riego Automatico

const char MQTT_STATE_ON[] = "ON";
const char MQTT_STATE_OFF[] = "OFF";

//TODO: Cambiar IP del servidor MQTT
const char MQTT_IPSERVER[] = "192.168.17.58";
const char MQTT_PORT[] = "1883";

bool MsjMQTTRcb = false;

/*============================================================================*\
 * Local Object Definitions
\*============================================================================*/
El_SsubTopic_Es_T El_SsubTopic_Es;

/*============================================================================*\
 * Local Function Prototypes
\*============================================================================*/

/*============================================================================*\
 * Local Inline Function Definitions and Function-Like Macros
\*============================================================================*/

/*============================================================================*\
 * Local Preprocessor Declaration
\*============================================================================*/
CircularBuffer<String, CIRCULARBUFFERSIZE> SSubTopic_Bfr;

/*============================================================================*\
 * Exported Preprocessor Declaration
\*============================================================================*/
CircularBuffer<String, CIRCULARBUFFERSIZE> Mqtt_PL_Rcd_Bfr;

/*******************************************************************************
 * FUNCTION: Fun_messageReceived
 *******************************************************************************
 * RETURN VALUE:
 *    None.
 *     
 * PARAMETERS:
 *    None.
 *     
 * EXTERNAL REFERENCES:
 *    None.
 *     
 * DEVIATION FROM STANDARDS:
 *    None.
 *
 * -----------------------------------------------------------------------------
 * ABSTRACT:
 * -----------------------------------------------------------------------------
 * Fun que maneja los mensajes de MQTT entrantes
 * 
 * PRECONDITIONS:
 * None.
 * 
 * POSTCONDITIONS:
 * None.      
 *
\******************************************************************************/
void Fun_MessageReceived(String &topic, String &payload)
{
  String ssubtopic;

  ssubtopic = topic;
  ssubtopic = ssubtopic.substring(STARTINDEXSUBTOPIC);  //De todo el topico solo tomo el subtopico

  #ifdef IDEBUGSERIAL
  Serial.print("Mensaje Recibido en el Topico Suscrito: ");
  Serial.print(ssubtopic);
  Serial.print(": ");
  Serial.println(payload);
  #endif

  MsjMQTTRcb = true;

  if(ssubtopic == MQTT_SSUBTOPIC_REGAR)
  {
    El_SsubTopic_Es = SSUBTOPIC_REGAR;
  }
  if(ssubtopic == MQTT_SSUBTOPIC_ACTDESFUNRGR)
  {
    El_SsubTopic_Es = SSUBTOPIC_ACTDESFUNRGR;
  }
  else
  {
    El_SsubTopic_Es = SSUBTOPIC_NO_DEFINIDO;
    MsjMQTTRcb = false;
    #ifdef IDEBUGSERIAL
    Serial.println("No se ha definido el topico");
    #endif
  }

  if(MsjMQTTRcb)
  {
    Mqtt_PL_Rcd_Bfr.push(payload);  //Agrego el mensaje recibido de MQTT al buffer
    SSubTopic_Bfr.push(ssubtopic);  //Agrego el Sub Topico recibido de MQTT al buffer
  }
}

/*******************************************************************************
 * FUNCTION: Fun_Analizar_MsjRc
 *******************************************************************************
 * RETURN VALUE:
 *    None.
 *
 * PARAMETERS:
 *    None.
 *
 * EXTERNAL REFERENCES:
 *    None.
 *
 * DEVIATION FROM STANDARDS:
 *    None.
 *
 * -----------------------------------------------------------------------------
 * ABSTRACT:
 * -----------------------------------------------------------------------------
 * Analiza el mensaje recibido al topico suscrito
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void Fun_Analizar_MsjRc()
{
  bool msjvalido = true;
  String mqtt_pl_rcd = Mqtt_PL_Rcd_Bfr.shift(); //Extraigo del Buffer por medio de FIFO el mensaje que toca
  String sstopic = SSubTopic_Bfr.shift();       //Extraigo del Buffer por medio de FIFO el Sub Topico que toca
  
  #ifdef IDEBUGSERIAL
  Serial.println("Analizando Topico y Mensaje Recibido: ");
  #endif
  
  switch (El_SsubTopic_Es)
  {
    case SSUBTOPIC_REGAR:
    {
      if(mqtt_pl_rcd == MQTT_STATE_ON)
      {
        PRENDER_BOMBA();
      }
      else if(mqtt_pl_rcd == MQTT_STATE_OFF)
      {
        APAGAR_BOMBA();
      }
      else
      {
        msjvalido = false;
      }
      break;
    }

    case SSUBTOPIC_ACTDESFUNRGR:
    {
      if(mqtt_pl_rcd == MQTT_STATE_ON)
      {
        //TODO: Implementar codigo para estaciones que no manden a dormir el ESP
      }
      else if(mqtt_pl_rcd == MQTT_STATE_OFF)
      {
        //TODO: Implementar codigo para estaciones que no manden a dormir el ESP
      }
      else
      {
        msjvalido = false;
      }
      break;
    }

    case SSUBTOPIC_NO_DEFINIDO:
    {
      #ifdef IDEBUGSERIAL
      Serial.println("No se ha definido el topico");
      #endif
      msjvalido = false;
      break;
    }

    default:
    {
      msjvalido = false;
      break;
    }
  }
  
  if(msjvalido)
  {
    clientmqttc.publish((String(MQTT_TOPIC) + sstopic + String(MQTT_STATE)), mqtt_pl_rcd.c_str());
  }
  else
  {
    #ifdef IDEBUGSERIAL
    Serial.println("Mensaje no valido en un SSusTopico Valido");
    #endif
  }
}