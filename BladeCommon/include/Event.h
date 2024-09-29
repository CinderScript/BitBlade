// Event.h

#ifndef EVENT_H
#define EVENT_H

#include <vector>

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
		typedef void (T::* MemberFunction)(ArgsD...);

		Delegate( T* instance, MemberFunction function )
			: instance( instance ), function( function ) {}

		void Invoke( ArgsD... args ) override {
			(instance->*function)(args...);
		}

		bool EqualsInstanceFunction( const void* instancePtr, const void* memberFunctionPtr ) const override {
			return instance == static_cast<const T*>(instancePtr) &&
				function == *reinterpret_cast<const MemberFunction*>(memberFunctionPtr);
		}

		T* instance;
		MemberFunction function;
	};

	std::vector<DelegateBase<Args...>*> delegates;

public:

	Event( int reserve ) { delegates.reserve( reserve ); }

	template<typename T>
	bool Subscribe( T* instance, void (T::* memberFunction)(Args...) ) {
		delegates.push_back( new Delegate<T, Args...>( instance, memberFunction ) );
		return true;
	}

	template<typename T>
	void Unsubscribe( T* instance, void (T::* memberFunction)(Args...) ) {
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

	void Notify( Args... args ) {
		// Copy the list to prevent issues if delegates  
		// subscribe or unsubscribe to this delegates list
		auto delegatesCopy = delegates;
		for (auto delegate : delegatesCopy) {
			delegate->Invoke( args... );
		}
	}

	~Event() {
		for (auto delegate : delegates) {
			delete delegate;
		}
	}
};

#endif // EVENT_H
