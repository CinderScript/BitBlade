#include "BladeConsole.h"

#include <DisplayDriver.h>

int BladeConsole::TestFunction()
{
	DisplayDriver test;
	return test.Test();
}