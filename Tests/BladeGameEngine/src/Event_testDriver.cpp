// Event_test.cpp

#include <gtest/gtest.h>
#include "Event.h"

// Listener class for testing
class TestListener {
public:
	TestListener() : call_count( 0 ), last_value( 0 ) {}

	void OnEvent( int value ) {
		call_count++;
		last_value = value;
	}

	int call_count;
	int last_value;
};

// Test subscribing and notifying a single listener
TEST( EventTest, SingleSubscription ) {
	Event<int> event( 10 );
	TestListener listener;

	// Subscribe the listener
	event.Subscribe( &listener, &TestListener::OnEvent );

	// Notify the event
	event.Notify( 42 );

	// Verify that the listener was called
	EXPECT_EQ( listener.call_count, 1 );
	EXPECT_EQ( listener.last_value, 42 );
}

// Test subscribing multiple listeners
TEST( EventTest, MultipleSubscriptions ) {
	Event<int> event( 10 );
	TestListener listener1;
	TestListener listener2;

	// Subscribe both listeners
	event.Subscribe( &listener1, &TestListener::OnEvent );
	event.Subscribe( &listener2, &TestListener::OnEvent );

	// Notify the event
	event.Notify( 100 );

	// Verify that both listeners were called
	EXPECT_EQ( listener1.call_count, 1 );
	EXPECT_EQ( listener1.last_value, 100 );
	EXPECT_EQ( listener2.call_count, 1 );
	EXPECT_EQ( listener2.last_value, 100 );
}

// Test unsubscribing a listener
TEST( EventTest, UnsubscribeListener ) {
	Event<int> event( 10 );
	TestListener listener1;
	TestListener listener2;

	// Subscribe both listeners
	event.Subscribe( &listener1, &TestListener::OnEvent );
	event.Subscribe( &listener2, &TestListener::OnEvent );

	// Unsubscribe listener1
	event.Unsubscribe( &listener1, &TestListener::OnEvent );

	// Notify the event
	event.Notify( 200 );

	// Verify that listener1 was not called, but listener2 was
	EXPECT_EQ( listener1.call_count, 0 );
	EXPECT_EQ( listener2.call_count, 1 );
	EXPECT_EQ( listener2.last_value, 200 );
}

// Test unsubscribing a non-subscribed listener
TEST( EventTest, UnsubscribeNonSubscriber ) {
	Event<int> event( 10 );
	TestListener listener;

	// Attempt to unsubscribe without subscribing first
	event.Unsubscribe( &listener, &TestListener::OnEvent );

	// Notify the event
	event.Notify( 300 );

	// Verify that listener was not called
	EXPECT_EQ( listener.call_count, 0 );
}

// Test subscribing the same listener multiple times
TEST( EventTest, SubscribeSameListenerMultipleTimes ) {
	Event<int> event( 10 );
	TestListener listener;

	// Subscribe the same listener multiple times
	event.Subscribe( &listener, &TestListener::OnEvent );
	event.Subscribe( &listener, &TestListener::OnEvent );

	// Notify the event
	event.Notify( 400 );

	// Verify that the listener was called twice
	EXPECT_EQ( listener.call_count, 2 );
	EXPECT_EQ( listener.last_value, 400 );
}

// Test event with multiple arguments
TEST( EventTest, EventWithMultipleArguments ) {
	Event<int, const std::string&> event( 10 );

	class MultiArgListener {
	public:
		MultiArgListener() : call_count( 0 ), last_int( 0 ), last_str( "" ) {}

		void OnEvent( int value, const std::string& str ) {
			call_count++;
			last_int = value;
			last_str = str;
		}

		int call_count;
		int last_int;
		std::string last_str;
	};

	MultiArgListener listener;
	event.Subscribe( &listener, &MultiArgListener::OnEvent );

	// Notify the event
	event.Notify( 500, "test" );

	// Verify that the listener received the correct arguments
	EXPECT_EQ( listener.call_count, 1 );
	EXPECT_EQ( listener.last_int, 500 );
	EXPECT_EQ( listener.last_str, "test" );
}

// Test notifying an event with no subscribers
TEST( EventTest, NotifyWithNoSubscribers ) {
	Event<int> event( 10 );

	// Notify the event (should not cause any issues)
	event.Notify( 600 );
}

// Test subscribing and unsubscribing multiple listeners
TEST( EventTest, SubscribeUnsubscribeMultipleListeners ) {
	Event<int> event( 10 );
	TestListener listener1;
	TestListener listener2;
	TestListener listener3;

	// Subscribe all listeners
	event.Subscribe( &listener1, &TestListener::OnEvent );
	event.Subscribe( &listener2, &TestListener::OnEvent );
	event.Subscribe( &listener3, &TestListener::OnEvent );

	// Unsubscribe listener2
	event.Unsubscribe( &listener2, &TestListener::OnEvent );

	// Notify the event
	event.Notify( 700 );

	// Verify that listener1 and listener3 were called, but listener2 was not
	EXPECT_EQ( listener1.call_count, 1 );
	EXPECT_EQ( listener1.last_value, 700 );
	EXPECT_EQ( listener2.call_count, 0 );
	EXPECT_EQ( listener3.call_count, 1 );
	EXPECT_EQ( listener3.last_value, 700 );
}

// Test unsubscribing all listeners
TEST( EventTest, UnsubscribeAllListeners ) {
	Event<int> event( 10 );
	TestListener listener1;
	TestListener listener2;

	// Subscribe both listeners
	event.Subscribe( &listener1, &TestListener::OnEvent );
	event.Subscribe( &listener2, &TestListener::OnEvent );

	// Unsubscribe both listeners
	event.Unsubscribe( &listener1, &TestListener::OnEvent );
	event.Unsubscribe( &listener2, &TestListener::OnEvent );

	// Notify the event
	event.Notify( 800 );

	// Verify that neither listener was called
	EXPECT_EQ( listener1.call_count, 0 );
	EXPECT_EQ( listener2.call_count, 0 );
}

// Test modifying subscriptions during notification
TEST( EventTest, ModifySubscriptionsDuringNotification ) {
	class SelfModifyingListener {
	public:
		SelfModifyingListener( Event<int>& event ) : event( event ), call_count( 0 ) {}

		void OnEvent( int value ) {
			call_count++;
			// Unsubscribe itself during notification
			event.Unsubscribe( this, &SelfModifyingListener::OnEvent );
		}

		Event<int>& event;
		int call_count;
	};

	Event<int> event( 10 );
	SelfModifyingListener listener( event );

	// Subscribe the listener
	event.Subscribe( &listener, &SelfModifyingListener::OnEvent );

	// Notify the event multiple times
	event.Notify( 900 );
	event.Notify( 900 );

	// Verify that the listener was only called once
	EXPECT_EQ( listener.call_count, 1 );
}

// Test that subscribers can subscribe new listeners during notification
TEST( EventTest, SubscribeDuringNotification ) {
	class DynamicSubscriber {
	public:
		DynamicSubscriber( Event<int>& event, TestListener& newListener )
			: event( event ), newListener( newListener ), call_count( 0 ) {}

		void OnEvent( int value ) {
			call_count++;
			// Subscribe a new listener during notification
			event.Subscribe( &newListener, &TestListener::OnEvent );
		}

		Event<int>& event;
		TestListener& newListener;
		int call_count;
	};

	Event<int> event( 10 );
	TestListener newListener;
	DynamicSubscriber listener( event, newListener );

	// Subscribe the dynamic subscriber
	event.Subscribe( &listener, &DynamicSubscriber::OnEvent );

	// Notify the event
	event.Notify( 1000 );

	// Verify that the dynamic subscriber was called
	EXPECT_EQ( listener.call_count, 1 );

	// The new listener should not have been called during the first notification
	EXPECT_EQ( newListener.call_count, 0 );

	// Notify the event again
	event.Notify( 1000 );

	// Verify that the new listener was now called
	EXPECT_EQ( newListener.call_count, 1 );
	EXPECT_EQ( newListener.last_value, 1000 );
}

// Test handling of different argument types
TEST( EventTest, DifferentArgumentTypes ) {
	Event<float, char, const std::string&> event( 10 );

	class ComplexListener {
	public:
		ComplexListener() : call_count( 0 ), last_float( 0.0f ), last_char( '\0' ), last_str( "" ) {}

		void OnEvent( float f, char c, const std::string& s ) {
			call_count++;
			last_float = f;
			last_char = c;
			last_str = s;
		}

		int call_count;
		float last_float;
		char last_char;
		std::string last_str;
	};

	ComplexListener listener;
	event.Subscribe( &listener, &ComplexListener::OnEvent );

	// Notify the event
	event.Notify( 3.14f, 'A', "Hello" );

	// Verify that the listener received the correct arguments
	EXPECT_EQ( listener.call_count, 1 );
	EXPECT_FLOAT_EQ( listener.last_float, 3.14f );
	EXPECT_EQ( listener.last_char, 'A' );
	EXPECT_EQ( listener.last_str, "Hello" );
}

// Test that the event system cleans up properly
TEST( EventTest, DestructorTest ) {
	{
		Event<int> event( 10 );
		TestListener listener;

		// Subscribe the listener
		event.Subscribe( &listener, &TestListener::OnEvent );

		// Notify the event
		event.Notify( 42 );

		// Verify that the listener was called
		EXPECT_EQ( listener.call_count, 1 );
		EXPECT_EQ( listener.last_value, 42 );
	}
	// At this point, the event should be destroyed without any issues
}

// Test subscribing with different member functions
TEST( EventTest, MultipleMemberFunctions ) {
	class MultiMethodListener {
	public:
		MultiMethodListener() : call_count1( 0 ), call_count2( 0 ) {}

		void OnEvent1( int value ) {
			call_count1++;
		}

		void OnEvent2( int value ) {
			call_count2++;
		}

		int call_count1;
		int call_count2;
	};

	Event<int> event( 10 );
	MultiMethodListener listener;

	// Subscribe both member functions
	event.Subscribe( &listener, &MultiMethodListener::OnEvent1 );
	event.Subscribe( &listener, &MultiMethodListener::OnEvent2 );

	// Notify the event
	event.Notify( 123 );

	// Verify that both member functions were called
	EXPECT_EQ( listener.call_count1, 1 );
	EXPECT_EQ( listener.call_count2, 1 );
}
