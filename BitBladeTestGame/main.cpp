#include <iostream>

#include <windows.h>
#include <vector>

#include "BladeRun.h"
#include "TestGame.h"

// #include <memory>  // for smart pointers

using std::cout;

int main()
{
	TestGame myGame;
	game::RunGame( myGame );

	cout << "\n"
		<< "Console Done" << "\n";
}