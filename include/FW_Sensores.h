#ifndef FW_SENSORES_H /* { */
#define FW_SENSORES_H
/*===========================================================================*\
 * FILE: name: FW_Sensores.h
 *===========================================================================
 * Copyright 2021 IXGL Electronics., All Rights Reserved.
 * IXGL Electronics
 *---------------------------------------------------------------------------
 * version: 1
 * derived_by: Israel Gomez
 * date_modified: 05/ENE/2022
 *----------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   Archico que contiene los headers para leer los Sensores de Corrietne de la lampara del Acuario
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
#include <Wire.h>
#include "INA3221_IXGL.h"
#include <AHT10.h>
#include <BH1750.h>

/*============================================================================*\
 * Other Header Files
\*============================================================================*/

/*============================================================================*\
 * Exported Preprocessor #define Constants
\*============================================================================*/

/*============================================================================*\
 * Exported Preprocessor #define MACROS
\*============================================================================*/

/*============================================================================*\
 * Exported Type Declarations
\*============================================================================*/
extern float INA_Corriente_Carga;

/*============================================================================*\
 * Exported Object Declarations
\*============================================================================*/

/*============================================================================*\
 * Exported Function Prototypes
 * ----------------------------------------------------------------------------
 * The header information for each of the following function prototypes is 
 * located in the source file
\*============================================================================*/
extern void Fun_Leer_INA3221(void);
extern void Fun_Init_Sensores(void);
extern void Fun_Leer_Temp_AHT10(void);
extern void Fun_Leer_Hum_AHT10(void);
extern void Fun_Leer_Lux(void);
extern void Fun_Rutina_Sensores(void);
extern void Fun_Leer_CapSoil(void);

/*============================================================================*\
 * Exported In-line Function Definitions (#define function-like macros)
\*============================================================================*/

/*============================================================================*\
 * Exported Preprocessor Declaration
\*============================================================================*/

/*============================================================================*\
 * File Revision History (top to bottom: first revision to last revision)
 *==============================================================================
 *
 * Date         userid    (Description on following lines: SCR #, etc.)
 * -----------  --------
 * 05-ENE-2022  Israel Gomez
 * - Created initial file.
 *
\*============================================================================*/
#endif /* } FW_SENSORES_H */