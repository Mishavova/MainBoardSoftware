#pragma once
#include "project_config.h"
//GPIO pins are on RCC_AHB1

namespace pins
{
	inline void setPwm(TIM_TypeDef *TIMx, uint8_t ch, uint32_t val)
	{
		if(val > 65535) val = 65535;
		switch(ch)
		{
			case 1:
			TIMx->CCR1 = val;
			break;
			case 2:
			TIMx->CCR2 = val;
			break;
			case 3:
			TIMx->CCR3 = val;
			break;
			case 4:
			TIMx->CCR4 = val;
			break;
			default:
			break;
		}
	}
	inline void setPin(GPIO_TypeDef *port, uint32_t pin, bool state)
	{
		if(state) GPIO_SetBits(port, pin);
		else GPIO_ResetBits(port, pin);
	}
	inline void initOut(GPIO_TypeDef *port, uint32_t pin)
	{
		GPIO_InitTypeDef gpio;
		GPIO_StructInit(&gpio);
		gpio.GPIO_Mode = GPIO_Mode_OUT;
		gpio.GPIO_OType = GPIO_OType_PP;
		gpio.GPIO_Speed = GPIO_Speed_2MHz;
		gpio.GPIO_Pin = pin;
		GPIO_Init(port, &gpio);
	}
	inline void initPwm(GPIO_TypeDef *port, uint32_t pin, TIM_TypeDef *TIMx, uint8_t ch)
	{
		GPIO_InitTypeDef gpio;
		GPIO_StructInit(&gpio);
		gpio.GPIO_Mode = GPIO_Mode_AF;
		gpio.GPIO_OType = GPIO_OType_PP;
		gpio.GPIO_Speed = GPIO_Speed_2MHz;
		gpio.GPIO_Pin = pin;
		GPIO_Init(port, &gpio);
		
		TIM_TimeBaseInitTypeDef timer;
		TIM_TimeBaseStructInit(&timer);
		timer.TIM_Prescaler = 2100; // PWM_freq = Clocking / Prescaler = 20KHz
		timer.TIM_Period = 65535; //0xffff
		timer.TIM_ClockDivision = 0;
		timer.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM3, &timer);
	
		TIM_OCInitTypeDef timerPWM;	
		TIM_OCStructInit(&timerPWM);
		timerPWM.TIM_OCMode = TIM_OCMode_PWM1;
		timerPWM.TIM_OutputState = TIM_OutputState_Enable;
		timerPWM.TIM_Pulse = 0;
		timerPWM.TIM_OCPolarity = TIM_OCPolarity_High;
		switch(ch)
		{
			case 1:
			TIM_OC1Init(TIMx, &timerPWM);
			break;
			case 2:
			TIM_OC2Init(TIMx, &timerPWM);
			break;
			case 3:
			TIM_OC3Init(TIMx, &timerPWM);
			break;
			case 4:
			TIM_OC4Init(TIMx, &timerPWM);
			break;
			default:
			break;
		}
		TIM_Cmd(TIMx, ENABLE);
		TIM_CtrlPWMOutputs(TIMx, ENABLE);
	}
	namespace motors
	{
		//see datasheet for mc33926
		static uint32_t in1Pin[4] = {0, 1, 2, 3};
		static GPIO_TypeDef* in1Port[4] = {GPIOB, GPIOC, GPIOD, GPIOE};
		static TIM_TypeDef* in1Tim[4] = {TIM1, TIM2, TIM3, TIM4};
		static uint8_t in1Ch[4] = {0, 1, 2, 3};
		
		static uint32_t in2Pin[4] = {4, 5, 6, 7};
		static GPIO_TypeDef* in2Port[4] = {GPIOB, GPIOB, GPIOB, GPIOB};
		static TIM_TypeDef* in2Tim[4] = {TIM1, TIM2, TIM3, TIM4};
		static uint8_t in2Ch[4] = {0, 1, 2, 3};
		
		static uint32_t enPin[4] = {8, 9, 10, 11};
		static GPIO_TypeDef* enPort[4] = {GPIOB, GPIOB, GPIOB, GPIOB};
		
		static uint32_t sfPin[4] = {12, 13, 14, 15};
		static GPIO_TypeDef* sfPort[4] = {GPIOB, GPIOB, GPIOB, GPIOB};
		
		static uint32_t d1Pin[4] = {0, 1, 2, 3};
		static GPIO_TypeDef* d1Port[4] = {GPIOC, GPIOC, GPIOC, GPIOC};
		
		static uint32_t d2Pin[4] = {4, 5, 6, 7};
		static GPIO_TypeDef* d2Port[4] = {GPIOC, GPIOC, GPIOC, GPIOC};
		
		static uint32_t slewPin[4] = {8, 9, 10, 11};
		static GPIO_TypeDef* slewPort[4] = {GPIOC, GPIOC, GPIOC, GPIOC};
		
		static uint32_t invPin[4] = {12, 13, 14, 15};
		static GPIO_TypeDef* invPort[4] = {GPIOC, GPIOC, GPIOC, GPIOC};
	};
};
