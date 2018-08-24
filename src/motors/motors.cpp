#include "motors.h"

Motors::Motors()
{
}

void Motors::initPins()
{
	for(int i = 0; i < 4; i++)
	{
		RCC_AHB1PeriphClockCmd(gpioToRcc(pins::motors::in1Port[i]), ENABLE);
		RCC_AHB1PeriphClockCmd(gpioToRcc(pins::motors::in2Port[i]), ENABLE);
		RCC_AHB1PeriphClockCmd(gpioToRcc(pins::motors::d1Port[i]), ENABLE);
		RCC_AHB1PeriphClockCmd(gpioToRcc(pins::motors::d2Port[i]), ENABLE);
		RCC_AHB1PeriphClockCmd(gpioToRcc(pins::motors::enPort[i]), ENABLE);
		RCC_AHB1PeriphClockCmd(gpioToRcc(pins::motors::sfPort[i]), ENABLE);
		RCC_AHB1PeriphClockCmd(gpioToRcc(pins::motors::slewPort[i]), ENABLE);
		RCC_AHB1PeriphClockCmd(gpioToRcc(pins::motors::invPort[i]), ENABLE);
	}
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	for(int i = 0; i < 4; i++)
	{
		pins::initPwm(pins::motors::in1Port[i], pins::motors::in1Pin[i], pins::motors::in1Tim[i], pins::motors::in1Ch[i]);
		pins::initPwm(pins::motors::in2Port[i], pins::motors::in2Pin[i], pins::motors::in2Tim[i], pins::motors::in2Ch[i]);
		pins::initOut(pins::motors::d1Port[i], pins::motors::d1Pin[i]);
		pins::initOut(pins::motors::d2Port[i], pins::motors::d2Pin[i]);
		pins::initOut(pins::motors::enPort[i], pins::motors::enPin[i]);
		pins::initOut(pins::motors::sfPort[i], pins::motors::sfPin[i]);
		pins::initOut(pins::motors::slewPort[i], pins::motors::slewPin[i]);
		pins::initOut(pins::motors::invPort[i], pins::motors::invPin[i]);
	}
	for(int i = 0; i < 4; i++)
	{
		pins::setPin(pins::motors::d1Port[i], pins::motors::d1Pin[i], 0);
		pins::setPin(pins::motors::d2Port[i], pins::motors::d2Pin[i], 1);
		pins::setPin(pins::motors::enPort[i], pins::motors::enPin[i], 1);
		pins::setPin(pins::motors::sfPort[i], pins::motors::sfPin[i], 1);
		pins::setPin(pins::motors::slewPort[i], pins::motors::slewPin[i], 0);
		pins::setPin(pins::motors::invPort[i], pins::motors::invPin[i], 0);
	}
}

void Motors::setPower(uint8_t mot, int32_t pwr)
{
	bool dir = false;
	if(pwr > 0) dir = true;
	if(abs(pwr) > MAX_SPEED) pwr = MAX_SPEED;
	if(dir)
	{
		pins::setPwm(pins::motors::in1Tim[mot], pins::motors::in1Ch[mot], abs(pwr));
		pins::setPwm(pins::motors::in2Tim[mot], pins::motors::in2Ch[mot], 0);
	}
	else
	{
		pins::setPwm(pins::motors::in1Tim[mot], pins::motors::in1Ch[mot], 0);
		pins::setPwm(pins::motors::in2Tim[mot], pins::motors::in2Ch[mot], abs(pwr));
	}
}

void Motors::setPower(int32_t pwr1, int32_t pwr2, int32_t pwr3, int32_t pwr4)
{
	this->setPower(RU, pwr1);
	this->setPower(RD, pwr2);
	this->setPower(LD, pwr3);
	this->setPower(LU, pwr4);
}

void Motors::setPower(int32_t pwr[4])
{
	this->setPower(pwr[0], pwr[1], pwr[2], pwr[3]);
}

Motors::~Motors()
{
}
