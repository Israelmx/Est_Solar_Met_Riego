/*===========================================================================*\
 * FILE: name: INA3221_IXGL.cpp
 *===========================================================================
 * Copyright 2021 IXGL Electronics., All Rights Reserved.
 * IXGL Electronics
 *---------------------------------------------------------------------------
 * version: 1
 * derived_by: Israel Gomez
 * date_modified: 07/19/2021
 *----------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   Archico que contiene los metodos de la clase INA3221
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
#include "INA3221_IXGL.h"

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
 * Local Preprocessor Declaration
\*============================================================================*/

/*******************************************************************************
 * FUNCTION: INA3221_IXGL CONSTRUCTOR
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
 * Constructor para Inicializar los atributos de la clase
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
INA3221_IXGL::INA3221_IXGL(INA3221_ADDRESS_T addr, float shuntresistor)
{
    _INA3221_i2caddr = addr;
    _INA3221_shuntresistor = shuntresistor;
}

/*******************************************************************************
 * FUNCTION: ~INA3221_IXGL Destructor
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
 * Constructor para Inicializar los atributos de la clase
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
INA3221_IXGL::~INA3221_IXGL()
{

}

/*******************************************************************************
 * FUNCTION: Mtd_wireWriteRegister
 *******************************************************************************
 * RETURN VALUE:
 *    None.
 *
 * PARAMETERS:
 *    reg   Registro al que se escribira el comando por I2C
 *    value Valor del comando que se va a escribir al registro
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
 * Metodo para mandar un camando a un registro del INA3221 por I2C
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void INA3221_IXGL::Mtd_wireWriteRegister (uint8_t reg, uint16_t value)
{
  Wire.beginTransmission(_INA3221_i2caddr);
  #if ARDUINO >= 100
    Wire.write(reg);                       // Register
    Wire.write((value >> 8) & 0xFF);       // Upper 8-bits
    Wire.write(value & 0xFF);              // Lower 8-bits
  #else
    Wire.send(reg);                        // Register
    Wire.send(value >> 8);                 // Upper 8-bits
    Wire.send(value & 0xFF);               // Lower 8-bits
  #endif
  Wire.endTransmission();
}

/*******************************************************************************
 * FUNCTION: Mtd_wireReadRegister
 *******************************************************************************
 * RETURN VALUE:
 *    None.
 *
 * PARAMETERS:
 *    reg   Registro al que se quiere leer por I2C
 *    value Registro del micro el cual guardara el valor del registro del sensor
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
 * Metodo para leer un un registro del INA3221 por I2C
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void INA3221_IXGL::Mtd_wireReadRegister(uint8_t reg, uint16_t *value)
{
  Wire.beginTransmission(_INA3221_i2caddr);
  #if ARDUINO >= 100
    Wire.write(reg);                       // Register
  #else
    Wire.send(reg);                        // Register
  #endif
  Wire.endTransmission();
  
  delay(1); // Max 12-bit conversion time is 586us per sample

  Wire.requestFrom(((uint8_t)_INA3221_i2caddr), ((uint8_t)2));  
  #if ARDUINO >= 100
    // Shift values to create properly formed integer
    *value = ((Wire.read() << 8) | Wire.read());
  #else
    // Shift values to create properly formed integer
    *value = ((Wire.receive() << 8) | Wire.receive());
  #endif
}

/*******************************************************************************
 * FUNCTION: Mtd_INA3221SetConfig
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
 * Metodo para mandar la configuracion inicial del INA3221 que queremos
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void INA3221_IXGL::Mtd_INA3221SetConfig(void)
{
  // Set Config register to take into account the settings above
  uint16_t config = INA3221_CONFIG_ENABLE_CHAN1 |
                    INA3221_CONFIG_ENABLE_CHAN2 |
                    INA3221_CONFIG_ENABLE_CHAN3 |
                    INA3221_CONFIG_AVG1 |
                    INA3221_CONFIG_VBUS_CT2 |
                    INA3221_CONFIG_VSH_CT2 |
                    INA3221_CONFIG_MODE_2 |
                    INA3221_CONFIG_MODE_1 |
                    INA3221_CONFIG_MODE_0;

  Mtd_wireWriteRegister(INA3221_REG_CONFIG, config);
}

/*******************************************************************************
 * FUNCTION: Mtd_begin
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
 * Metodo para iniciar la configuracion del INA3221
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void INA3221_IXGL::Mtd_begin()
{
  Wire.begin();    
  // Set chip to known config values to start
  Mtd_INA3221SetConfig();
  
  #ifdef IDEBUGSERIAL
  Serial.print("shunt resistor = ");
  Serial.println(_INA3221_shuntresistor);
  Serial.print("INA3221 Address: ");
  Serial.println(_INA3221_i2caddr);
  #endif
}

/*******************************************************************************
 * FUNCTION: Mtd_getBusVoltage_raw
 *******************************************************************************
 * RETURN VALUE:
 *    Retorna un entero 16bits con el valor del canal a leer
 *
 * PARAMETERS:
 *    channel: El canal a leer
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
 * Metodo que Gets the raw bus voltage (16-bit signed integer, so +-32767)
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
int16_t INA3221_IXGL::Mtd_getBusVoltage_raw(int channel)
{
  uint16_t value;
  Mtd_wireReadRegister(INA3221_REG_BUSVOLTAGE_1 + ((channel -1) * 2), &value);

  #ifdef IDEBUGSERIAL
  Serial.print("BusVoltage_raw: ");
  Serial.println(value,HEX);
  #endif

  // Shift to the right 3 to drop CNVR and OVF and multiply by LSB
  return (int16_t)(value);
}

/*******************************************************************************
 * FUNCTION: Mtd_getShuntVoltage_raw
 *******************************************************************************
 * RETURN VALUE:
 *    Retorna un entero 16bits con el valor del canal a leer
 *
 * PARAMETERS:
 *    channel: El canal a leer
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
 * Metodo que Gets the raw shunt voltage (16-bit signed integer, so +-32767)
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
int16_t INA3221_IXGL::Mtd_getShuntVoltage_raw(int channel)
{
  uint16_t value;
  Mtd_wireReadRegister(INA3221_REG_SHUNTVOLTAGE_1 + ((channel -1) * 2), &value);
  #ifdef IDEBUGSERIAL
  Serial.print("ShuntVoltage_raw: ");
  Serial.println(value,HEX);
  #endif
  
  return (int16_t)value;
}

/*******************************************************************************
 * FUNCTION: Mtd_getShuntVoltage_mV
 *******************************************************************************
 * RETURN VALUE:
 *    Retorna un float con el valor del canal a leer
 *
 * PARAMETERS:
 *    channel: El canal a leer
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
 * Metodo que Gets the shunt voltage in mV (so +-168.3mV)
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
float INA3221_IXGL::Mtd_getShuntVoltage_mV(int channel)
{
  int16_t value;
  value = Mtd_getShuntVoltage_raw(channel);
  return value * 0.005;
}

/*******************************************************************************
 * FUNCTION: Mtd_getBusVoltage_V
 *******************************************************************************
 * RETURN VALUE:
 *    Retorna un float con el valor del canal a leer
 *
 * PARAMETERS:
 *    channel: El canal a leer
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
 * Metodo que Gets the shunt voltage in volts
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
float INA3221_IXGL::Mtd_getBusVoltage_V(int channel)
{
  int16_t value = Mtd_getBusVoltage_raw(channel);
  return value * 0.001;
}

/*******************************************************************************
 * FUNCTION: Mtd_getCurrent_mA
 *******************************************************************************
 * RETURN VALUE:
 *    Retorna un float con el valor del canal a leer
 *
 * PARAMETERS:
 *    channel: El canal a leer
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
 * Metodo que Gets the current value in mA, taking into account the config
 * settings and current LSB
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
float INA3221_IXGL::Mtd_getCurrent_mA(int channel)
{
    float valueDec = Mtd_getShuntVoltage_mV(channel) / _INA3221_shuntresistor;
  return valueDec;
}

/*******************************************************************************
 * FUNCTION: Mtd_getManufID
 *******************************************************************************
 * RETURN VALUE:
 *    Retorna un entero de 16bits con el valor del ManufID
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
 * Metodo que Gets the Manufacturers ID
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
int INA3221_IXGL::Mtd_getManufID()
{
  uint16_t value;
  Mtd_wireReadRegister(0xFE, &value);
  return value;
}