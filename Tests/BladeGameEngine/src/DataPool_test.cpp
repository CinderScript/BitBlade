// DataPool_test.cpp

#include <gtest/gtest.h>
#include "DataPool.h"
#include "DataPoolTypes_test.h"

// Test fixture for DataPool with Flower
class DataPoolTest : public ::testing::Test {
protected:
	game::DataPool<Flower> flowerPool;

	DataPoolTest() : flowerPool( 10 ) {}

	void SetUp() override {
	}

	void TearDown() override {
	}
};


TEST_F( DataPoolTest, BasicAssertions ) {

	Flower* d = flowerPool.Add( "Daisy" );
	Flower* r = flowerPool.Add( "Rose" );
	Flower* l = flowerPool.Add( "Lily" );

	ASSERT_NE( d, nullptr ) << "Failed to add Plant object. Pointer is null.";
	EXPECT_EQ( d->name, "Daisy" );

	uint16_t index0 = flowerPool.GetObjID( d );
	uint16_t index1 = flowerPool.GetObjID( r );
	uint16_t index2 = flowerPool.GetObjID( l );
	EXPECT_EQ( index0, 0 ) << "GetObjID() returned " << index0 << " but ID = 0 was expected.";
	EXPECT_EQ( index1, 1 ) << "GetObjID() returned " << index1 << " but ID = 1 was expected.";
	EXPECT_EQ( index2, 2 ) << "GetObjID() returned " << index2 << " but ID = 2 was expected.";

	Flower* retrieved = flowerPool.GetObject( index1 );
	EXPECT_EQ( r, retrieved ) << "GetObject() did not return the correct ptr.";
	EXPECT_EQ( r->name, retrieved->name ) << "GetObject() did not return the same object.";


	// Make sure object is removed at expected index
	const bool* occupation = flowerPool.GetOccupationList();
	flowerPool.Remove( index1 );
	EXPECT_TRUE( occupation[index0] );
	EXPECT_FALSE( occupation[index1] );
	EXPECT_TRUE( occupation[index2] );
}


// Test sorting insertion points
TEST_F( DataPoolTest, IndexManagement )
{
	// fill each element
	for (size_t i = 0; i < 10; i++)
	{
		flowerPool.Add( "Flower " + std::to_string( i ) );
	}

	const std::vector<uint16_t>& free = flowerPool.GetFreeIndices();

	EXPECT_TRUE( free.size() == 0 ) << "No free indicies should be left, but size != 0";

	flowerPool.Remove( 4 );
	flowerPool.Remove( 2 );
	flowerPool.Remove( 8 );
	flowerPool.Remove( 7 );

	EXPECT_EQ( free.size(), 4 );

	// index 4 should be first on the list of free IDs, followed by 2...
	EXPECT_EQ( free[0], 4 );
	EXPECT_EQ( free[1], 2 );
	EXPECT_EQ( free[2], 8 );
	EXPECT_EQ( free[3], 7 );

	// test addition to next free index
	auto* daisy = flowerPool.Add( "Daisy" );
	uint16_t dID = flowerPool.GetObjID( daisy );

	EXPECT_EQ( dID, 7 ) << "Daisy should have been added to index 7.";

	EXPECT_EQ( free[0], 4 );
	EXPECT_EQ( free[1], 2 );
	EXPECT_EQ( free[2], 8 );

	flowerPool.SortInsertionOrder();

	EXPECT_EQ( free[0], 8 );
	EXPECT_EQ( free[1], 4 );
	EXPECT_EQ( free[2], 2 );

	auto* lily = flowerPool.Add( "Lily" );
	uint16_t lID = flowerPool.GetObjID( lily );

	EXPECT_EQ( lID, 2 ) << "Daisy should have been added to index 7.";
}

// Additional test: Ensure proper handling of invalid removals
TEST_F( DataPoolTest, RemoveInvalidIndex ) {
	EXPECT_DEATH( flowerPool.Remove( flowerPool.Capacity() + 1 ), ".*Invalid index.*" );
	EXPECT_DEATH( flowerPool.Remove( 0 ), ".*Object at index is already free.*" );
}

// Additional test: Test with derived classes
TEST_F( DataPoolTest, MatchingObjectRetrieval ) {
	// Assuming we have Flower derived from Plant
	game::DataPool<Flower> flowerPool;

	Flower* f1 = flowerPool.Add( "Rose", Color::Red );
	uint16_t index = flowerPool.GetObjID( f1 );

	Flower* retrieved = flowerPool.GetObject( index );
	EXPECT_EQ( f1, retrieved );
	EXPECT_EQ( f1->color, Color::Red );
}
