/* CallbackProcessMessage.h
*/

#ifndef CALLBACK_PROCESS_MESSAGE_H
#define CALLBACK_PROCESS_MESSAGE_H

template<typename T>
class CallbackProcessMessage {
public:
	using Method = void (T::*)(const char*);

	CallbackProcessMessage(T* object, Method method)
		: objectPtr(object), methodPtr(method)
	{
		assert(objectPtr != nullptr && "CallbackProcessMessage constructor: objectPtr is null");
		assert(methodPtr != nullptr && "CallbackProcessMessage constructor: methodPtr is null");
	}

	void SetCallback(T* object, Method method) {
		objectPtr = object;
		methodPtr = method;
	}

	void Execute(const char* message) {
		(objectPtr->*methodPtr)(message);
	}

private:
	T* objectPtr;
	Method methodPtr;
};

#endif // CALLBACK_PROCESS_MESSAGE_H