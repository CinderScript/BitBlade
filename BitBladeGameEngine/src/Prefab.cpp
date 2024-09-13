/* Prefab.cpp
*/

#include "Prefab.h"

ConsoleLink* Prefab::consoleLink;

Prefab::Prefab( const char* filename ) : filename( filename )
{
	//consoleLink->PackInstruction()
}

Prefab::~Prefab()
{
}

void Prefab::setConsoleLink( ConsoleLink* link )
{
	consoleLink = link;
}
