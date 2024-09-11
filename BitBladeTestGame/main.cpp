#include <iostream>

#include <windows.h>
#include <vector>

#include "BladeConsole.h"
#include "TestGame.h"

// #include <memory>  // for smart pointers

using std::cout;

int main()
{
	TestGame myGame;
	console::BladeConsole console( myGame );

	console.StartConsole();
	while (true)
	{
		console.UpdateConsole();
	}

	cout << "\n"
		<< "Console Done" << "\n";
}