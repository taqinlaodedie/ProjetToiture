#include "user_delay.h"
#include "main.h"

void delay_us(uint32_t tim)
{
	for(uint32_t i = 0; i < tim; i++) {
		uint8_t delay = 80;
    while (delay--);
	}
}
