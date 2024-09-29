/* DataCluster.h
*/

#ifndef DATA_CLUSTER_H
#define DATA_CLUSTER_H

#include "DataPoolMember.h"
#include "DataTypeID.h"
#include "DataPool.h"
#include "BladeConfig.h"

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cassert>
#include <algorithm>
#include <type_traits> // For std::is_base_of

namespace game {

	class DataCluster {

	public:

		DataCluster() {}
		~DataCluster() {}

		template<typename T, typename... Args>
		T* Add( Args&&... args ) {
			static_assert(std::is_base_of<DataPoolMember, T>::value, "T must be a DataPoolMember");

			// Get or create the pool for type T
			uint16_t poolID;
			DataPool<T>& pool = GetOrCreatePool<T>( poolID );

			// Add the object to the pool
			T* obj = pool.Add( std::forward<Args>( args )... );

			// if there was not room for the new object
			if (obj == nullptr)
				return nullptr;

			// Set the identification of the Pool member
			uint16_t objID = pool.GetObjID( obj );
			obj->SetIdentification( poolID, objID );
			return obj;
		}

		void Remove( DataPoolMember* obj ) {
			uint16_t poolID = obj->PoolID();
			uint16_t objID = obj->ObjectID();

			auto it = pools.find( poolID );
			assert( it != pools.end() && "Invalid poolID." );

			IPool* ipool = it->second.get();
			ipool->Remove( objID );

			obj->SetIdentification( 65535, 65535 ); // Invalidate identification
		}


		template<typename T>
		void SortInsertionOrder() {
			DataPool<T>& pool = GetOrCreatePool<T>();
			pool.SortInsertionOrder();
		}

		// Sorts all pools in the cluster
		void SortInsertionOrder() {
			for (auto& pair : pools) {
				IPool* ipool = pair.second.get();
				ipool->SortInsertionOrder();
			}
		}


		uint16_t PoolCount() const {
			return pools.size();
		}

		template<typename T>
		uint16_t GetPoolID() const {
			return DataTypeID<T>::GetID();
		}


		template<typename T>
		const DataPool<T>* GetPool() const {
			uint16_t typeID = DataTypeID<T>::GetID();
			auto it = pools.find( typeID );
			if (it != pools.end()) {
				return static_cast<DataPool<T>*>(it->second.get());
			}
			else {
				return nullptr;
			}
		}

		template<typename T>
		bool DoesPoolExist() const {
			uint16_t typeID = DataTypeID<T>::GetID();
			return pools.find( typeID ) != pools.end();
		}

		/// @brief If a pool has not already been added, creates the pool with the reserved 
		///			capacity. If the pool already exists (through previous reserve or add) then
		///			does nothing.
		/// @tparam T - Data Type the pool holds
		/// @param capacity - Max capacity of the pool
		/// @return The reserved pool, otherwise nullptr
		template<typename T>
		const DataPool<T>* ReservePool( uint16_t capacity ) {
			uint16_t typeID = DataTypeID<T>::GetID();

			// Check if pool already exists
			if (pools.find( typeID ) != pools.end()) {
				return nullptr; // Pool already exists
			}

			// Create the new pool
			pools[typeID] = std::make_unique<DataPool<T>>( capacity );

			DataPool<T>* poolPtr = static_cast<DataPool<T>*>(pools[typeID].get());
			return poolPtr; // Return the newly created pool
		}



	private:
		std::unordered_map<uint16_t, std::unique_ptr<IPool>> pools;

		template<typename T>
		DataPool<T>& GetOrCreatePool(
			uint16_t& out_PoolID,
			uint16_t capacity = gameConfig::DATA_CLUSTER_DEFAULT_POOL_CAPACITY )
		{
			uint16_t typeID = DataTypeID<T>::GetID();
			out_PoolID = typeID;

			auto it = pools.find( typeID );
			if (it == pools.end()) {
				// Create a new pool for this type
				pools[typeID] = std::make_unique<DataPool<T>>( capacity );
			}

			DataPool<T>* poolPtr = static_cast<DataPool<T>*>(pools[typeID].get());
			return *poolPtr;
		}

	};

} // End of namespace game

#endif // DATA_CLUSTER_H
