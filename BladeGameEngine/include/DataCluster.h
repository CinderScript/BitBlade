/* DataCluster.h
*/

#ifndef DATA_CLUSTER_H
#define DATA_CLUSTER_H

#include "DataPoolMember.h"
#include "DataPool.h"
#include "BitBladeCommon.h"

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <cassert>
#include <algorithm>
#include <type_traits> // For std::is_base_of

namespace game {

	class DataCluster {

	public:

		DataCluster() : nextPoolID( 0 ) {}
		~DataCluster() {}

		template<typename T, typename... Args>
		T* Add( Args&&... args ) {
			static_assert(std::is_base_of<DataPoolMember, T>::value, "T must be a DataPoolMember");

			// Get or create the pool for type T
			uint16_t poolID;
			DataPool<T>& pool = GetOrCreatePool<T>( poolID );

			// Add the object to the pool
			T* obj = pool.Add( std::forward<Args>( args )... );

			if (obj == nullptr)
				return nullptr;
			else {
				// Set the identification of the Pool member
				uint16_t objID = pool.GetObjID( obj );
				obj->SetIdentification( poolID, objID );
				return obj;
			}
		}

		void Remove( DataPoolMember* obj ) {
			uint16_t poolID = obj->PoolID();
			uint16_t objID = obj->ObjectID();

			assert( poolID < pools.size() && "Invalid poolID." );

			IPool* ipool = pools[poolID].get();
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
			for (auto& poolPtr : pools) {
				IPool* ipool = poolPtr.get();
				ipool->SortInsertionOrder();
			}
		}

		int32_t PoolCount() const {
			return typeToPoolIndex.size();
		}

		template<typename T>
		int32_t GetPoolID() const {
			auto typeIndex = std::type_index( typeid(T) );
			auto it = typeToPoolIndex.find( typeIndex );

			if (it != typeToPoolIndex.end()) {
				return it->second;
			}
			else {
				return -1;
			}
		}

		template<typename T>
		const DataPool<T>* GetPool() const
		{
			uint32_t poolID = GetPoolID<T>();
			if (poolID != -1) {
				return static_cast<DataPool<T>*>(pools[poolID].get());
			}
			else {
				return nullptr;
			}
		}

		template<typename T>
		bool DoesPoolExist() const {
			auto typeIndex = std::type_index( typeid(T) );
			return typeToPoolIndex.count( typeIndex ) != 0;
		}

		/// @brief If a pool has not already been added, creates the pool with the reserved 
		///			capacity. If the pool already exists (through previous reserve or add) then
		///			does nothing.
		/// @tparam T - Data Type the pool holds
		/// @param capacity - Max capacity of the pool
		/// @return The reserved pool, otherwise nullptr
		template<typename T>
		const DataPool<T>* ReservePool( uint16_t capacity )
		{
			auto typeIndex = std::type_index( typeid(T) );

			// Check if pool already exists
			if (typeToPoolIndex.find( typeIndex ) != typeToPoolIndex.end()) {
				return nullptr; // Pool already exists
			}

			uint16_t poolID = nextPoolID++;
			typeToPoolIndex[typeIndex] = poolID;

			// Create the new pool
			pools.resize( poolID + 1 );
			auto pool = std::make_unique<DataPool<T>>( capacity );
			pools[poolID] = std::move( pool );

			DataPool<T>* poolPtr = static_cast<DataPool<T>*>(pools[poolID].get());
			return poolPtr; // Return the newly created pool
		}


	private:

		std::vector<std::unique_ptr<IPool>> pools;
		std::unordered_map<std::type_index, uint16_t> typeToPoolIndex;
		uint16_t nextPoolID = 0;

		template<typename T>
		DataPool<T>& GetOrCreatePool(
			uint16_t& out_PoolID,
			uint16_t capacity = gameConfig::DATA_CLUSTER_DEFAULT_POOL_CAPACITY )
		{
			auto typeIndex = std::type_index( typeid(T) );
			auto it = typeToPoolIndex.find( typeIndex );

			if (it != typeToPoolIndex.end()) {
				out_PoolID = it->second;
			}
			else {
				out_PoolID = nextPoolID++;
				typeToPoolIndex[typeIndex] = out_PoolID;
			}

			// if the DataPool has not been added, add it here
			if (out_PoolID >= pools.size()) {
				pools.resize( out_PoolID + 1 );
				auto pool = std::make_unique<DataPool<T>>( capacity );
				pools[out_PoolID] = std::move( pool );
			}

			DataPool<T>* poolPtr = static_cast<DataPool<T>*>(pools[out_PoolID].get());
			return *poolPtr;
		}
	};

} // End of namespace game

#endif // DATA_CLUSTER_H
