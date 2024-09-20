/* DataCluster.h
*/

#ifndef DATA_CLUSTER_H
#define DATA_CLUSTER_H

#include "DataPoolMember.h"
#include "DataPool.h"

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
		/// @brief If a pool type has not been initialized before adding elements,
		///			This default capacity will be used for this pool.
		static constexpr uint16_t DEFAULT_POOL_CAPACITY = 200;

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

			// Set the identification of the Pool member
			uint16_t objID = pool.GetObjID( obj );
			obj->SetIdentification( poolID, objID );

			return obj;
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
		DataPool<T>& GetPool() {
			return GetOrCreatePool<T>();
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

		template<typename T>
		void InitializeCapacity( uint16_t capacity ) {
			uint16_t trash;
			GetOrCreatePool<T>( trash, capacity );
		}

	private:

		std::vector<std::unique_ptr<IPool>> pools;
		std::unordered_map<std::type_index, uint16_t> typeToPoolIndex;
		uint16_t nextPoolID = 0;

		template<typename T>
		DataPool<T>& GetOrCreatePool( uint16_t& out_PoolID, uint16_t capacity = DEFAULT_POOL_CAPACITY ) {
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
