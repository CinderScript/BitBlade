// Event.h

#ifndef EVENT_H
#define EVENT_H

#include "Delegate.h"

#include <vector>
#include <cstdint>


template<typename... Args>
class Event {
private:
	std::vector<DelegateBase<Args...>*> delegates;

public:
	Event( size_t reserve ) { delegates.reserve( reserve ); }

	template<typename T>
	bool Subscribe( T* instance, void (T::* memberFunction)(Args...) ) {
		delegates.push_back( new Delegate<T, Args...>( instance, memberFunction ) );
		return true;
	}

	template<typename T>
	void Unsubscribe( T* instance, void (T::* memberFunction)(Args...) ) {
		for (auto it = delegates.begin(); it != delegates.end(); ++it) {
			Delegate<T, Args...> tempDelegate( instance, memberFunction );
			if ((*it)->Equals( &tempDelegate )) {
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
