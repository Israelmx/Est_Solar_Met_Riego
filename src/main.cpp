/*===========================================================================*\
 * FILE: name: Riego_Automatico
 *===========================================================================
 * Copyright 2020 IXGL Electronics., All Rights Reserved.
 * IXGL Electronics
 *---------------------------------------------------------------------------
 * version: 1
 * derived_by: Israel Gomez
 * date_modified: 20/Jul/2021
 *----------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains the Riego Automatico program.
 *
 * ABBREVIATIONS:
 *   None.
 *
 * TRACEABILITY INFO:
 *   Design Document(s):
 *
 * Requirements Document(s):
 *
 *
 * DEVIATIONS FROM STANDARDS:
 *
\*===========================================================================*/
/*============================================================================*\
 * Standard Header Files
\*============================================================================*/
#include <Arduino.h>

/*============================================================================*\
 * Program Header Files
\*============================================================================*/
#include "FW_Debug.h"
#include "HW_IO.h"
#include "FW_HandlrRiego.h"
#include "FW_MQTT.h"
#include "FW_Wifi.h"
#include "FW_Btn_Handler.h"
#include "FW_Sensores.h"
#include "FW_HandlrRiego.h"

/*============================================================================*\
 * Local Preprocessor #define Constants
\*============================================================================*/

/*============================================================================*\
 * Local Preprocessor #define MACROS
\*============================================================================*/

/*============================================================================*\
 * Local Type Declarations
\*============================================================================*/

/*============================================================================*\
 * Local Variable Definitions
\*============================================================================*/

/*============================================================================*\
 * Local Preprocessor Declaration
\*============================================================================*/

/*============================================================================*\
 * Local Function Prototypes
\*============================================================================*/

/*============================================================================*\
 * Local Inline Function Definitions and Function-Like Macros
\*============================================================================*/

/*============================================================================*\
 * Function Definitions
\*============================================================================*/

/*******************************************************************************
 * FUNCTION: setup
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
 * Setup de Arduino del programa
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void setup()
{
  delay(250);

  pinMode(GPIBTNGR, INPUT);
  pinMode(GPOBOMBA, OUTPUT);
  pinMode(GPOPWRSH, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  #ifdef IDEBUGSERIAL
  Serial.begin(SERIALBAUDR);
  while (!Serial)
  {
    // wait serial port initialization
  }
  Serial.println(""); Serial.println("Wake up!!!");
  #endif

  digitalWrite(LED_BUILTIN, LED_ESP_OFF);

  Fun_WifiConnect();

  delay(500);

  Fun_Valida_Boton();

  if(!Btn_Press_Flag)
  {
    PRENDER_GPO(GPOPWRSH);
    delay(1000);    //Tiempo muerto para que los sensores prendan correctamente
    Fun_Init_Sensores();
    Fun_Rutina_Sensores();
    APAGARF_GPO(GPOPWRSH);
  }

  Regar_Flag = false; //TODO Si no se tiene bomba quitar comentario a esta linea
  if(!Btn_Press_Flag && !Regar_Flag)
  {
    #ifdef IDEBUGSERIAL
    Serial.println("Deep Sleep");
    #endif
    ESP.deepSleep(5 * 60 * 1000000);   //El primer digito son los minutos que quieres el DS
  }
}

/*******************************************************************************
 * FUNCTION: loop
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
 * Loop infinito de Arduino
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void loop()
{
  static u_int8_t cuentas_seguridad = 0;
  static unsigned long tiempo1 = 0;
  static unsigned long tiempo2 = 0;
  #ifdef IDEBUGSERIAL
  static unsigned long tiempoSegundos = 0;
  #endif

  tiempo2 = millis();
  if(tiempo2 > (tiempo1 + 20000))
  {
    tiempo1 = millis();
    #ifdef IDEBUGSERIAL
    tiempoSegundos = tiempo1 / 1000;
    Serial.print("Ha transcurrido: ");
    Serial.print(tiempoSegundos);
    Serial.println(" desde que se encendio el Arduino");
    PRENDER_GPO(GPOPWRSH);
    delay(1000);
    Fun_Rutina_Sensores();
    APAGARF_GPO(GPOPWRSH);
    #endif

    if(!Btn_Press_Flag)
    {
      cuentas_seguridad++;
      if(cuentas_seguridad == NUM_MAX_CUENTAS_SEGURIDAD)
      {
        Regar_Flag = false;
        APAGAR_BOMBA();
      }

      if(!Regar_Flag)
      {
        #ifdef IDEBUGSERIAL
        Serial.println("Deep Sleep por termino de rutina de riego");
        #endif
        ESP.deepSleep(5 * 60 * 1000000);   //El primer digito son los minutos que quieres el DS
      }
    }
  }
}