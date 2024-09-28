// Delegate.h

#ifndef DELEGATE_H
#define DELEGATE_H


/* ------------------------------ DELEGATE BASE ----------------------------- */

template<typename... Args>
class DelegateBase {
public:
	virtual void Invoke( Args... args ) = 0;
	virtual bool Equals( const DelegateBase<Args...>* other ) const = 0;
	virtual ~DelegateBase() {}
};


/* -------------------------------- DELEGATE -------------------------------- */

template<typename T, typename... Args>
class Delegate : public DelegateBase<Args...> {

private:
	T* instance;
	MemberFunction function;

public:
	typedef void (T::* MemberFunction)(Args...);

	Delegate( T* instance, MemberFunction function )
		: instance( instance ), function( function ) {}

	void Invoke( Args... args ) override {
		(instance->*function)(args...);
	}

	bool Equals( const DelegateBase<Args...>* other ) const override {
		const Delegate<T, Args...>* otherDelegate = dynamic_cast<const Delegate<T, Args...>*>(other);
		return otherDelegate && otherDelegate->instance == instance && otherDelegate->function == function;
	}
};

#endif // DELEGATE_H
