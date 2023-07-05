#ifndef FW_HANDLR_RIEGO_H /* { */
#define FW_HANDLR_RIEGO_H
/*===========================================================================*\
 * FILE: name: FW_HandlerRiego.h
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
 *   This file contains the Handler Riego FW Headers
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

/*============================================================================*\
 * Exported Preprocessor #define Constants
\*============================================================================*/
#define HUM_BAJA_REGAR 30
#define HUM_ALTA_NO_REGAR 80
#define NUM_MAX_CUENTAS_SEGURIDAD 10

/*============================================================================*\
 * Exported Preprocessor #define MACROS
\*============================================================================*/

/*============================================================================*\
 * Exported Type Declarations
\*============================================================================*/

/*============================================================================*\
 * Exported Object Declarations
\*============================================================================*/
extern bool Regar_Flag;

/*============================================================================*\
 * Exported Function Prototypes
 * ----------------------------------------------------------------------------
 * The header information for each of the following function prototypes is 
 * located in the source file
\*============================================================================*/
extern void Fun_Verificar_Regar(uint8_t hum_maceta_validar);

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
 * 01-FEB-2022  Israel Gomez
 * - Created initial file.
 *
\*============================================================================*/
#endif /* } FW_HANDLR_RIEGO_H */