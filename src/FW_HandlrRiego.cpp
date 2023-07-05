/*===========================================================================*\
 * FILE: name: FW_HandlrRiego.cpp
 *===========================================================================
 * Copyright 2021 IXGL Electronics., All Rights Reserved.
 * IXGL Electronics
 *---------------------------------------------------------------------------
 * version: 1
 * derived_by: Israel Gomez
 * date_modified: 20/FEB/2022
 *----------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains the Handler Riego FW
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
#include "FW_HandlrRiego.h"
#include "HW_IO.h"
#include "FW_MQTT.h"
#include "FW_Debug.h"
#include "FW_Sensores.h"

/*============================================================================*\
 * Local Preprocessor #define Constants
\*============================================================================*/
#define CORRIENTE_BAJA_NO_REGAR 150.00
#define CORRIENTE_ALTA_NO_REGAR 270.00

/*============================================================================*\
 * Local Preprocessor #define MACROS
\*============================================================================*/

/*============================================================================*\
 * Local Type Declarations
\*============================================================================*/

/*============================================================================*\
 * Exported Object Definitions
\*============================================================================*/
bool Regar_Flag = false;

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
 * FUNCTION: Fun_
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
 * Fun que 
 * 
 * PRECONDITIONS:
 * None.
 * 
 * POSTCONDITIONS:
 * None.      
 *
\******************************************************************************/
void Fun_Verificar_Regar(uint8_t hum_maceta_validar)
{
  if(hum_maceta_validar < HUM_BAJA_REGAR)
  {
    Regar_Flag = true;
    PRENDER_BOMBA();
    #ifdef IDEBUGSERIAL
    Serial.println("La Humedad es menor al setpoint iniciar riego");
    #endif
  }
  else if(hum_maceta_validar > HUM_ALTA_NO_REGAR)
  {
    Regar_Flag = false;
    APAGAR_BOMBA();
    #ifdef IDEBUGSERIAL
    Serial.println("La Humedad es mayor al setpoint parar riego");
    #endif
  }
}
