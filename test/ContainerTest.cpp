#include <gtest\gtest.h>

#include "TestComponent.hpp"

#include "ObjectPool.hpp"
#include <vector>
#include <random>

#define SIZE 4096
#define OBJECTS 1024
#define ITERATIONS 1024 * 64
#define GETS 1024 * 64

class ContainerTest : public ::testing::Test{
protected:
	BasePool* pool;
	std::vector<BaseComponent*>* vector;

	using TestComponent = TestComponent<SIZE>;

	void SetUp() override{
		pool = new ObjectPool<TestComponent>(TestComponent::chunk());
		vector = new std::vector<BaseComponent*>;
	
		for (unsigned int i = 0; i < OBJECTS; i++){
			pool->insert<TestComponent>(i);
		}

		for (unsigned int i = 0; i < OBJECTS; i++){
			vector->insert(vector->end(), new TestComponent);
		}

		srand(time(0));
	}

	void TearDown() override{
		delete pool;

		for (auto component : *vector){
			if (component)
				delete component;
		}

		delete vector;
	}
};

TEST_F(ContainerTest, Vector_Iteration){
	for (unsigned int x = 0; x < ITERATIONS; x++){
		for (unsigned int i = 0; i < OBJECTS; i++){
			memset((void*)static_cast<TestComponent*>(vector->at(i))->memory, rand(), SIZE);
		}
	}
}

TEST_F(ContainerTest, ObjectPool_Iteration){
	for (unsigned int x = 0; x < ITERATIONS; x++){
		for (unsigned int i = 0; i < OBJECTS; i++){
			memset((void*)pool->get<TestComponent>(i)->memory, rand(), SIZE);
		}
	}
}

TEST_F(ContainerTest, ObjectPool_Gets){
	for (unsigned int x = 0; x < GETS; x++){
		unsigned int i = rand() % OBJECTS;
		memset((void*)pool->get<TestComponent>(i)->memory, rand(), SIZE);
	}
}

TEST_F(ContainerTest, Vector_Gets){
	for (unsigned int x = 0; x < GETS; x++){
		unsigned int i = rand() % OBJECTS;
		memset((void*)static_cast<TestComponent*>(vector->at(i))->memory, rand(), SIZE);
	}
}

TEST(DEBUG, END){
	getchar();
}