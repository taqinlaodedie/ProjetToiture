#ifndef DS18B20_H
#define DS18B20_H

#include "main.h"

#define DS18B20_GROUP GPIOA
#define DS18B20_PIN	  GPIO_PIN_0

#define READ() 			HAL_GPIO_ReadPin(DS18B20_GROUP, DS18B20_PIN)
#define OUT_SET() 	HAL_GPIO_WritePin(DS18B20_GROUP, DS18B20_PIN, GPIO_PIN_SET)
#define OUT_RESET()	HAL_GPIO_WritePin(DS18B20_GROUP, DS18B20_PIN, GPIO_PIN_RESET)

/**
  * @brief  Retourner la temperature
  * @param  
  * @retval 10 fois plus grand, entre -550 et 1250
  */
int16_t ds18b20_temp(void);

#endif
