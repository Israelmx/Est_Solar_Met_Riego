/*===========================================================================*\
 * FILE: name: FW_Btn_Handler.cpp
 *===========================================================================
 * Copyright 2021 IXGL Electronics., All Rights Reserved.
 * IXGL Electronics
 *---------------------------------------------------------------------------
 * version: 1
 * derived_by: Israel Gomez
 * date_modified: 01/FEB/2022
 *----------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains the Button Handler FW
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
#include <Arduino.h>

/*============================================================================*\
 * Other Header Files
\*============================================================================*/
#include "FW_Btn_Handler.h"
#include "HW_IO.h"
#include "FW_Debug.h"

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
 * Exported Object Definitions
\*============================================================================*/
bool Btn_Press_Flag = false;

/*============================================================================*\
 * Local Object Definitions
\*============================================================================*/

/*============================================================================*\
 * Local Function Prototypes
\*============================================================================*/

/*============================================================================*\
 * Local Inline Function Definitions and Function-Like Macros
\*============================================================================*/

/*============================================================================*\
 * Expoerted Preprocessor Declaration
\*============================================================================*/

/*******************************************************************************
 * FUNCTION: Fun_Valida_Boton
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
 * Fun que valida si el boton general (en este caso para entrar en modod programar) se apreto
 * 
 * PRECONDITIONS:
 * None.
 * 
 * POSTCONDITIONS:
 * None.      
 *
\******************************************************************************/
void Fun_Valida_Boton()
{
  Btn_Press_Flag = false;
  
  if(!digitalRead(GPIBTNGR))  //Si el btn esta apretado
  {
    delay(5);
    #ifdef IDEBUGSERIAL
    Serial.println("Primer Debounce");
    #endif
    if(!digitalRead(GPIBTNGR))
    {
      delay(5);
      #ifdef IDEBUGSERIAL
      Serial.println("Segundo Debounce");
      #endif
      if(!digitalRead(GPIBTNGR))
      {
        Btn_Press_Flag = true;
        digitalWrite(LED_BUILTIN, LED_ESP_ON);
        #ifdef IDEBUGSERIAL
        Serial.println("Se apreto boton para entrar a modo programacion con Debounce");
        #endif
      }
    }
  }
}