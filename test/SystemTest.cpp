#include <gtest\gtest.h>

#include "Engine.hpp"

#include "Scripting.hpp"

class SystemTest : public ::testing::Test{
protected:
	Engine engine;

	void SetUp() override{
		//engine.addSystem<Scripting>(0, 0);

		engine.load();
	}
};

TEST_F(SystemTest, updating){
	for (unsigned int i = 0; i < 1000; i++)
		engine.update();
}

TEST_F(SystemTest, updating2){
	for (unsigned int i = 0; i < 1000; i++)
		engine.update();

	std::cin.get();
}