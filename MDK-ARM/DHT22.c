#include "DHT22.h"
#include "main.h"
#include "dwt_stm32_delay.h"

extern uint8_t dht_hum_l, dht_hum_h, dht_tmp_l, dht_tmp_h;

// PB1 comme entree
void dht_input(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/*Configure GPIO pin : PB1 */
  GPIO_InitStruct.Pin = DHT_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DHT_GROUP, &GPIO_InitStruct);
}

// PB1 comme sortie
void dht_output(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/*Configure GPIO pin : PB1 */
  GPIO_InitStruct.Pin = DHT_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DHT_GROUP, &GPIO_InitStruct);
}

// Commencer lire le dht22
void dht_start(void)
{
	dht_output();
	HAL_GPIO_WritePin(DHT_GROUP, DHT_PIN, GPIO_PIN_RESET);
	DWT_Delay_us(18000);	// Attendre pendant 18ms
	dht_input();
}

/**
  * @brief  Verifier si le dht22 est pre
  * @param  
  * @retval 1 si oui, 0 si non
  */
uint8_t dht_ready(void)
{
	uint8_t res = 0;
	
	DWT_Delay_us(40);
	if(!HAL_GPIO_ReadPin(DHT_GROUP, DHT_PIN)) {	// Verifier si le signal est a 0
		DWT_Delay_us(80);	// Attendre pour 80us
		if(HAL_GPIO_ReadPin(DHT_GROUP, DHT_PIN)) 	// Verifier si le signal est a 1
			res = 1;
		else
			return res;
	}
	while(HAL_GPIO_ReadPin(DHT_GROUP, DHT_PIN));	// Attendre jusqu'a signal est a 0, la lecture commence
	
	return res;
}

/**
  * @brief  Lire un octet de dht22
  * @param  
  * @retval L'octet lu
  */
uint8_t dht_read_byte(void)
{
	uint8_t res, i;
	
	for(i = 0; i < 8; i++) {
		while(!HAL_GPIO_ReadPin(DHT_GROUP, DHT_PIN));	// Attendre jusqu'a 1
		DWT_Delay_us(40);
		if(!HAL_GPIO_ReadPin(DHT_GROUP, DHT_PIN))	// Le signal est a 0
			res &= ~(1 << (7-i));	// Ecrire 0
		else	// Le signal est a 1
			res |= (1 << (7-i));	// Ecrire 1
		while(HAL_GPIO_ReadPin(DHT_GROUP, DHT_PIN));	// Attendre jusqu'a 0
	}
	
	return res;
}

// Lire la donee de dht22
void dht_read(void)
{
	dht_start();
	while(dht_ready());
	dht_hum_h = dht_read_byte();
	dht_hum_l = dht_read_byte();
	dht_tmp_h = dht_read_byte();
	dht_tmp_l = dht_read_byte();
}

// Retourner la valeur d'humidie de dht22, cette valeur est 10 fois plus grande
uint16_t dht_hum(void)
{
	return (dht_hum_h*256 + dht_hum_l);
}

// Retourner la valeur de tmperature de dht22, cette valeur est 10 fois plus grande
int16_t dht_tmp(void)
{
	uint8_t sign;
	
	sign = dht_hum_h & (1<<7);
	if(!sign)
		return (dht_hum_h*256 + dht_hum_l);
	return (-((dht_hum_h & (~(1<<7)))*256 + dht_hum_l));
}
