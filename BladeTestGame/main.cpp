#include <iostream>

#include <windows.h>
#include <vector>

#include "BladeStart.h"
#include "TestGame.h"
#include "BladeConsole.h"

// #include <memory>  // for smart pointers

using std::cout;

int main()
{
	cout << "\n\n";

	game::StartGame<TestGame>();

	cout << "\n"
		<< "Console Done" << "\n";
}