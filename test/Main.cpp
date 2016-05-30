#include "TestUtils.hpp"
#include "ObjectPool.hpp"
#include "Component.hpp"

#include <iostream>

void stopFor(const ObjectPool& pool){
	system("cls");
	pool.print();
	std::cin.get();
}

int main(int argc, char* argv[]){
	ObjectPool pool(sizeof(Transform));

	stopFor(pool);

	Transform* tst0 = pool.insert(0, Transform(123.f, 456.f));
	Transform* tst1 = pool.insert(1, Transform(789.f, 101112.f));

	stopFor(pool);

	pool.erase(tst0);
	pool.erase<BaseComponent>(1);

	stopFor(pool);

	pool.insert(16, Transform(99999.f, 99999.f));

	stopFor(pool);

	return 0;
}