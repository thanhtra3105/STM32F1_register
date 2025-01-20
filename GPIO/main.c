#include "stm32f10x.h"

void delay(uint32_t time)
{
	while(time--);
}

/* config with standard library */
/*
void GPIO_Config(void)
{ 
   GPIO_InitTypeDef  GPIO_InitStructure; 
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
   //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure); 
}

int main()
{
	GPIO_Config();
	while(1)
	{
		
		
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0U)
		{
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
		}
		else
			GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		
		
	}
}
*/


/* config with register */
void gpio_config_input()
{
	// input PORTB PA0 
	/*RCC->APB2ENR |= 0x01<<2;	// porta
	GPIOA->CRL &= ~(0x0f);	// clear bit 0-3
	GPIOA->CRL |= 0x01<<3;	// 1000 input
	GPIOA->ODR |= (0x01); // keo len
	
	// input PORTB PA1 
	//RCC->APB2ENR |= 0x01<<3;
	GPIOA ->CRL &= ~(0x0f<<4); 	// clear bit 4-7
	GPIOA ->CRL |= 0x01<<7;
	GPIOA->ODR |= 0x01<<1; */
	
	
	// config PA0 input
	RCC->APB2ENR |= 0x01<<2;	// PORTA
	GPIOA->CRL &= ~0x0F;	// clear bit 0-3;
	GPIOA->CRL |= 0x01<<3;	// input 1000
	GPIOA->ODR |= 0x01;
}
uint8_t maled[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void gpio_config_output()
{
	RCC->APB2ENR |= 0x01<<4;	// portc
	GPIOC->CRH &= ~(0x0f<<20);	// clear bit 20-23  
	GPIOC->CRH |= 0x03<<20;	//OUTPUT max speeed
	GPIOC->CRH &= ~(0x03 <<22);
	GPIOC->ODR &= ~(0x01<<13);	// reset
	// config portb output
	RCC->APB2ENR |= 0x01<<3;
	GPIOB->CRL = 0x33333333;	// output max speed 
	// pb8-pb11 output
	GPIOB->CRH &= ~(0xFFFF); 	//clear 16 bit 0-15
	GPIOB->CRH |= 0x3333;	// output max speed 
	GPIOB->ODR |= 0xFF<<8;	// set
}

int i=0;
void seg_number(uint16_t num)	// vd 1234
{
	int temp = num%10;	//4	pb11 reset
	
	GPIOB->ODR &= ~(0x01<<11);	// reset pb11
	GPIOB->ODR &= ~(0xFF); // clear 8bit 0-7
	GPIOB->ODR |= maled[temp];
	delay(100);
	GPIOB->ODR |= (0x01<<11);	// set pb11
	
	num/=10;	//123
	temp = num%10;	//3
	GPIOB->ODR &= ~(0x01<<10);	// reset pb10
	GPIOB->ODR &= ~(0xFF); // clear 8bit 0-7
	GPIOB->ODR |= maled[temp];
	delay(100);
	GPIOB->ODR |= (0x01<<10);	// set pb11
	
	num/=10;	//12
	temp = num%10;	//2
	GPIOB->ODR &= ~(0x01<<9);	// reset pb9
	GPIOB->ODR &= ~(0xFF); // clear 8bit 0-7
	GPIOB->ODR |= maled[temp];
	delay(100);
	GPIOB->ODR |= (0x01<<9);	// set pb11
	
	num/=10;	//1
	temp = num%10;	//1
	GPIOB->ODR &= ~(0x01<<8);	// reset pb8
	GPIOB->ODR &= ~(0xFF); // clear 8bit 0-7
	GPIOB->ODR |= maled[temp];
	delay(100);
	GPIOB->ODR |= (0x01<<8);	// set pb11
	
}
int main()
{
	gpio_config_output();
	gpio_config_input();
	while(1)
	{
		//GPIO_SetBits(GPIOC, GPIO_Pin_13);
		/*if((GPIOA->IDR&(0x01<<1)) == 0)
		{
			while((GPIOA->IDR&0x01<<1) == 0);
			
			GPIOC->ODR = ~GPIOC->ODR;
		}*/
		if((GPIOA->IDR&(0x01)) == 0)
		{
			while((GPIOA->IDR&(0x01)) == 0);
			i++;
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
		}
		//delay(1000);
		seg_number(i);
	}
}
