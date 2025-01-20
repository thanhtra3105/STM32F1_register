#include "stm32f10x.h"
#include "string.h"
#include "stdio.h"


/* systick */

void delay_ms(uint16_t time)
{
	while(time)
	{
		SysTick->LOAD = 8000-1;
		SysTick->VAL = 0;
		SysTick->CTRL =0x05;	// AHB
		while(!(SysTick->CTRL&(0x01<<16)))
		{
		}
		time--;
	}
}
int main()
{
	RCC->APB2ENR |= 0x01<<4;
	GPIOC->CRH &=~(0x0F<<20);
	GPIOC->CRH |= 0x03<<20;
	GPIOC->ODR &= ~(0x01<<13);
	while(1)
	{
		GPIOC->ODR |= 0x01<<13;
		delay_ms(1);
		GPIOC->ODR &= ~(0x01<<13);
		delay_ms(1);
		
	}
	
}