/* DataPoolTypes_test.h
*/

#ifndef DATA_POOL_TYPES_test_H
#define DATA_POOL_TYPES_test_H

#include "DataPoolMember.h"

#include <cstdint>
#include <string>

enum class Color {

	Red, Green, Blue
};

struct Organism : public game::DataPoolMember {
	std::string name;
	Organism( std::string name ) : name( name ) {}
};

struct Plant : public Organism {
	Plant( std::string name ) : Organism( name ) {}
};

struct Tree : Plant {
	Tree( std::string name ) : Plant( name ) {}
};
struct Flower : Plant {
	Color color;
	Flower( std::string name ) : Plant( name ), color( Color::Blue ) {}
	Flower( std::string name, Color color ) : Plant( name ), color( color ) {}
};

struct Animal : public Organism {
	Animal( std::string name ) : Organism( name ) {}
};

struct Cat : public Animal {
	bool isIndoor;
	Cat() : Animal( "unknown" ), isIndoor( false ) {}
	Cat( std::string name, bool isIndoor ) : Animal( name ), isIndoor( isIndoor ) {}
};

struct Dog : public Animal {
	int weight;
	Dog( std::string name, int weight ) : Animal( name ), weight( weight ) {}
	Dog( std::string name ) : Animal( name ), weight( 50 ) {}
};


#endif // DATA_POOL_TYPES_test_H
