#include "project_config.h"
#include "pins.h"
#include "motors.h"

int main()
{
	Motors motors;
	motors.initPins();
	motors.setPower(0, 0, 0, 0);
	return 0;
}
