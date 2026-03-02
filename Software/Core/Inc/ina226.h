/*
 * ina226.cpp
 *
 *  Created on: Aug 4, 2025
 *      Author: az47klk
 */

#ifndef __INA226_H
#define __INA226_H

#include "stm32f1xx_hal.h"
#include <stdbool.h>

// pin definitions
#define INA226_PIN_GND      0
#define INA226_PIN_VS       1
#define INA226_PIN_SDA      2
#define INA226_PIN_SCL      3

// registers
#define INA226_REG_CONF     0x00
#define INA226_REG_SHNT     0x01
#define INA226_REG_BUS      0x02
#define INA226_REG_PWR      0x03
#define INA226_REG_CUR      0x04
#define INA226_REG_CLB      0x05
#define INA226_REG_MSK      0x06
#define INA226_REG_ALIM     0x07
#define INA226_REG_MID      0xFE
#define INA226_REG_DID      0xFF

typedef struct {
	uint8_t INA226_Address;
	I2C_HandleTypeDef *hi2c;
	float current_lsb;

	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_PIN_x;

} INA226;

#define default_INA226_Config(a) INA226_Config(a, INA226_SHUNT_VOLTAGE_ENABLE, INA226_BUS_VOLTAGE_ENABLE, INA226_CONF_CONTINIOUS, INA226_CONF_CONV_TIME_8_244MS, INA226_CONF_CONV_TIME_8_244MS, INA226_CONF_AVG_NUM_1);


#define INA226_ALARM_SHUNT_OVER_VOLTAGE  (1 << 15)
#define INA226_ALARM_SHUNT_UNDER_VOLTAGE (1 << 14)
#define INA226_ALARM_BUS_OVER_VOLTAGE    (1 << 13)
#define INA226_ALARM_BUS_UNDER_VOLTAGE   (1 << 12)
#define INA226_ALARM_POWER_OVERLOAD      (1 << 11)
#define INA226_ALARM_CONVERSION_READY    (1 << 10)
#define INA226_ALARM_ALERT_FUNC_FLAG     (1 << 4)
#define INA226_ALARM_CONV_READY_FLAG     (1 << 3)
#define INA226_ALARM_MATH_OVERFLOW_FLAG  (1 << 2)
#define INA226_ALARM_ALERT_POLARITY      (1 << 1)
#define INA226_ALARM_ALERT_LATCH_EN      1


// avg config
typedef enum {
      INA226_CONF_AVG_NUM_1    = 0b000,
      INA226_CONF_AVG_NUM_4    = 0b001,
      INA226_CONF_AVG_NUM_16   = 0b010,
      INA226_CONF_AVG_NUM_64   = 0b011,
      INA226_CONF_AVG_NUM_128  = 0b100,
      INA226_CONF_AVG_NUM_256  = 0b101,
      INA226_CONF_AVG_NUM_512  = 0b110,
      INA226_CONF_AVG_NUM_1024 = 0b111,
} INA226_CONF_AVG;



//convertion time config
typedef enum {
	  INA226_CONF_CONV_TIME_140US   = 0b000,
	  INA226_CONF_CONV_TIME_204US   = 0b001,
	  INA226_CONF_CONV_TIME_332US   = 0b010,
	  INA226_CONF_CONV_TIME_588US   = 0b011,
	  INA226_CONF_CONV_TIME_1_1MS   = 0b100,
	  INA226_CONF_CONV_TIME_2_116MS = 0b101,
	  INA226_CONF_CONV_TIME_4_156MS = 0b110,
	  INA226_CONF_CONV_TIME_8_244MS = 0b111
} INA226_CONF_CONV_TIME;

// mode config
typedef enum {
	INA226_CONF_TRIGGERED = 0,
	INA226_CONF_CONTINIOUS = 1
} INA226_CONF_TRIG;

typedef enum {
	INA226_BUS_VOLTAGE_DISABLE = 0,
	INA226_BUS_VOLTAGE_ENABLE = 1
} INA226_CONF_BUS_VOLTAGE;


typedef enum {
	INA226_SHUNT_VOLTAGE_DISABLE = 0,
	INA226_SHUNT_VOLTAGE_ENABLE = 1
} INA226_CONF_SHUNT_VOLTAGE;


void INA226_Set_Address(INA226*, I2C_HandleTypeDef*, uint8_t, uint8_t);

int INA226_WriteRegister(INA226*, uint8_t, uint16_t);
uint16_t INA226_ReadRegister(INA226*, uint8_t);

void INA226_Config(INA226 *INA226_P, INA226_CONF_SHUNT_VOLTAGE , INA226_CONF_BUS_VOLTAGE, INA226_CONF_TRIG, INA226_CONF_CONV_TIME, INA226_CONF_CONV_TIME, INA226_CONF_AVG);

float INA226_ReadShuntVoltage(INA226*);
float INA226_ReadBusVoltage(INA226*);
float INA226_ReadPower(INA226*);
float INA226_ReadCurrent(INA226*);


void INA226_SetAlarmMode(INA226*, uint16_t[]);
void INA226_SetAlarmPin(INA226*, GPIO_TypeDef *, uint16_t);
void INA226_SetCalibrationReg(INA226 *, float, float);

void INA226_SetAlarmLimitShuntVoltage(INA226 *, float);
void INA226_SetAlarmLimitBusVoltage(INA226 *, float);
void INA226_SetAlarmLimitCurrent(INA226 *, float);
void INA226_SetAlarmLimitPower(INA226 *, float);

bool INA226_ReadAlarmPin(INA226*);


#endif
