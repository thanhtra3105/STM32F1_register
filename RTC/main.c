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
	GPIOA->CRL &= 0x00; // clear bit 0-31;
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

void config_rtc()
{
	RCC->APB1ENR |= 0x03<<27;	// set enable BKUEN va PWREN 
	PWR->CR |= 0x100;
	
	RCC->BDCR |= 0x01; // external clock	32k ON
	while(RCC->BDCR>>1 &(0x01) != 1);	// wait for LSERDY=1(external 32k ready)
	RCC->BDCR |= 0x01<<8;
	RCC->BDCR |= 0x01<<15;	//enable RTC clock

	while((RTC->CRL>>5 &0x01)!=1); // RTOFF doi nhay len 1
	RTC->CRL |= 0x10;	//Set the CNF bit to enter configuration mode
	RTC->PRLH =0x00;
	RTC->PRLL = 0x7FFF+1;	//1s dau ra
	RTC->CRL &= ~0x10;	//clearerr CNF
	while((RTC->CRL>>5 &0x01)!=1);
	
	while((RTC->CRL>>5 &0x01)!=1); // RTOFF doi nhay len 1
	RTC->CRL |= 0x10;	//Set the CNF bit to enter configuration mode
	RTC->ALRH = 0x00;
	RTC->ALRL = 0x01;	//5s alarm
	RTC->CRL &= ~0x10;	//clearerr CNF
	while((RTC->CRL>>5 &0x01)!=1); // doi set
}

int main()
{
	config_rtc();
	int second= 0;
	int minute =0;
	int hour = 0;
	int i=0;
	while(1)
	{
		
		if(RTC->CRL&0x01 == 1)
		{
			second++;
			if(second >59)
			{
				second = 0;
				minute++;
				if(minute>59)
				{
					minute=0;
					hour++;
				}
			}
			
			RTC->CRL &=~0x01;
		}
		
		lcd_setcursor(0,0);
		lcd_number(hour);
		lcd_setcursor(0,3);
		lcd_number(minute);
		lcd_setcursor(0,6);
		lcd_number(second);
		
		
	}
	
}