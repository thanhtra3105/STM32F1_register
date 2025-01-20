#include "stm32f10x.h" // Ði?u ch?nh theo dòng STM32 c?a b?n

void PWM_TIM2_CH1_Init(void) {
    // B?t clock cho GPIOA và Timer 2
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // B?t clock cho GPIOA
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // B?t clock cho Timer 2

    // C?u hình PA0 (TIM2_CH1) là Alternate Function Push-Pull
    GPIOA->CRL &= ~GPIO_CRL_CNF0;       // Xóa các bit CNF cho PA0
    GPIOA->CRL |= GPIO_CRL_CNF0_1;      // Ð?t PA0 là Alternate Function Push-Pull
    GPIOA->CRL |= GPIO_CRL_MODE0;       // Ð?t PA0 là output v?i t?c d? max 50 MHz

    // C?u hình Timer 2
    TIM2->PSC = 7;  // Prescaler = 7 (t?n s? timer = 8 MHz / (7 + 1) = 1 MHz)
    TIM2->ARR = 2000; // Auto-reload value = 2000 (t?n s? PWM = 1 MHz / 2000 = 500 Hz)

    // C?u hình PWM mode 1 cho Channel 1
    TIM2->CCMR1 &= ~TIM_CCMR1_OC1M; // Xóa các bit OC1M tru?c khi c?u hình
    TIM2->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; // PWM mode 1
    TIM2->CCMR1 |= TIM_CCMR1_OC1PE; // Enable preload cho CCR1

    // Thi?t l?p giá tr? CCR1 cho duty cycle 50%
    TIM2->CCR1 = (50 * 2000) / 100; // 50% duty cycle

    // Enable TIM2_CH1 output
    TIM2->CCER |= TIM_CCER_CC1E;

    // B?t auto-reload preload
    TIM2->CR1 |= TIM_CR1_ARPE;

    // B?t counter
    TIM2->CR1 |= TIM_CR1_CEN;

    // Generate an update event d? kh?i t?o giá tr? ARR và CCR1
    TIM2->EGR = TIM_EGR_UG;
}

int main(void) {
    PWM_TIM2_CH1_Init(); // Kh?i t?o PWM trên TIM2 CH1

    while (1) {
        // Vòng l?p chính
    }
}
