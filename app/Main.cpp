#include "ObjectPool.hpp"
#include "Component.hpp"

#include <iostream>

using namespace std;

struct Transform : public Component<Transform>{
	Transform(float x = 0.f, float y = 0.f) : x(x), y(y){}

	void print(){
		cout << "X " << x << "\tY " << y << "\n";
	}

	float x;
	float y;
};

void stopFor(const ObjectPool& pool){
	system("cls");
	pool.print();
	getchar();
}

int main(int argc, char* argv[]){
	ObjectPool pool(sizeof(Transform));

	stopFor(pool);

	uint64_t id0 = pool.insert(Transform(99999.f, 99999.f));
	uint64_t id1 = pool.insert(Transform(99999.f, 99999.f));
	
	stopFor(pool);
	
	pool.erase<Transform>(id0);
	
	pool.get<Transform>(id1)->y = 753.f;
	pool.get<Transform>(id1)->x = 1.f;

	stopFor(pool);

	for (unsigned int i = 0; i < 16; i++){
		pool.insert(Transform(0.f, 0.f));
		stopFor(pool);
	}
	
	stopFor(pool);

	return 0;
}