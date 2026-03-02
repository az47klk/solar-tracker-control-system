/*
 * DS1307.c
 *
 *  Created on: Aug 18, 2025
 *      Author: az47klk
 */

#include "DS1307.h"

uint8_t dddderr;

uint8_t DectoBCD(uint8_t dec){
	uint8_t BCD;
	if (dec > 99) return 0;
	else {
		uint8_t ten = dec / 10;
		uint8_t one = dec % 10;
		BCD = ten << 4;
		BCD |= one;
		return BCD;
	}
}

uint8_t BCDtoDec(uint8_t bcd){
	uint8_t Dec;
	uint8_t ten = bcd >> 4;
	uint8_t one = bcd |= 0b1111;
	Dec = ten + one;
	return Dec;
}

uint8_t DS1307_ReadRegister(DS1307 *ds1307_addr, uint8_t RegisterAddress){
	uint8_t dataBuffer;
	HAL_StatusTypeDef state = HAL_I2C_Mem_Read(ds1307_addr->hi2c, (ds1307_addr->i2c_addr << 1), RegisterAddress, I2C_MEMADD_SIZE_8BIT, &dataBuffer, sizeof(dataBuffer),	HAL_MAX_DELAY);
	if (state == HAL_OK)
		return dataBuffer;
	else
		return 255;
}

HAL_StatusTypeDef DS1307_WriteRegister(DS1307 *ds1307_addr, uint8_t RegisterAddress, uint8_t data){
	HAL_StatusTypeDef buffer = HAL_I2C_Mem_Write(ds1307_addr->hi2c, (ds1307_addr->i2c_addr << 1), RegisterAddress, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
	return buffer;
}

HAL_StatusTypeDef DS1307_Init(DS1307 *ds1307_pointer, I2C_HandleTypeDef *hi2c, uint8_t DeviceAddress){
	ds1307_pointer->i2c_addr = DeviceAddress;
	ds1307_pointer->hi2c = hi2c;
	if (DS1307_WriteRegister(ds1307_pointer, 0x07, 0b00000000) == HAL_OK) return HAL_OK;
	else return HAL_ERROR;
}

HAL_StatusTypeDef DS1307_SetTimeDate(DS1307 *ds1307_addr, uint16_t _year, uint8_t _month, uint8_t _day, uint8_t dayOfWeek, uint8_t _hour, uint8_t _minute, uint8_t _second, DS1307_HourFormat hourFormat){
	if (_month > 12 || _day > 31 || _hour > 24 || _minute > 60 || _second > 60) return HAL_BUSY;  // if date format is wrong, return error

	uint8_t __year = _year - DS1307_ZERO_YEAR;
	uint8_t year = DectoBCD(__year);
	uint8_t month = DectoBCD(_month);
	uint8_t day = DectoBCD(_day);
	uint8_t hour = DectoBCD(_hour);
	if (hourFormat == DS1307_HOUR_FORMAT_12) {
		hour |= (1 << 5); // 12-hour format
	}
	else {
		hour |= (0 << 5); // 24-hour format
	}

	uint8_t minute = DectoBCD(_minute);
	uint8_t second = DectoBCD(_second);

	dddderr =1;
	if (DS1307_WriteRegister(  ds1307_addr, 0x00, second     ) != HAL_OK) return HAL_ERROR; // set second
	dddderr =2;
	if (DS1307_WriteRegister(  ds1307_addr, 0x05, month      ) != HAL_OK) return HAL_ERROR; // set month
	dddderr =3;
	if (DS1307_WriteRegister(  ds1307_addr, 0x04, day        ) != HAL_OK) return HAL_ERROR; // set day
	dddderr =4;
	if (DS1307_WriteRegister(  ds1307_addr, 0x03, dayOfWeek  ) != HAL_OK) return HAL_ERROR; // set day of week
	dddderr =5;
	if (DS1307_WriteRegister(  ds1307_addr, 0x02, hour       ) != HAL_OK) return HAL_ERROR; // Set hour
	dddderr =6;
	if (DS1307_WriteRegister(  ds1307_addr, 0x01, minute     ) != HAL_OK) return HAL_ERROR; // set minute
	dddderr =7;
	if (DS1307_WriteRegister(  ds1307_addr, 0x06, year       ) != HAL_OK) return HAL_ERROR; // set year
	return HAL_OK;
}

uint8_t DS1307_GetSecond(DS1307 *ds1307_addr){
	uint8_t buffer = DS1307_ReadRegister(ds1307_addr, 0x00);
	uint8_t buffer_ten = buffer >> 4;
	uint8_t buffer_zero = buffer & 0b1111;
	return (buffer_ten*10 + buffer_zero);
}

uint8_t DS1307_GetMinute(DS1307 *ds1307_addr){
	uint8_t buffer = DS1307_ReadRegister(ds1307_addr, 0x01);
	uint8_t buffer_ten = buffer >> 4;
	uint8_t buffer_zero = buffer & 0b1111;
	return (buffer_ten*10 + buffer_zero);
}

uint8_t DS1307_GetHour(DS1307 *ds1307_addr){
	uint8_t buffer = DS1307_ReadRegister(ds1307_addr, 0x02);
	uint8_t buffer_ten = buffer >> 4;
	uint8_t buffer_zero = buffer & 0b1111;
	return (buffer_ten*10 + buffer_zero);
}

uint8_t DS1307_GetDayOfWeek(DS1307 *ds1307_addr){
	uint8_t buffer = DS1307_ReadRegister(ds1307_addr, 0x03);
	return buffer;
}

uint8_t DS1307_GetDay(DS1307 *ds1307_addr){
	uint8_t buffer = DS1307_ReadRegister(ds1307_addr, 0x04);
	uint8_t buffer_ten = buffer >> 4;
	uint8_t buffer_zero = buffer & 0b1111;
	return (buffer_ten*10 + buffer_zero);
}

uint8_t DS1307_GetMonth(DS1307 *ds1307_addr){
	uint8_t buffer = DS1307_ReadRegister(ds1307_addr, 0x05);
	uint8_t buffer_ten = buffer >> 4;
	uint8_t buffer_zero = buffer & 0b1111;
	return (buffer_ten*10 + buffer_zero);
}

uint16_t DS1307_GetYear(DS1307 *ds1307_addr){
	uint8_t buffer = DS1307_ReadRegister(ds1307_addr, 0x06);
	uint8_t buffer_ten = buffer >> 4;
	uint8_t buffer_zero = buffer & 0b1111;
	return  DS1307_ZERO_YEAR + (buffer_ten*10 + buffer_zero);
}

DS1307_TimeDateTypeDef DS1307_GetTimeDate(DS1307 *ds1307_addr){
	DS1307_TimeDateTypeDef buffer;
	buffer.Second = DS1307_GetSecond(ds1307_addr);
	buffer.Minute = DS1307_GetMinute(ds1307_addr);
	buffer.Hour   = DS1307_GetHour(ds1307_addr);
	buffer.DayOfWeek = DS1307_GetDayOfWeek(ds1307_addr);
	buffer.Day    = DS1307_GetDay(ds1307_addr);
	buffer.Month  = DS1307_GetMonth(ds1307_addr);
	buffer.Year   = DS1307_GetYear(ds1307_addr);
	return buffer;
}

