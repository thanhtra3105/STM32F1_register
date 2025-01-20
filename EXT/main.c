#include "stm32f10x.h"
#include "string.h"
#include "stdio.h"

#define PORTA 2
#define PORTB 3
#define PORTC 4
#define PORTD 5

uint8_t flag;
// RS RW EN D0 D1 D2 D3 D4 D5 D6 D7 output a01234567
void lcd_config_gpio()
{
	RCC->APB2ENR |= 0x01 << PORTA;
	GPIOA->CRL &= ~0x00; // clear bit 0-31;
	GPIOA->CRL |= 0x33333333;	//50mhz  a0
	GPIOA->ODR &= ~0xFF;	// reset
	// config RS EN
	RCC->APB2ENR |= 0x01 << PORTB;
	GPIOB->CRL &= ~0xFF; // clear bit 0-31;
	GPIOB->CRL |= 0x33;	//50mhz  b0 b1
	GPIOB->ODR &= ~0x3;	// reset b0b1
	
	
}
void config_gpio_input()
{
	RCC->APB2ENR |= 0x01<<3; // portb
	GPIOB->CRL &= ~0x0F;
	GPIOB->CRL |= 0x01<<3;	// input b0
	GPIOB->ODR |= 0x01;	// pull up
}
void delay(uint32_t time)
{
	while(time--);
}
void lcd_cmd(uint8_t cmd)
{
	// RS EN: b0 b1
	GPIOB->ODR &= ~0x01;		// cmd mode
	GPIOB->ODR |= 0x01<<1;	//  en
	GPIOA->ODR &= ~0xFF;  // clear
	GPIOA->ODR |= cmd;
	delay(1000);
	GPIOB->ODR &= ~(0x01<<1);// unable

}
void lcd_data(uint8_t data)
{
	GPIOB->ODR |= 0x01;		// data mode
	GPIOB->ODR |= 0x01<<1;	//  en
	GPIOA->ODR &= ~0xFF;  // clear
	GPIOA->ODR |= data;
	delay(1000);
	GPIOB->ODR &= ~(0x01<<1);// unable
}
void lcd_init()
{
	lcd_config_gpio();
	lcd_cmd(0x0C);
	lcd_cmd(0x38);
	lcd_cmd(0x80);
}
void lcd_setcursor(uint8_t row,uint8_t col)
{
	if(row == 0)
	{
		lcd_cmd(0x80+col);
	}
	else{
		lcd_cmd(0xC0+col);
	}
}
void lcd_string(unsigned char *str)
{
	for(int i=0; i<strlen(str); i++)
	{
		lcd_data(str[i]);
	}
}
void lcd_number(uint16_t num)
{
	unsigned char num_str;
	sprintf(num_str,"%d",num);
	lcd_string(num_str);
}

/* EXTERAL INTERRUPT */
void EXTI_config()
{
	/*----Step----
	1.config clock use AFIO and config input PIN(pullup or pulldown)
	2.config AFIO_EXTICR
	3.Disable EXT_IMR
	4.Configure the Trigger Selection bits of the Interrupt lines (EXTI_RTSR and EXTI_FTSR)
	5.NVIC set priority and Enable IQR EXTI
	6.check EXTT_PR and clear it by code in EXTIn_IRQHandler() function
	*/
	// config clock
	RCC->APB2ENR |= 0x01;		// anable AFIO
	RCC->APB2ENR |= 0x01<<3;	// PB2 enable clock
	GPIOB->CRL &= ~(0xF00);	// clear PB2
	GPIOB->CRL |= 0x800;	// PB2 input
	GPIOB->ODR |= 0x01<<2;	// pullup
	
	AFIO->EXTICR[0] &= ~(0xF00);	// clear
	AFIO->EXTICR[0] |= (0x100);	// PB2
	
	EXTI->IMR |= 0x01<<2 ;	// 0:mask
	EXTI->RTSR &= ~(0x01<<2);	// disable rising trigger
	EXTI->FTSR |= 0x01<<2;	// enable falling
	NVIC_SetPriority (EXTI2_IRQn, 1);  // Set Priority
	NVIC_EnableIRQ (EXTI2_IRQn);
	
	
}
void EXTI2_IRQHandler(void) 
{
  if ((EXTI->PR>>2 & 0x01) ==1)
	{
		EXTI->PR |=0x01<<2;
		flag = 1;
	}
}


int main()
{
	EXTI_config();
	

	while(1)
	{
		if(flag == 1)
		{
			//.....
		}
	}
	
}