#include <iostream>

#include <windows.h>
#include <vector>

#include "BitBlade.h"
#include "TestGame.h"

// #include <memory>  // for smart pointers

using std::cout;

int main()
{
	TestGame myGame;
	bitBlade::RunGame( myGame );

	cout << "\n"
		<< "Console Done" << "\n";
}