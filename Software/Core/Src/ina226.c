/*
 * ina226.cpp
 *
 *  Created on: Aug 4, 2025
 *      Author: az47klk
 */


#include "ina226.h"
#include <math.h>

void INA226_Set_Address(INA226 *INA226_P, I2C_HandleTypeDef *hi2c_tmp, uint8_t A1, uint8_t A0){
	uint8_t newAddr = 0b1000000;
	newAddr |= (A1 << 2);
	newAddr |= (A0);
	INA226_P->INA226_Address = newAddr;
	INA226_P->hi2c = hi2c_tmp;
}

int INA226_WriteRegister(INA226 *INA226_P, uint8_t reg, uint16_t value) {
    uint8_t buf[2] = {value >> 8, value & 0xFF};
    if (HAL_I2C_Mem_Write(INA226_P->hi2c, INA226_P->INA226_Address << 1, reg, I2C_MEMADD_SIZE_8BIT, buf, 2, HAL_MAX_DELAY) != HAL_OK)
    	return -1;
    else return 0;
}

uint16_t INA226_ReadRegister(INA226 *INA226_P, uint8_t reg) {
    uint8_t buf[2];
    HAL_I2C_Mem_Read(INA226_P->hi2c, INA226_P->INA226_Address << 1, reg, I2C_MEMADD_SIZE_8BIT, buf, 2, HAL_MAX_DELAY);
    return (buf[0] << 8) | buf[1];
}

void INA226_Config(INA226 *INA226_P,  INA226_CONF_SHUNT_VOLTAGE ShuntVoltageEnable,  INA226_CONF_BUS_VOLTAGE BusVoltageEnable, INA226_CONF_TRIG Triggered,  INA226_CONF_CONV_TIME ShuntVoltConversionTime,  INA226_CONF_CONV_TIME BusVoltConversionTime, INA226_CONF_AVG AveragingMode){
	uint16_t ina226_config = 0x0000000000000000;

	ina226_config |= ShuntVoltageEnable;
	ina226_config |= (BusVoltageEnable << 1);
	ina226_config |= (Triggered << 2);

	ina226_config |= (ShuntVoltConversionTime << 3);
	ina226_config |= (BusVoltConversionTime << 6);
	ina226_config |= (AveragingMode << 9);

	INA226_WriteRegister(INA226_P, INA226_REG_CONF, ina226_config);
}

float INA226_ReadShuntVoltage(INA226 *INA226_P){
	uint16_t buffer = INA226_ReadRegister(INA226_P, INA226_REG_SHNT);
	int16_t signedBuffer = (int16_t)buffer;
	float volt = signedBuffer * 2.5/1000000;
	return volt;
}

float INA226_ReadBusVoltage(INA226 *INA226_P){
	uint16_t buffer = INA226_ReadRegister(INA226_P, INA226_REG_BUS);
	int16_t signedBuffer = (int16_t)buffer;
	float volt = signedBuffer * 1.25/1000;
	return volt;
}

float INA226_ReadPower(INA226 *INA226_P){
	uint16_t rawPower = INA226_ReadRegister(INA226_P, INA226_REG_PWR);
	float realPower = 25*rawPower*INA226_P->current_lsb;
	return realPower;
}

float INA226_ReadCurrent(INA226 *INA226_P){
	uint16_t rawCurrent = INA226_ReadRegister(INA226_P, INA226_REG_CUR);
	float realCurrent = rawCurrent*INA226_P->current_lsb;
	return realCurrent;
}

void INA226_SetAlarmMode(INA226* INA226_P, uint16_t modes[]){
	int modeNums = sizeof(modes) / sizeof(modes[0]);
	uint16_t template = 0b0000000000000000;

	for (int mode = 0; mode < modeNums; mode++){
		template |= modes[mode];
	}

	INA226_WriteRegister(INA226_P, INA226_REG_MSK, template);
}

void INA226_SetAlarmPin(INA226 *INA226_P, GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN_x){
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if (GPIOx == GPIOA){
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if(GPIOx == GPIOB){
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}
	else if(GPIOx == GPIOC){
		__HAL_RCC_GPIOC_CLK_ENABLE();
	}
	else if(GPIOx == GPIOD){
		__HAL_RCC_GPIOD_CLK_ENABLE();
	}
	else if(GPIOx == GPIOE){
		__HAL_RCC_GPIOE_CLK_ENABLE();
	}
	else if(GPIOx == GPIOF){
		__HAL_RCC_GPIOF_CLK_ENABLE();
	}
	else if(GPIOx == GPIOG){
		__HAL_RCC_GPIOG_CLK_ENABLE();
	}

	INA226_P->GPIOx = GPIOx;
	INA226_P->GPIO_PIN_x = GPIO_PIN_x;

	GPIO_InitStruct.Pin = GPIO_PIN_x;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;

	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}



void INA226_SetCalibrationReg(INA226 *INA226_P, float max_current, float r_shunt){
	uint16_t calibration_val;
	float current_lsb = max_current / 32768;
	INA226_P->current_lsb = current_lsb;
	calibration_val = trunc( 0.00512 / (current_lsb * r_shunt));

	INA226_WriteRegister(INA226_P, INA226_REG_CLB, calibration_val);
}

void INA226_SetAlarmLimitShuntVoltage(INA226 *INA226_P, float limit_value){
	uint16_t raw_val = trunc(limit_value / (2.5*0.000001));
	INA226_WriteRegister(INA226_P , INA226_REG_ALIM, raw_val);
}

void INA226_SetAlarmLimitBusVoltage(INA226 *INA226_P, float limit_value){
	uint16_t raw_val = trunc(limit_value / (1.25*0.001));
	INA226_WriteRegister(INA226_P , INA226_REG_ALIM, raw_val);
}


void INA226_SetAlarmLimitCurrent(INA226 *INA226_P, float limit_value){
	uint16_t raw_val = trunc(limit_value / INA226_P->current_lsb);
	INA226_WriteRegister(INA226_P , INA226_REG_ALIM, raw_val);
}

void INA226_SetAlarmLimitPower(INA226 *INA226_P, float limit_value){
	uint16_t raw_val = trunc(limit_value / (25*INA226_P->current_lsb));
	INA226_WriteRegister(INA226_P , INA226_REG_ALIM, raw_val);
}

bool INA226_ReadAlarmPin(INA226 *INA226_P){
	if (HAL_GPIO_ReadPin(INA226_P->GPIOx, INA226_P->GPIO_PIN_x) == GPIO_PIN_SET)
		return true;
	else return false;
}
