/* DataPoolMember.h
*/

#ifndef DATA_POOL_MEMBER_H
#define DATA_POOL_MEMBER_H

#include <cstdint>

namespace game {

	class DataCluster;

	class DataPoolMember {
	public:
		friend class DataCluster;

		DataPoolMember( uint16_t poolID, uint16_t objectID ) : poolID( poolID ), objectID( objectID ) {}
		virtual ~DataPoolMember() = default;

		uint16_t PoolID() const { return poolID; }
		uint16_t ObjectID() const { return objectID; }

		void InvalidateStorage() { poolID = 65535; objectID = 65535; }

	protected:
		uint16_t poolID = 65535;
		uint16_t objectID = 65535;
	};
}


#endif // DATA_POOL_MEMBER_H
