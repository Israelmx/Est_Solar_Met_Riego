/*===========================================================================*\
 * FILE: name: FW_Wifi.cpp
 *===========================================================================
 * Copyright 2021 IXGL Electronics., All Rights Reserved.
 * IXGL Electronics
 *---------------------------------------------------------------------------
 * version: 1
 * derived_by: Israel Gomez
 * date_modified: 03/08/2021
 *----------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains the Wifi features for Control_Acuario_Rellenador program.
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
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

/*============================================================================*\
 * Other Header Files
\*============================================================================*/
#include "FW_Wifi.h"
#include "FW_MQTT.h"
#include "FW_Debug.h"
#include "HW_IO.h"

/*============================================================================*\
 * Local Preprocessor #define Constants
\*============================================================================*/
#define TIMER_VERIRICAR_WIFI_MQTT 30000 //Quiero si perdi cnx al Wifi y al servidor de MQTT intente reconectar cada 30 segundos
#define NUM_MAX_CNTS_SEG_RED 20 //Despues de 20 intentos de conectar con red se duerme si no lo ha logrado

/*============================================================================*\
 * Local Preprocessor #define MACROS
\*============================================================================*/

/*============================================================================*\
 * Local Type Declarations
\*============================================================================*/

/*============================================================================*\
 * Exported Object Definitions
\*============================================================================*/
Connection_Status_T Connection_Status = WIFI_DOWN_MQTT_DOWN;   // Connection status for WiFi and MQTT:

/*============================================================================*\
 * Local Object Definitions
\*============================================================================*/
// WiFi network info.
const char* SSID = "EatonDemoMX24";
const char* PASSWORD = "eaton54321";
// const char* SSID = "Gonar";
// const char* PASSWORD = "2008201112";

String NewHostName = "ESP_EstSol_1";

/*============================================================================*\
 * Local Function Prototypes
\*============================================================================*/
void Fun_Connection_Status(void);

/*============================================================================*\
 * Local Inline Function Definitions and Function-Like Macros
\*============================================================================*/

/*============================================================================*\
 * Expoerted Preprocessor Declaration
\*============================================================================*/
WiFiClient clientnetwf;
MQTTClient clientmqttc;
AsyncWebServer server_ota(80);

/*******************************************************************************
 * FUNCTION: Fun_Checa_MQTT_Con
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
 * Fun que verifica el estado de cnx del servidor MQTT
 * 
 * PRECONDITIONS:
 * None.
 * 
 * POSTCONDITIONS:
 * None.      
 *
\******************************************************************************/
void Fun_Checa_MQTT_Con()
{
  static long previousmillis_wifi_mqtt_chk;
  static unsigned long currentmillis_wifi_mqtt_chk;

  if(!clientmqttc.loop()) 
  {
    currentmillis_wifi_mqtt_chk = millis();
    if(currentmillis_wifi_mqtt_chk - previousmillis_wifi_mqtt_chk > TIMER_VERIRICAR_WIFI_MQTT)
    {
      previousmillis_wifi_mqtt_chk = currentmillis_wifi_mqtt_chk;
      Connection_Status = WIFI_DOWN_MQTT_DOWN;
      Fun_Connection_Status();
    }
  }
  delay(15);
}

/*******************************************************************************
 * FUNCTION: Fun_Connection_Status
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
 * Fun que verifica el estado de cnx de Wifi y MQTT
 * 
 * PRECONDITIONS:
 * None.
 * 
 * POSTCONDITIONS:
 * None.      
 *
\******************************************************************************/
void Fun_Connection_Status()
{
  int wifi_status;

  #ifdef IDEBUGSERIAL
  int mqtt_status;
  #endif

  wifi_status = WiFi.status();
  
  #ifdef IDEBUGSERIAL
  mqtt_status = clientmqttc.connected();
  #endif

  #ifdef IDEBUGSERIAL
  Serial.println("Verificando estado de cnx con Wifi y el Servidor MQTT");
  Serial.print("Wifi Status: ");
  Serial.println(wifi_status);
  Serial.print("MQTT Status: ");
  Serial.println(mqtt_status);
  #endif
  
  if (wifi_status != WL_CONNECTED)
  {
    #ifdef IDEBUGSERIAL
    Serial.println("WiFi and MQTT down: reconectar primero al WiFi");
    #endif
    WiFi.reconnect();
  }
  else if(Connection_Status != WIFI_UP_MQTT_UP)
  {
    #ifdef IDEBUGSERIAL
    Serial.println("WiFi UP, MQTT down: reconectar al servidor MQTT");
    #endif

    if(clientmqttc.connect(HOST_MQTT_DEVICE_ID))
    {
      #ifdef IDEBUGSERIAL
      Serial.println("Reconectado al Servidor MQTT");
      #endif
      if(clientmqttc.subscribe((String(MQTT_TOPIC) + String(MQTT_SUBTOPICGRAL)).c_str()))
      {
        #ifdef IDEBUGSERIAL
        Serial.println("ReSuscrito al Topico");
        #endif
        Connection_Status = WIFI_UP_MQTT_UP;
      }
      else
      {
        #ifdef IDEBUGSERIAL
        Serial.println("NO Suscrito al Topico: ");
        #endif
      }
    }
    else
    {
      #ifdef IDEBUGSERIAL
      Serial.println("No se ha podido reconectar al Servidor MQTT");
      #endif
    }
  }
  else
  {
    //do nothing
  }
}

/*******************************************************************************
 * FUNCTION: Fun_WifiConnect
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
 * Fun que conecta por primera vez al Wifi y al servidor MQTT
 * 
 * PRECONDITIONS:
 * None.
 * 
 * POSTCONDITIONS:
 * None.      
 *
\******************************************************************************/
void Fun_WifiConnect()
{
  uint8_t cont_tmp_seg_red;

  #ifdef IDEBUGSERIAL
  Serial.print("Conectando al WiFi ");
  #endif

  clientmqttc.begin(MQTT_IPSERVER, atoi(MQTT_PORT), clientnetwf);
  clientmqttc.onMessage(Fun_MessageReceived);

  WiFi.begin(SSID, PASSWORD);

  cont_tmp_seg_red = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    #ifdef IDEBUGSERIAL
    Serial.print(".");
    #endif
    digitalWrite(LED_BUILTIN, LED_ESP_ON);
    delay(500);
    digitalWrite(LED_BUILTIN, LED_ESP_OFF);
    delay(500);

    cont_tmp_seg_red++;
    if(cont_tmp_seg_red == NUM_MAX_CNTS_SEG_RED)
    {
      #ifdef IDEBUGSERIAL
      Serial.println("Deep Sleep por sobrepasar tiempo max de red por Wifi");
      #endif
      ESP.deepSleep(2 * 60 * 1000000);   //El primer digito son los minutos que quieres el DS
    }
  }
  #ifdef IDEBUGSERIAL
  Serial.println("");
  Serial.print("WiFi conectado con IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("ESP MAC Address: ");
  Serial.println(WiFi.macAddress());
  #endif

  //TODO: Cambiar tambien el nombre de la Control en el web interface
  server_ota.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Control Riego Gardenia V1.1");
  });

  AsyncElegantOTA.begin(&server_ota);    // Start ElegantOTA
  server_ota.begin();
  #ifdef IDEBUGSERIAL
  Serial.println("HTTP server started");
  #endif

  #ifdef IDEBUGSERIAL
  Serial.print("Conectando al Servidor MQTT ");
  #endif
  cont_tmp_seg_red = 0;
  while(!clientmqttc.connect(HOST_MQTT_DEVICE_ID)) 
  {
    #ifdef IDEBUGSERIAL
    Serial.print(".");
    #endif
    digitalWrite(LED_BUILTIN, LED_ESP_ON);
    delay(250);
    digitalWrite(LED_BUILTIN, LED_ESP_OFF);
    delay(2000);

    cont_tmp_seg_red++;
    if(cont_tmp_seg_red == NUM_MAX_CNTS_SEG_RED)
    {
      #ifdef IDEBUGSERIAL
      Serial.println("Deep Sleep por sobrepasar tiempo max de red por MQTT");
      #endif
      ESP.deepSleep(2 * 60 * 1000000);   //El primer digito son los minutos que quieres el DS
    }
  }

  #ifdef IDEBUGSERIAL
  Serial.println("");
  Serial.print("Conectado al Servidor MQTT: ");
  Serial.println(MQTT_IPSERVER);
  #endif
  
  if(clientmqttc.subscribe((String(MQTT_TOPIC) + String(MQTT_SUBTOPICGRAL)).c_str()))
  {
    Connection_Status = WIFI_UP_MQTT_UP;
    #ifdef IDEBUGSERIAL
    Serial.print("Suscrito al Topico: ");
    #endif
  }
  else
  {
    #ifdef IDEBUGSERIAL
    Serial.print("No se pudo suscribir al Topico: ");
    #endif
  }
  #ifdef IDEBUGSERIAL
  Serial.println(String(MQTT_TOPIC) + String(MQTT_SUBTOPIC));
  #endif
}