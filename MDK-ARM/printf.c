#include <stdio.h>
#include "main.h"

extern UART_HandleTypeDef huart1;

int fputc(int ch,FILE *f)
{
    uint8_t temp[1]={ch};
    HAL_UART_Transmit(&huart1,temp,1,2); 
}
