#include "StaticEngine.hpp"

#include "Scripting.hpp"

int main(int argc, char* argv[]){
	StaticEngine::get().addSystem<Scripting>();

	return StaticEngine::get().run(argc, argv);
}