/* DataPoolTypes.h
*/

#ifndef DATA_POOL_TYPES_H
#define DATA_POOL_TYPES_H

#include "DataPoolMember.h"

#include <cstdint>
#include <string>

struct Organism : public game::DataPoolMember {
	std::string name;
	Organism() {}
};

struct Plant : public Organism {
	Plant() {}
};

struct Tree : Plant {
	Tree() {}
};

struct Animal : public Organism {
	Animal() {}
};

struct Cat : public Animal {
	Cat() {}
};

struct MainCoon : public Cat {
	MainCoon() {}
};

struct Dog : public Animal {
	Dog() {}
};

#endif // DATA_POOL_TYPES_H
