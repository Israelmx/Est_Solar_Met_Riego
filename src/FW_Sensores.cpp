/*===========================================================================*\
 * FILE: name: FW_Sensores.cpp
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
 *   Archico que contiene las funciones para leer los Sensores de Corrietne de la lampara del Acuario
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
#include <CAPSOIL_IXGL.h>

/*============================================================================*\
 * Other Header Files
\*============================================================================*/
#include "FW_Sensores.h"
#include "HW_IO.h"
#include "FW_Debug.h"
#include "FW_MQTT.h"
#include "FW_Btn_Handler.h"
#include "FW_HandlrRiego.h"

/*============================================================================*\
 * Local Preprocessor #define Constants
\*============================================================================*/
#define INA3221_I2C_ADDRESS 0x40
#define BH1750_ADDRESS 0x23

#define AIRVALUE 655    //Calibration value, read the ADC value from the CS Sensor in air
#define WATERVALUE 245  //Calibration value, read the ADC value from the CS Sensor in water

#define MUESTRAS_FILTRO 10  //Valor de muestras para el filtro
#define TIME_DELAY_FILTER 100  //Valor de delay en el filtro

#define TOTAL_DE_ESPERA_RESET 10

/*============================================================================*\
 * Local Preprocessor #define MACROS
\*============================================================================*/

/*============================================================================*\
 * Local Type Declarations
\*============================================================================*/
typedef enum Canales_INA3221_Tag
{
  CANAL_INA3221_Celda_Solar = 1,
  CANAL_INA3221_Bateria_Litio,
  CANAL_INA3221_Carga
}
Canales_INA3221_T;

/*============================================================================*\
 * Exported Object Definitions
\*============================================================================*/
float INA_Corriente_Carga;

/*============================================================================*\
 * Local Object Definitions
\*============================================================================*/
CAPSOIL_IXGL Sensor_Hum(GPAHUSOL, AIRVALUE, WATERVALUE);

/*============================================================================*\
 * Local Function Prototypes
\*============================================================================*/

/*============================================================================*\
 * Local Inline Function Definitions and Function-Like Macros
\*============================================================================*/
void Fun_Filter_Sensor(void);

/*============================================================================*\
 * Exported Preprocessor Declaration
\*============================================================================*/
INA3221_IXGL Sensor_INA3221;
BH1750 lightMeter(BH1750_ADDRESS);
AHT10 myAHT10(AHT10_ADDRESS_0X38);

/*******************************************************************************
 * FUNCTION: Fun_Init_Sensores
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
 * Fun inicializa los sensores
 *
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void Fun_Init_Sensores()
{
  uint8_t cuentas_antes_de_reset;
  #ifdef IDEBUGSERIAL
  Serial.println("Inicializando sensores:");
  Serial.println("Initialize INA3221");
  Serial.println("-----------------------------------------------");
  #endif
  Sensor_INA3221.Mtd_begin();
  #ifdef IDEBUGSERIAL
  Serial.print("Manufactures ID=0x");
  int MID;
  MID = Sensor_INA3221.Mtd_getManufID();
  Serial.println(MID,HEX);
  Serial.println("-----------------------------------------------");
  #endif

  if(!Btn_Press_Flag)
  {
    cuentas_antes_de_reset = 0;
    while (myAHT10.begin() != true)
    {
      cuentas_antes_de_reset++;
      #ifdef IDEBUGSERIAL
      Serial.println("AHT10 not connected or fail to load calibration coefficient");
      #endif
      digitalWrite(LED_BUILTIN, LED_ESP_ON);
      delay(1000);
      digitalWrite(LED_BUILTIN, LED_ESP_OFF);
      delay(1000);
      if(cuentas_antes_de_reset == TOTAL_DE_ESPERA_RESET)
      {
        ESP.restart();
      }
    }
    #ifdef IDEBUGSERIAL
    Serial.println("AHT10 OK");
    #endif
    
    while (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE) != true)
    {
      cuentas_antes_de_reset = 0;
      #ifdef IDEBUGSERIAL
      Serial.println("Error initialising BH1750");
      #endif
      digitalWrite(LED_BUILTIN, LED_ESP_ON);
      delay(2000);
      digitalWrite(LED_BUILTIN, LED_ESP_OFF);
      delay(2000);
      if(cuentas_antes_de_reset == TOTAL_DE_ESPERA_RESET)
      {
        ESP.restart();
      }
    }
    #ifdef IDEBUGSERIAL
    Serial.println("BH1750 OK");
    #endif
  }
  else
  {
    myAHT10.begin();
    lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
  }
}

/*******************************************************************************
 * FUNCTION: Fun_Leer_INA3221
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
 * Fun Lee el sensor INA3221 Voltaje y Corrientes de la estacion
 *
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void Fun_Leer_INA3221()
{
  float variables_ina3221_cs[INA3221_TOTALES];    //Valores Celda Solar
  float variables_ina3221_bl[INA3221_TOTALES];    //Valores Bateria de Litio
  float variables_ina3221_ld[INA3221_TOTALES];    //Valores Carga
  float filteredvalue;
  
  /*============================================================================*\
  * Primer Canal INA3221
  \*============================================================================*/
  filteredvalue = 0;
  for(uint8_t i = 0; i < MUESTRAS_FILTRO; i++)
  {
    filteredvalue = (filteredvalue + (Sensor_INA3221.Mtd_getBusVoltage_V(CANAL_INA3221_Celda_Solar)));
    delay(TIME_DELAY_FILTER);
  }
  variables_ina3221_cs[INA3221_VOLTAJE_BUS] = filteredvalue / MUESTRAS_FILTRO;
  
  filteredvalue = 0;
  for(uint8_t i = 0; i < MUESTRAS_FILTRO; i++)
  {
    filteredvalue = (filteredvalue + (Sensor_INA3221.Mtd_getCurrent_mA(CANAL_INA3221_Celda_Solar)));
    delay(TIME_DELAY_FILTER);
  }
  variables_ina3221_cs[INA3221_CORRIENTE] = filteredvalue / MUESTRAS_FILTRO;

  variables_ina3221_cs[INA3221_VOLTAJE_SHNT] = Sensor_INA3221.Mtd_getShuntVoltage_mV(CANAL_INA3221_Celda_Solar);

  #ifdef IDEBUGSERIAL
  Serial.print("INA3221 CS Bus Voltage: ");
  Serial.print(variables_ina3221_cs[INA3221_VOLTAJE_BUS], 5);
  Serial.println(" V");
  Serial.print("INA3221 CS Shunt Current: ");
  Serial.print(variables_ina3221_cs[INA3221_CORRIENTE], 5);
  Serial.println(" A");
  Serial.print("INA3221 CS Shunt Voltage: ");
  Serial.print(variables_ina3221_cs[INA3221_VOLTAJE_SHNT], 5);
  Serial.println(" mV");
  #endif

  if(!Btn_Press_Flag)
  {
    clientmqttc.publish((String(MQTT_TOPIC) + String(MQTT_PUBTOPIC_INA3221VCS)), String(variables_ina3221_cs[INA3221_VOLTAJE_BUS]).c_str());
    clientmqttc.publish((String(MQTT_TOPIC) + String(MQTT_PUBTOPIC_INA3221ICS)), String(variables_ina3221_cs[INA3221_CORRIENTE]).c_str());
  }

  /*============================================================================*\
  * Segundo Canal INA3221
  \*============================================================================*/
  filteredvalue = 0;
  for(uint8_t i = 0; i < MUESTRAS_FILTRO; i++)
  {
    filteredvalue = (filteredvalue + (Sensor_INA3221.Mtd_getBusVoltage_V(CANAL_INA3221_Bateria_Litio)));
    delay(TIME_DELAY_FILTER);
  }
  variables_ina3221_bl[INA3221_VOLTAJE_BUS] = filteredvalue / MUESTRAS_FILTRO;
  
  filteredvalue = 0;
  for(uint8_t i = 0; i < MUESTRAS_FILTRO; i++)
  {
    filteredvalue = (filteredvalue + (Sensor_INA3221.Mtd_getCurrent_mA(CANAL_INA3221_Bateria_Litio)));
    delay(TIME_DELAY_FILTER);
  }
  variables_ina3221_bl[INA3221_CORRIENTE] = filteredvalue / MUESTRAS_FILTRO;

  variables_ina3221_bl[INA3221_VOLTAJE_SHNT] = Sensor_INA3221.Mtd_getShuntVoltage_mV(CANAL_INA3221_Bateria_Litio);

  #ifdef IDEBUGSERIAL
  Serial.print("INA3221 BL Bus Voltage: ");
  Serial.print(variables_ina3221_bl[INA3221_VOLTAJE_BUS], 5);
  Serial.println(" V");
  Serial.print("INA3221 BL Shunt Current: ");
  Serial.print(variables_ina3221_bl[INA3221_CORRIENTE], 5);
  Serial.println(" A");
  Serial.print("INA3221 BL Shunt Voltage: ");
  Serial.print(variables_ina3221_bl[INA3221_VOLTAJE_SHNT], 5);
  Serial.println(" mV");
  #endif

  if(!Btn_Press_Flag)
  {
    clientmqttc.publish((String(MQTT_TOPIC) + String(MQTT_PUBTOPIC_INA3221VBL)), String(variables_ina3221_bl[INA3221_VOLTAJE_BUS]).c_str());
    clientmqttc.publish((String(MQTT_TOPIC) + String(MQTT_PUBTOPIC_INA3221IBL)), String(variables_ina3221_bl[INA3221_CORRIENTE]).c_str());
  }

  /*============================================================================*\
  * Tercel Canal INA3221
  \*============================================================================*/
  filteredvalue = 0;
  for(uint8_t i = 0; i < MUESTRAS_FILTRO; i++)
  {
    filteredvalue = (filteredvalue + (Sensor_INA3221.Mtd_getBusVoltage_V(CANAL_INA3221_Carga)));
    delay(TIME_DELAY_FILTER);
  }
  variables_ina3221_ld[INA3221_VOLTAJE_BUS] = filteredvalue / MUESTRAS_FILTRO;
  
  filteredvalue = 0;
  for(uint8_t i = 0; i < MUESTRAS_FILTRO; i++)
  {
    filteredvalue = (filteredvalue + (Sensor_INA3221.Mtd_getCurrent_mA(CANAL_INA3221_Carga)));
    delay(TIME_DELAY_FILTER);
  }
  variables_ina3221_ld[INA3221_CORRIENTE] = filteredvalue / MUESTRAS_FILTRO;
  
  variables_ina3221_ld[INA3221_VOLTAJE_SHNT] = Sensor_INA3221.Mtd_getShuntVoltage_mV(CANAL_INA3221_Carga);

  #ifdef IDEBUGSERIAL
  Serial.print("INA3221 LF Bus Voltage: ");
  Serial.print(variables_ina3221_ld[INA3221_VOLTAJE_BUS], 5);
  Serial.println(" V");
  Serial.print("INA3221 LF Shunt Current: ");
  Serial.print(variables_ina3221_ld[INA3221_CORRIENTE], 5);
  Serial.println(" A");
  Serial.print("INA3221 LF Shunt Voltage: ");
  Serial.print(variables_ina3221_ld[INA3221_VOLTAJE_SHNT], 5);
  Serial.println(" mV");
  #endif

  INA_Corriente_Carga = variables_ina3221_ld[INA3221_CORRIENTE];

  if(!Btn_Press_Flag)
  {
    clientmqttc.publish((String(MQTT_TOPIC) + String(MQTT_PUBTOPIC_INA3221VLD)), String(variables_ina3221_ld[INA3221_VOLTAJE_BUS]).c_str());
    clientmqttc.publish((String(MQTT_TOPIC) + String(MQTT_PUBTOPIC_INA3221ILD)), String(variables_ina3221_ld[INA3221_CORRIENTE]).c_str());
  }
}

/*******************************************************************************
 * FUNCTION: Fun_Leer_Temp_AHT10
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
 * Fun que lee la temperatura del sensor AHT10
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void Fun_Leer_Temp_AHT10()
{
  float temp_c;
  float filteredvalue = 0;

  for(uint8_t i = 0; i < MUESTRAS_FILTRO; i++)
  {
    filteredvalue = (filteredvalue + (myAHT10.readTemperature()));
    delay(TIME_DELAY_FILTER);
  }

  temp_c = filteredvalue / MUESTRAS_FILTRO;

  #ifdef IDEBUGSERIAL
  Serial.print("Temperatura: ");
  Serial.print(temp_c);
  Serial.println("°C");
  #endif
  
  if(!Btn_Press_Flag)
  {
    clientmqttc.publish((String(MQTT_TOPIC) + String(MQTT_PUBTOPIC_ESLR_TEMP)), String(temp_c).c_str());
  }
}

/*******************************************************************************
 * FUNCTION: Fun_Leer_Hum_AHT10
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
 * Fun que lee la humedad del sensor AHT10
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void Fun_Leer_Hum_AHT10()
{
  float humidity;
  float filteredvalue = 0;

  for(uint8_t i = 0; i < MUESTRAS_FILTRO; i++)
  {
    filteredvalue = (filteredvalue + (myAHT10.readHumidity()));
    delay(TIME_DELAY_FILTER);
  }

  humidity = filteredvalue / MUESTRAS_FILTRO;

  #ifdef IDEBUGSERIAL
  Serial.print("Humedad: ");
  Serial.print(humidity);
  Serial.println("%");
  #endif
  
  if(!Btn_Press_Flag)
  {
    clientmqttc.publish((String(MQTT_TOPIC) + String(MQTT_PUBTOPIC_ESLR_HUM)), String(humidity).c_str());
  }
}

/*******************************************************************************
 * FUNCTION: Fun_Leer_Lux
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
 * Fun que lee los luxes de la estacion solar
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void Fun_Leer_Lux()
{
  float lux;
  float filteredvalue = 0;

  for(uint8_t i = 0; i < MUESTRAS_FILTRO; i++)
  {
    filteredvalue = (filteredvalue + (lightMeter.readLightLevel()));
    delay(TIME_DELAY_FILTER);
  }

  lux = filteredvalue / MUESTRAS_FILTRO;

  #ifdef IDEBUGSERIAL
  Serial.print("Iluminancia: ");
  Serial.print(lux);
  Serial.println(" luxes");
  #endif

  if(!Btn_Press_Flag)
  {
    clientmqttc.publish((String(MQTT_TOPIC) + String(MQTT_PUBTOPIC_ESLR_LUX)), String(lux).c_str());
  }
}

/*******************************************************************************
 * FUNCTION: Fun_Leer_CapSoil
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
 * Fun que lee el sensor capacitivo de humedad de sustrato
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void Fun_Leer_CapSoil()
{
  uint8_t hum_maceta;

  hum_maceta = Sensor_Hum.Mtd_get_hum(MUESTRAS_FILTRO);
  #ifdef IDEBUGSERIAL
  Serial.print("Humedad en la Maceta: ");Serial.print(hum_maceta);Serial.println("%");
  #endif

  if(!Btn_Press_Flag)
  {
    MQTTPUBHUMEDAD(hum_maceta);
    Fun_Verificar_Regar(hum_maceta);
  }
}

/*******************************************************************************
 * FUNCTION: Fun_Rutina_Sensores
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
 * Fun que genera la rutina para leer los sensores con debounce
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void Fun_Rutina_Sensores()
{
  delay(100);
  Fun_Leer_CapSoil();
  delay(100);
  Fun_Leer_Lux();
  delay(100);
  Fun_Leer_INA3221();
  delay(100);
  Fun_Leer_Temp_AHT10();
  delay(100);
  Fun_Leer_Hum_AHT10();
  delay(100);
}
