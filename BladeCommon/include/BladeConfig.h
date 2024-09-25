#ifndef BLADECONFIG_H
#define BLADECONFIG_H

#include <cstdint>	// uint*_t


namespace gameConfig
{
	static constexpr int MAX_IMAGE_SOURCES = 100;
	static constexpr int MAX_GAME_OBJECTS = 300;

	/// @brief If a pool type has not been initialized before adding elements,
	///			This default capacity will be used for this pool.
	static constexpr uint16_t DATA_CLUSTER_DEFAULT_POOL_CAPACITY = 200;

	/* ------------------------------ Game Objects ------------------------------ */
	static constexpr int GAME_OBJECT_CHILD_RESERVE = 8;
	static constexpr int GAME_OBJECT_COMPONENT_RESERVE = 5;

}

namespace gfxLinkConfig
{
	/// @brief Total length of a message packed with gfx instructions
	static constexpr int MESSAGE_BUFFER_LENGTH = 5000;

	/// @brief Max lengh (bytes) each packed instruction can take within the total message buffer.
	///        This sets a limit on filename length that can be send during image loading.
	static constexpr int PACKED_INSTRUCTION_MAX_LENGTH = 200;
}

#endif // BLADECONFIG_H