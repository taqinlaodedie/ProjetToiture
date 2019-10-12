#ifndef DHT22_H
#define DHT22_H

#include "main.h"

#define DHT_GROUP GPIOB
#define DHT_PIN   GPIO_PIN_1

// Lire la donee de dht22
void dht_read(void);
	
// Retourner la valeur d'humidie de dht22, cette valeur est 10 fois plus grande
uint16_t dht_hum(void);

// Retourner la valeur de tmperature de dht22, cette valeur est 10 fois plus grande
int16_t dht_tmp(void);

#endif
