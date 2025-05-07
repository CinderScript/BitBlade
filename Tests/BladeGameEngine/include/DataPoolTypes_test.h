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
	Organism( uint16_t pID, uint16_t oID, std::string name ) : DataPoolMember( pID, oID ), name( name ) {}
};

struct Plant : public Organism {
	Plant( uint16_t pID, uint16_t oID, std::string name ) : Organism( pID, oID, name ) {}
};

struct Tree : Plant {
	Tree( uint16_t pID, uint16_t oID, std::string name ) : Plant( pID, oID, name ) {}
};
struct Flower : Plant {
	Color color;
	Flower( uint16_t pID, uint16_t oID, std::string name ) : Plant( pID, oID, name ), color( Color::Blue ) {}
	Flower( uint16_t pID, uint16_t oID, std::string name, Color color ) : Plant( pID, oID, name ), color( color ) {}
};

struct Animal : public Organism {
	Animal( uint16_t pID, uint16_t oID, std::string name ) : Organism( pID, oID, name ) {}
};

struct Cat : public Animal {
	bool isIndoor;
	Cat( uint16_t pID, uint16_t oID ) : Animal( pID, oID, "unknown" ), isIndoor( false ) {}
	Cat( uint16_t pID, uint16_t oID, std::string name, bool isIndoor ) : Animal( pID, oID, name ), isIndoor( isIndoor ) {}
};

struct Dog : public Animal {
	int weight;
	Dog( uint16_t pID, uint16_t oID, std::string name, int weight ) : Animal( pID, oID, name ), weight( weight ) {}
	Dog( uint16_t pID, uint16_t oID, std::string name ) : Animal( pID, oID, name ), weight( 50 ) {}
};


#endif // DATA_POOL_TYPES_test_H
