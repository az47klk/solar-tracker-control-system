/*
 * DS1307.c
 *
 *  Created on: Aug 18, 2025
 *      Author: az47klk
 */

#ifndef __DS1307_H
#define __DS1307_H

#include "stm32f1xx_hal.h"


#include "string.h"

#define DS1307_ZERO_YEAR 2000

extern uint8_t dddderr;

typedef struct {
	uint8_t Second;
	uint8_t Minute;
	uint8_t Hour;
	uint8_t DayOfWeek;
	uint8_t Day;
	uint8_t Month;
	uint16_t Year;
} DS1307_TimeDateTypeDef;

typedef struct {
	I2C_HandleTypeDef *hi2c;
	uint8_t i2c_addr;
} DS1307;

typedef enum{
	DS1307_HOUR_FORMAT_24 = 0,
	DS1307_HOUR_FORMAT_12 = 1
} DS1307_HourFormat;

// Write and Read Functions
uint8_t DS1307_ReadRegister(DS1307 *, uint8_t);
HAL_StatusTypeDef DS1307_WriteRegister(DS1307 *, uint8_t, uint8_t);

// Initalization Functions
HAL_StatusTypeDef DS1307_Init(DS1307*, I2C_HandleTypeDef*, uint8_t);
HAL_StatusTypeDef DS1307_SetTimeDate(DS1307*, uint16_t , uint8_t , uint8_t, uint8_t , uint8_t , uint8_t , uint8_t, DS1307_HourFormat);

// Get Time and Date Functions
uint8_t DS1307_GetSecond(DS1307*);
uint8_t DS1307_GetMinute(DS1307*);
uint8_t DS1307_GetHour(DS1307*);
uint8_t DS1307_GetDay(DS1307*);
uint8_t DS1307_GetDayOfWeek(DS1307*);
uint8_t DS1307_GetMonth(DS1307*);
uint16_t DS1307_GetYear(DS1307*);

DS1307_TimeDateTypeDef DS1307_GetTimeDate(DS1307*);

#endif
