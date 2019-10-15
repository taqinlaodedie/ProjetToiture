#include "main.h"
#include "sigfox.h"
#include <stdio.h>
#include "printf.h"

extern UART_HandleTypeDef huart1;
extern uint16_t air_hum, soil_hum;
extern int16_t air_temp, soil_temp;
extern RGB rgb;
extern uint8_t lux;

// Envoyer toutes les parametres mesurees par les capteurs
void sigfox_send(void)
{
	printf("AT$SF=%04x%04x%04x%04x%02x%02x%02x%02x\r", air_temp, air_hum, soil_temp, soil_hum, 
		lux, (uint8_t)rgb.R, (uint8_t)rgb.G, (uint8_t)rgb.B);
}
