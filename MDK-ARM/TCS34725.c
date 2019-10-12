/*****************************************************************************
* | File      	:   TCS34725.c
* | Author      :   Waveshare team
* | Function    :   TCS34725 driver
* | Info        :
*                Tcs34725 initialization, reading data, writing data 
                 and data processing
*----------------
* |	This version:   V1.0
* | Date        :   2019-01-16
* | Info        :

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "TCS34725.h"

TCS34725IntegrationTime_t IntegrationTime_t = TCS34725_INTEGRATIONTIME_700MS;

/******************************************************************************
function:   Write a byte to TCS34725
parameter	:
        add : Register address
        data: Written data
******************************************************************************/
static void TCS34725_WriteByte(UBYTE add, UBYTE data)
{
    //Note: remember to add this when users write their own
    //Responsible for not finding the register, 
    //refer to the data sheet Command Register CMD(Bit 7)
    add = add | TCS34725_CMD_BIT;
    DEV_I2C_WriteByte(add, data);
}

/******************************************************************************
function:   Read a byte to TCS34725
parameter	:
        add : Register address
******************************************************************************/
static UBYTE TCS34725_ReadByte(UBYTE add)
{
    add = add | TCS34725_CMD_BIT;
    return DEV_I2C_ReadByte(add);
}

/******************************************************************************
function:   Read a word to TCS34725
parameter	:
        add : Register address
        data: Written data
******************************************************************************/
static UWORD TCS34725_ReadWord(UBYTE add)
{

    add = add | TCS34725_CMD_BIT;
    return DEV_I2C_ReadWord(add);
}

/******************************************************************************
function:   
        TCS34725 wake up
******************************************************************************/
static void TCS34725_Enable(void)
{
    TCS34725_WriteByte(TCS34725_ENABLE, TCS34725_ENABLE_PON);
    DEV_Delay_ms(3);
    TCS34725_WriteByte(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
    DEV_Delay_ms(3);  
}

/******************************************************************************
function:   
        TCS34725 Sleep
******************************************************************************/
void TCS34725_Disable(void)
{
    /* Turn the device off to save power */
    UBYTE reg = 0;
    reg = TCS34725_ReadByte(TCS34725_ENABLE);
    TCS34725_WriteByte(TCS34725_ENABLE, reg & ~(TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN));
}

/******************************************************************************
function:   TCS34725 Set Integration Time
parameter	:
        time: Integration Time Reference "TCS34725.h" Enumeration Type
******************************************************************************/
void TCS34725_Set_Integration_Time(TCS34725IntegrationTime_t time)
{
    /* Update the timing register */
    TCS34725_WriteByte(TCS34725_ATIME, time);
    IntegrationTime_t = time;
}

/******************************************************************************
function:   TCS34725 initialization
parameter	:
        gain: gain Reference "TCS34725.h" Enumeration Type
        it  : Integration Time Reference "TCS34725.h" Enumeration Type
******************************************************************************/
UBYTE  TCS34725_Init(void)
{
	UBYTE ID = 0;
	DEV_Set_I2CAddress(TCS34725_ADDRESS);
	ID = TCS34725_ReadByte(TCS34725_ID);
	if(ID != 0x44 && ID != 0x4D){
			return 1;
	}
	//Set the integration time and gain
	TCS34725_Set_Integration_Time(TCS34725_INTEGRATIONTIME_154MS);	

	IntegrationTime_t = TCS34725_INTEGRATIONTIME_154MS;
	
	TCS34725_Enable();

	return 0;
}

/******************************************************************************
function:   TCS34725 Read RGBC data
parameter	:
     R,G,B,C: RGBC Numerical value,Is a pointer
******************************************************************************/
RGB TCS34725_Get_RGBData()
{
    RGB temp;
    temp.C = TCS34725_ReadWord(TCS34725_CDATAL | TCS34725_CMD_Read_Word);
    temp.R = TCS34725_ReadWord(TCS34725_RDATAL | TCS34725_CMD_Read_Word);
    temp.G = TCS34725_ReadWord(TCS34725_GDATAL | TCS34725_CMD_Read_Word);
    temp.B = TCS34725_ReadWord(TCS34725_BDATAL | TCS34725_CMD_Read_Word);

    switch (IntegrationTime_t){
        case TCS34725_INTEGRATIONTIME_2_4MS:
              DEV_Delay_ms(3);
              break;
        case TCS34725_INTEGRATIONTIME_24MS:
              DEV_Delay_ms(24);
              break;
        case TCS34725_INTEGRATIONTIME_50MS:
              DEV_Delay_ms(50);
              break;
        case TCS34725_INTEGRATIONTIME_101MS:
              DEV_Delay_ms(101);
              break;
        case TCS34725_INTEGRATIONTIME_154MS:
              DEV_Delay_ms(154);
              break;
        case TCS34725_INTEGRATIONTIME_700MS:
              DEV_Delay_ms(700);
              break;
    }
    return temp;
}
