#include <iostream>

#include <windows.h>
#include <vector>

#include "BladeConsole.h"



using std::cout;


int main()
{
	BladeConsole console;

	while (true) {
		console.UpdateConsole();
	}

	cout << "\n" << "Console Done" << "\n";
}