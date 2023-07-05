#ifndef INA3221_IXGL_H /* { */
#define INA3221_IXGL_H
/*===========================================================================*\
 * FILE: name: INA3221_IXGL.h
 *===========================================================================
 * Copyright 2021 IXGL Electronics., All Rights Reserved.
 * IXGL Electronics
 *---------------------------------------------------------------------------
 * version: 1
 * derived_by: Israel Gomez
 * date_modified: 07/18/2021
 *----------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   Archico que contiene los headers de la clase INA3221
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
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include <Wire.h>

/*============================================================================*\
 * Other Header Files
\*============================================================================*/

/*============================================================================*\
 * Class Preprocessor #define Constants
\*============================================================================*/
#define SHUNT_RESISTOR_VALUE                   (0.1)   // default shunt resistor value of 0.1 Ohm

/*=======================
    CONFIG REGISTER (R/W)
=======================*/
#define INA3221_REG_CONFIG                     (0x00)

#define INA3221_CONFIG_RESET                   (0x8000)  // Reset Bit

#define INA3221_CONFIG_ENABLE_CHAN1            (0x4000)  // Enable Channel 1
#define INA3221_CONFIG_ENABLE_CHAN2            (0x2000)  // Enable Channel 2
#define INA3221_CONFIG_ENABLE_CHAN3            (0x1000)  // Enable Channel 3

#define INA3221_CONFIG_AVG2                    (0x0800)  // AVG Samples Bit 2 - See table 3 spec
#define INA3221_CONFIG_AVG1                    (0x0400)  // AVG Samples Bit 1 - See table 3 spec
#define INA3221_CONFIG_AVG0                    (0x0200)  // AVG Samples Bit 0 - See table 3 spec
#define INA3221_CONFIG_VBUS_CT2                (0x0100)  // VBUS bit 2 Conversion time - See table 4 spec
#define INA3221_CONFIG_VBUS_CT1                (0x0080)  // VBUS bit 1 Conversion time - See table 4 spec
#define INA3221_CONFIG_VBUS_CT0                (0x0040)  // VBUS bit 0 Conversion time - See table 4 spec
#define INA3221_CONFIG_VSH_CT2                 (0x0020)  // Vshunt bit 2 Conversion time - See table 5 spec
#define INA3221_CONFIG_VSH_CT1                 (0x0010)  // Vshunt bit 1 Conversion time - See table 5 spec
#define INA3221_CONFIG_VSH_CT0                 (0x0008)  // Vshunt bit 0 Conversion time - See table 5 spec
#define INA3221_CONFIG_MODE_2                  (0x0004)  // Operating Mode bit 2 - See table 6 spec
#define INA3221_CONFIG_MODE_1                  (0x0002)  // Operating Mode bit 1 - See table 6 spec
#define INA3221_CONFIG_MODE_0                  (0x0001)  // Operating Mode bit 0 - See table 6 spec

/*============================
    SHUNT VOLTAGE REGISTER (R)
============================*/
#define INA3221_REG_SHUNTVOLTAGE_1             (0x01)

/*==========================
    BUS VOLTAGE REGISTER (R)
==========================*/
#define INA3221_REG_BUSVOLTAGE_1               (0x02)

/*============================================================================*\
 *  Class Preprocessor #define MACROS
\*============================================================================*/

/*============================================================================*\
 * Class Type Declarations
\*============================================================================*/
/*==================
    I2C ADDRESS/BITS
==================*/
typedef enum INA3221_ADDRESS_Tag
{
    INA3221_ADDRESS_A0_GND = (0x40),    // 1000000 (A0=GND)
    INA3221_ADDRESS_A0_VS  = (0x41),    // 1000001 (A0= VS)
    INA3221_ADDRESS_A0_SDA = (0x42),    // 1000010 (A0=SDA)
    INA3221_ADDRESS_A0_SCL = (0x43)     // 1000011 (A0=SCL)
}
INA3221_ADDRESS_T;

typedef enum Varialbes_INA3221_Tag
{
  INA3221_VOLTAJE_BUS,
  INA3221_CORRIENTE,
  INA3221_VOLTAJE_SHNT,
  INA3221_VOLTAJE_CARGA,
  INA3221_TOTALES
}
Varialbes_INA3221_T;

/*============================================================================*\
 * Class H
\*============================================================================*/
class INA3221_IXGL
{
private:
    INA3221_ADDRESS_T _INA3221_i2caddr;
    float _INA3221_shuntresistor;

public:
    INA3221_IXGL(INA3221_ADDRESS_T addr = INA3221_ADDRESS_A0_GND, float shuntresistor = SHUNT_RESISTOR_VALUE);    //Constructor
    ~INA3221_IXGL();    //Destructor
    void Mtd_begin(void);
    void Mtd_wireWriteRegister(uint8_t reg, uint16_t value);
    void Mtd_wireReadRegister(uint8_t reg, uint16_t *value);
    void Mtd_INA3221SetConfig(void);
    float Mtd_getBusVoltage_V(int channel);
    float Mtd_getShuntVoltage_mV(int channel);
    float Mtd_getCurrent_mA(int channel);
    int Mtd_getManufID();
    int16_t Mtd_getBusVoltage_raw(int channel);
    int16_t Mtd_getShuntVoltage_raw(int channel);
};


/*============================================================================*\
 * File Revision History (top to bottom: first revision to last revision)
 *==============================================================================
 *
 * Date         userid    (Description on following lines: SCR #, etc.)
 * -----------  --------
 * 18-JUL-2021  Israel Gomez
 * - Created initial file.
 *
\*============================================================================*/
#endif /* } INA3221_IXGL_H */