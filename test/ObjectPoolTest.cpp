#include <gtest\gtest.h>

#include "ObjectPool.hpp"

#include "TestHelper.hpp"
#include "TestComponent.hpp"

class ObjectPoolTest : public ::testing::Test{
protected:
	BasePool* pool;

	using TestComponent = TestComponent<4096>;

	void SetUp() override{
		pool = new ObjectPool<TestComponent>(8);
	}

	void TearDown() override{
		delete pool;
	}
};

TEST_F(ObjectPoolTest, allocation){
	//uint32_t index = 0xFFFFF;
	//
	//size_t size = sizeof(TestComponent);
	//
	//TestComponent* component = pool->insert<TestComponent>(index);
	//
	//EXPECT_TRUE(component);
	//
	//EXPECT_EQ(component, pool->get<TestComponent>(index));
	//
	//double begin = TestHelper::randDouble(index, DBL_MAX);
	//double end = TestHelper::randDouble(index, DBL_MAX);
	//
	//component->begin = begin;
	//component->end = end;
	//
	//EXPECT_EQ(pool->get<TestComponent>(index)->begin, begin);
	//EXPECT_EQ(pool->get<TestComponent>(index)->end, end);
	//
	//pool->erase(index);
	//
	//EXPECT_NE(pool->get<TestComponent>(index), nullptr);
}

TEST_F(ObjectPoolTest, reallocation){

}