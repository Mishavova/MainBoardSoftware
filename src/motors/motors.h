#pragma once
#include "project_config.h"
#include "pins.h"

#define RU 0 // Right up
#define RD 1 // Right down
#define LD 2 // Left down
#define LU 3 // Left up
#define MAX_SPEED 65535
class Motors
{
	private:
	public:
	Motors();
	void initPins();
	void setPower(uint8_t mot, int32_t pwr); // RU, RD, LD, LU
	void setPower(int32_t pwr1, int32_t pwr2, int32_t pwr3, int32_t pwr4);
	void setPower(int32_t pwr[4]);
	~Motors();
};
