// Event.h
#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <cassert>

template<typename... Args>
class Event {
private:
	/* ------------------------------ DELEGATE BASE ----------------------------- */

	template<typename... ArgsD>
	class DelegateBase {
	public:
		virtual void Invoke( ArgsD... args ) = 0;
		virtual bool EqualsInstanceFunction( const void* instancePtr, const void* memberFunctionPtr ) const = 0;
		virtual ~DelegateBase() {}
	};

	/* -------------------------------- DELEGATE -------------------------------- */

	template<typename T, typename... ArgsD>
	class Delegate : public DelegateBase<ArgsD...> {
	public:
		using MemberFunction = void (T::*)(ArgsD...);

		Delegate( T* instance, MemberFunction function )
			: instance( instance ), function( function ) {}

		inline void Invoke( ArgsD... args ) override {
			(instance->*function)(args...);
		}

		inline bool EqualsInstanceFunction( const void* instancePtr, const void* memberFunctionPtr ) const override {
			return instance == static_cast<const T*>(instancePtr) &&
				function == *reinterpret_cast<const MemberFunction*>(memberFunctionPtr);
		}

		T* instance;
		MemberFunction function;
	};

	std::vector<DelegateBase<Args...>*> delegates;

	// **Reentrancy Guard Flag**
	bool isNotifying = false;

public:
	Event( int reserve = 0 ) {
		if (reserve > 0) delegates.reserve( reserve );
	}

	template<typename T>
	bool Subscribe( T* instance, void (T::* memberFunction)(Args...) ) {
		assert( !isNotifying && "Cannot subscribe during Invoke" );

		delegates.push_back( new Delegate<T, Args...>( instance, memberFunction ) );
		return true;
	}

	template<typename T>
	void Unsubscribe( T* instance, void (T::* memberFunction)(Args...) ) {
		assert( !isNotifying && "Cannot unsubscribe during Invoke" );

		const void* instancePtr = static_cast<const void*>(instance);
		const void* memberFunctionPtr = reinterpret_cast<const void*>(&memberFunction);

		for (auto it = delegates.begin(); it != delegates.end(); ++it) {
			if ((*it)->EqualsInstanceFunction( instancePtr, memberFunctionPtr )) {
				delete* it;
				delegates.erase( it );
				break;
			}
		}
	}

	inline void Invoke( Args... args ) {

		assert( !isNotifying && "Cannot Invoke the same Event within a previous Invoke" );

		isNotifying = true;

		// // Copy the list to prevent issues if delegates  
		// // subscribe or unsubscribe to this delegates list
		// auto delegatesCopy = delegates;
		for (auto delegate : delegates) {
			delegate->Invoke( args... );
		}

		isNotifying = false;
	}

	~Event() {
		for (auto delegate : delegates) {
			delete delegate;
		}
	}
};

#endif // EVENT_H
