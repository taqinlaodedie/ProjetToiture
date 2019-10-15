#include "ds18b20.h"
#include "main.h"

#define READ() 			HAL_GPIO_ReadPin(DS18B20_GROUP, DS18B20_PIN)
#define OUT_SET() 	HAL_GPIO_WritePin(DS18B20_GROUP, DS18B20_PIN, GPIO_PIN_SET)
#define OUT_RESET()	HAL_GPIO_WritePin(DS18B20_GROUP, DS18B20_PIN, GPIO_PIN_RESET)

// PA0 comme entree
void ds18b20_input(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = DS18B20_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DS18B20_GROUP, &GPIO_InitStruct);
}


// PA0 comme sortie
void ds18b20_output(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = DS18B20_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(DS18B20_GROUP, &GPIO_InitStruct);
}

/**
  * @brief  Reset the ds18b20
  * @param  
  * @retval 1 si success, 0 si non
  */
uint8_t ds18b20_reset(void)
{
	ds18b20_output();
	OUT_RESET();	// pin = 0
	DWT_Delay_us(480);	// Attendre plus de 480us
	ds18b20_input();
	DWT_Delay_us(80);		// Attendre pendant 80us
	if(!READ()) {	// Lire la reponse
		DWT_Delay_us(400);
		return 0;
	}
	else {
		DWT_Delay_us(400);
		return 1;
	}
}

// Ecrire un bit vers ds18b20
void ds18b20_write_bit(uint8_t bit)
{
	ds18b20_output();
	if(bit & 1){
		OUT_RESET();
		DWT_Delay_us(1);
		OUT_SET();
		DWT_Delay_us(60);
	}
	else {
		OUT_RESET();
		DWT_Delay_us(60);
		OUT_SET();
		DWT_Delay_us(1);
	}
}

// Lire un bit de ds18b20
uint8_t ds18b20_read_bit(void)
{
	uint8_t res;
	
	ds18b20_output();
	OUT_RESET();
	DWT_Delay_us(2);
	OUT_SET();
	ds18b20_input();
	DWT_Delay_us(12);
	if(READ())
		res = 1;
	else 
		res = 0;
	DWT_Delay_us(50);
	
	return res;
}

// Ecrire un octet
void ds18b20_write_byte(uint8_t dat)
{
	uint8_t i, test;
	
	ds18b20_output();
	for(i = 0; i < 8; i++) {
		test = dat & 0x01;
		dat = dat >> 1;
		if(test) {	// Ecrire 1
			OUT_RESET();
			DWT_Delay_us(2);
			OUT_SET();
			DWT_Delay_us(60);
		}
		else {	// Ecrire 0
			OUT_RESET();
			DWT_Delay_us(60);
			OUT_SET();
			DWT_Delay_us(2);
		}
	}
}

// Lire un octet
uint8_t ds18b20_read_byte(void)
{
	uint8_t i, bit, res;
	
  res = 0;
  for(i = 1; i <= 8; i++) {
		bit = ds18b20_read_bit();
    res = (bit<<7) | (res>>1);
  }                           
    
	return res;
}

// Commencer la convertion de temperature
void ds18b20_convert(void)
{
	if(!ds18b20_reset())
		Error_Handler();
	ds18b20_write_byte(0xcc);	// Skip ROM
	ds18b20_write_byte(0x44);	// Convertir
}

/**
  * @brief  Retourner la temperature
  * @param  
  * @retval 10 fois plus grand, entre -550 et 1250
  */
int16_t ds18b20_temp(void)
{
	uint8_t sign, tl, th;
	int16_t temp;
	
	ds18b20_convert();
	if(!ds18b20_reset())
		Error_Handler();
	HAL_Delay (800);
	ds18b20_write_byte(0xcc);	// Skip ROM
	ds18b20_write_byte(0xbe);	// Commencer a lire
	th = ds18b20_read_byte();
	tl = ds18b20_read_byte();
	if(th > 7) {
		th = ~th;
		tl = ~tl;
		sign = 0;	// Temperature negative
	}
	else
		sign = 1;	// Temperature positive
	temp = th;
	temp <<= 8;
	temp |= tl;
	temp = (int16_t)((float)temp * 0.625);
	if(sign)
		return temp;
	
	return -temp;
}
