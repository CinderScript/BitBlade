/* Prefab.h
*/

#ifndef PREFAB_H 
#define PREFAB_H

#include "ConsoleLink.h"

class Prefab {
public:
	static constexpr int DATA_LENGTH = 5;

	Prefab( const char* filename );
	~Prefab();

private:
	static ConsoleLink* consoleLink;

	const char* filename;

	static void setConsoleLink( ConsoleLink* link );
	void packPrefab();
};

#endif // PREFAB_H