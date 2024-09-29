/* DataTypeID.h
*/


#ifndef DATA_TYPE_ID_H
#define DATA_TYPE_ID_H

#include <cstdint>


/// @brief Function to generate unique type IDs
/// @return Unique global ID
inline uint16_t GenerateUniqueID() {
	static uint16_t counter = 0;
	return counter++;
}

/// @brief Provides a static method GetID() to retrieve the unique ID for type T. 
/// The static const uint16_t id ensures that each type's ID is generated only once.
/// IDs are assigned sequentially starting at 0 so they can be used as array indicies.
/// @tparam T - Unique type for the ID
template <typename T>
struct DataTypeID {
	static uint16_t GetID() {
		static const uint16_t id = GenerateUniqueID();
		return id;
	}
};

#endif /*DATA_TYPE_ID_H*/
