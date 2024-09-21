// DataCluster_test.cpp

#include <gtest/gtest.h>
#include "DataPool.h"
#include "DataCluster.h"
#include "BitBladeCommon.h"
#include "DataPoolTypes_test.h"

using game::DataCluster;
using game::DataPool;

// Test fixture for DataPool with Flower
class DataClusterTest : public ::testing::Test {
protected:
	DataCluster cluster;

	DataClusterTest() {}

	void SetUp() override {
	}

	void TearDown() override {
	}
};


TEST_F( DataClusterTest, AddPool ) {

	cluster.Add<Flower>( "Daisy" );
	cluster.Add<Dog>( "Rover" );

	EXPECT_TRUE( cluster.DoesPoolExist<Flower>() );
	EXPECT_TRUE( cluster.DoesPoolExist<Dog>() );
	EXPECT_FALSE( cluster.DoesPoolExist<Tree>() );
}


TEST_F( DataClusterTest, GetPool ) {
	cluster.Add<Flower>( "Daisy", Color::Red );
	cluster.Add<Flower>( "Lily", Color::Blue );
	cluster.Add<Dog>( "Rover" );

	const auto* flowers = cluster.GetPool<Flower>();
	const auto* dogs = cluster.GetPool<Dog>();
	const auto* trees = cluster.GetPool<Tree>();

	ASSERT_NE( flowers, nullptr );  // Flowers should not be null
	ASSERT_NE( dogs, nullptr );     // Dogs should not be null
	ASSERT_EQ( trees, nullptr );    // Trees should be null (no Tree objects added)

	EXPECT_EQ( flowers->Count(), 2 );  // Flower pool should have 2 elements
	EXPECT_EQ( dogs->Count(), 1 );     // Dog pool should have 1 element
}


TEST_F( DataClusterTest, GetPoolID ) {
	cluster.Add<Flower>( "Daisy", Color::Red );
	cluster.Add<Dog>( "Rover" );
	cluster.Add<Tree>( "Maple" );

	// IDs should be in the order of creation
	EXPECT_EQ( cluster.GetPoolID<Flower>(), 0 );
	EXPECT_EQ( cluster.GetPoolID<Dog>(), 1 );
	EXPECT_EQ( cluster.GetPoolID<Tree>(), 2 );
}

TEST_F( DataClusterTest, ReservePool ) {

	cluster.ReservePool<Dog>( 5 );

	EXPECT_TRUE( cluster.DoesPoolExist<Dog>() );

	const DataPool<Dog>* pool = cluster.GetPool<Dog>();

	EXPECT_EQ( pool->Capacity(), 5 );

	// Try reserving the same pool again with a different capacity
	const DataPool<Dog>* secondReserve = cluster.ReservePool<Dog>( 10 );
	EXPECT_EQ( secondReserve, nullptr );  // Should not allow re-reservation
}

TEST_F( DataClusterTest, DoesPoolExist ) {
	cluster.Add<Flower>( "Lily", Color::Blue );
	cluster.Add<Dog>( "Rover" );

	EXPECT_TRUE( cluster.DoesPoolExist<Flower>() );
	EXPECT_TRUE( cluster.DoesPoolExist<Dog>() );
	EXPECT_FALSE( cluster.DoesPoolExist<Tree>() );
}

TEST_F( DataClusterTest, PoolCount ) {
	cluster.Add<Flower>( "Daisy", Color::Red );
	cluster.Add<Dog>( "Rover" );
	cluster.Add<Tree>( "Maple" );

	EXPECT_EQ( cluster.PoolCount(), 3 );

	cluster.Add<Plant>( "Big Plant" );
	cluster.Add<Organism>( "Unknown" );

	EXPECT_EQ( cluster.PoolCount(), 5 );
}

TEST_F( DataClusterTest, Polymorphism ) {
	cluster.Add<Flower>( "Daisy", Color::Red );
	cluster.Add<Dog>( "Rover" );
	cluster.Add<Tree>( "Maple" );
	cluster.Add<Plant>( "Big Plant" );
	cluster.Add<Organism>( "Unknown" );

	// Each type should have its own pool
	EXPECT_TRUE( cluster.DoesPoolExist<Flower>() );
	EXPECT_TRUE( cluster.DoesPoolExist<Dog>() );
	EXPECT_TRUE( cluster.DoesPoolExist<Tree>() );
	EXPECT_TRUE( cluster.DoesPoolExist<Plant>() );
	EXPECT_TRUE( cluster.DoesPoolExist<Organism>() );

	// Verify that each pool has exactly one object
	EXPECT_EQ( cluster.GetPool<Flower>()->Count(), 1 );
	EXPECT_EQ( cluster.GetPool<Dog>()->Count(), 1 );
	EXPECT_EQ( cluster.GetPool<Tree>()->Count(), 1 );
	EXPECT_EQ( cluster.GetPool<Plant>()->Count(), 1 );
	EXPECT_EQ( cluster.GetPool<Organism>()->Count(), 1 );
}

TEST_F( DataClusterTest, PoolCapacityLimit ) {
	cluster.ReservePool<Dog>( 2 );  // Reserve a pool with capacity 2

	auto* dog1 = cluster.Add<Dog>( "Rover" );
	auto* dog2 = cluster.Add<Dog>( "Buddy" );

	EXPECT_NE( dog1, nullptr );  // Should succeed
	EXPECT_NE( dog2, nullptr );  // Should succeed

	// Adding a third dog should fail, since the pool is full
	auto* dog3 = cluster.Add<Dog>( "Max" );
	EXPECT_EQ( dog3, nullptr );  // Pool is at capacity
}

TEST_F( DataClusterTest, RemoveObjects ) {
	auto* flower1 = cluster.Add<Flower>( "Daisy", Color::Red );
	auto* flower2 = cluster.Add<Flower>( "Lily", Color::Blue );

	EXPECT_EQ( cluster.GetPool<Flower>()->Count(), 2 );

	cluster.Remove( flower1 );

	EXPECT_EQ( cluster.GetPool<Flower>()->Count(), 1 );  // One object remains
	EXPECT_EQ( flower1->PoolID(), 65535 );  // Ensure ID is invalidated

	cluster.Remove( flower2 );
	EXPECT_EQ( cluster.GetPool<Flower>()->Count(), 0 );  // No objects left
}

TEST_F( DataClusterTest, InvalidPoolAccess ) {
	const auto* flowerPool = cluster.GetPool<Flower>();
	EXPECT_EQ( flowerPool, nullptr );  // No pool should exist for Flower initially

	cluster.Add<Dog>( "Rover" );

	const auto* treePool = cluster.GetPool<Tree>();
	EXPECT_EQ( treePool, nullptr );  // No Tree pool exists yet
}


TEST_F( DataClusterTest, StressTest ) {
	cluster.ReservePool<Dog>( 1000 );

	for (int i = 0; i < 1000; ++i) {
		cluster.Add<Dog>( "Dog" + std::to_string( i ) );
	}

	EXPECT_EQ( cluster.GetPool<Dog>()->Count(), 1000 );  // All dogs should be added successfully
}


TEST_F( DataClusterTest, ReuseFreedSlots ) {
	cluster.ReservePool<Dog>( 3 );

	auto* dog1 = cluster.Add<Dog>( "Rover" );
	auto* dog2 = cluster.Add<Dog>( "Buddy" );
	cluster.Remove( dog1 );

	// After removing Rover, the slot should be reusable
	auto* dog3 = cluster.Add<Dog>( "Max" );

	EXPECT_NE( dog3, nullptr );
	EXPECT_EQ( cluster.GetPool<Dog>()->Count(), 2 );  // Still 2 dogs in the pool
}

TEST_F( DataClusterTest, RemoveInvalidObject ) {
	cluster.ReservePool<Dog>( 2 );

	auto* dog1 = cluster.Add<Dog>( "Rover" );
	cluster.Remove( dog1 );

	// Attempt to remove the object again, which should be invalid
	EXPECT_DEATH( cluster.Remove( dog1 ), ".*Invalid poolID.*" );

}
