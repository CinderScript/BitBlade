#include <iostream>

#include <windows.h>
#include <vector>

#include "BladeConsole.h"
#include "TestGame.h"

#include <memory>  // for smart pointers

using std::cout;

int main()
{
	BladeConsole console;

	// Transfer ownership of TestGame to the console
	std::unique_ptr<TestGame> game = std::make_unique<TestGame>();
	console.LoadGame(std::move(game));

	console.StartConsole();
	while (true) {
		console.UpdateConsole();
	}

	cout << "\n" << "Console Done" << "\n";
}