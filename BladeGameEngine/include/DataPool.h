/* DataPool.h
*/

#ifndef DATA_POOL_H
#define DATA_POOL_H

#include "DataPoolMember.h" // Include PoolObject to ensure T derives from it
#include <vector>
#include <cassert>
#include <cstdint>
#include <algorithm>

namespace game {

	struct IPool {
		virtual ~IPool() = default;
		virtual void Remove( uint16_t index ) = 0;
		virtual void SortInsertionOrder() = 0;
	};

	template<typename T>
	struct DataPool : IPool {

		// Delete copy constructor and copy assignment operator
		DataPool( const DataPool& ) = delete;
		DataPool& operator=( const DataPool& ) = delete;

		DataPool( size_t cap = 100 ) : capacity( cap ), count( 0 )
		{
			// Initialize buffer in memory
			//buffer = static_cast<char*>(::operator new[]( capacity * sizeof( T ), std::align_val_t( alignof(T) ) ));
			buffer = static_cast<char*>(::operator new[]( capacity * sizeof( T ) ));

			occupied = new bool[capacity];

			freeIndices.reserve( capacity );

			// Initialize occupancy flags
			for (uint16_t i = 0; i < capacity; ++i) {
				occupied[i] = false;
			}
			// Initialize freeIndices with all indices
			for (uint16_t i = 0; i < capacity; ++i) {
				freeIndices.push_back( capacity - i - 1 ); // Add indices in reverse order
			}
		}

		~DataPool() override {
			// Call destructors for all occupied objects
			for (uint16_t i = 0; i < capacity; ++i) {
				if (occupied[i]) {
					T* obj = reinterpret_cast<T*>(buffer + i * sizeof( T ));
					obj->~T();
				}
			}

			// Deallocate the buffer and occupied array
			//::operator delete[]( buffer, std::align_val_t( alignof(T) ) );
			::operator delete[]( buffer );
			delete[] occupied;
		}

		template<typename... Args>
		T* Add( Args&&... args ) {

			//if (count < capacity)
			if (!freeIndices.empty())
			{
				uint16_t pos = freeIndices.back();
				freeIndices.pop_back();

				// Construct object in place using placement new
				T* obj = new (buffer + pos * sizeof( T )) T( std::forward<Args>( args )... );
				occupied[pos] = true;

				count++;
				return obj;
			}
			else {
				return nullptr;
			}
		}

		void Remove( uint16_t index ) override {
			assert( index < capacity && "Invalid index." );
			assert( occupied[index] && "Object at index is already free." );

			// Call the destructor
			T* obj = reinterpret_cast<T*>(buffer + index * sizeof( T ));
			obj->~T();

			// Mark as free
			occupied[index] = false;
			freeIndices.push_back( index );
			count--;
		}

		void SortInsertionOrder() override {
			std::sort( freeIndices.begin(), freeIndices.end(), std::greater<uint16_t>() );
		}

		T* GetObject( uint16_t pos ) {
			assert( pos < capacity && "Invalid index." );
			assert( occupied[pos] && "Object at index is not occupied." );

			return reinterpret_cast<T*>(buffer + pos * sizeof( T ));
		}
		uint16_t GetObjID( T* obj ) {
			uintptr_t objAddress = reinterpret_cast<uintptr_t>(obj);
			uintptr_t baseAddress = reinterpret_cast<uintptr_t>(buffer);

			uint16_t pos = static_cast<uint16_t>((objAddress - baseAddress) / sizeof( T ));
			assert( pos < capacity && "Invalid object pointer." );
			return pos;
		}

		uint16_t Capacity() const { return capacity; }
		uint16_t Count() const { return count; }
		const std::vector<uint16_t>& GetFreeIndices() const { return freeIndices; }
		const bool* GetOccupationList() const { return occupied; }

	private:
		const uint16_t capacity;
		uint16_t count;
		alignas(alignof(T)) char* buffer;  						// Raw memory buffer
		bool* occupied;						                    // Occupancy flags
		std::vector<uint16_t> freeIndices;                      // Indices of free slots

	};

} // End of namespace game

#endif // DATA_POOL_H
