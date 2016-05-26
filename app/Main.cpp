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

	Transform* tst0 = pool.insert(0, Transform(123.f, 456.f));
	Transform* tst1 = pool.insert(1, Transform(789.f, 101112.f));

	stopFor(pool);

	pool.erase(tst0);
	pool.erase<Transform>(1);

	stopFor(pool);

	pool.insert(16, Transform(99999.f, 99999.f));

	stopFor(pool);
	
	return 0;
}