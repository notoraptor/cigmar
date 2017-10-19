#include <cstring>
#include <iostream>
#include <cigmar/time.hpp>
#include <cigmar/classes/Timer.hpp>
#include <cigmar/unittests.hpp>

namespace cigmar {

class UnitTestArray {
private:
	UnitTest** array;
	size_t length;
	size_t count;
public:
	void init() {
		count = 0;
		length = 10;
		array = (UnitTest**)malloc(sizeof(UnitTest*) * length);
	}
	void add(UnitTest& ptr) {
		if (count == length) {
			size_t new_length = 2 * length;
			UnitTest** new_array = (UnitTest**)malloc(sizeof(UnitTest*) * new_length);
			memcpy(new_array, array, sizeof(UnitTest*) * length);
			free(array);
			array = new_array;
			length = new_length;
		}
		ptr.unique = false;
		array[count] = &ptr;
		++count;
	}
	UnitTest** begin() {return array;}
	UnitTest** end() {return (array + count);}
	~UnitTestArray() {free(array);}
};

UnitTestArray testMap;
bool testMapInitialized = false;

inline void initTestMap() {
	if (!testMapInitialized) {
		testMap.init();
		testMapInitialized = true;
	}
}

namespace tests {

void record(UnitTest& test) {
	initTestMap();
	testMap.add(test);
}

void run() {
	initTestMap();
	size_t testsCount = 0;
	Timer timer;
	timer.start();
	for (UnitTest* test: testMap) {
		(*test)();
	}
	double elapsedSeconds = timer.microseconds() * 1e-6;
	for (UnitTest* test: testMap)
		testsCount += test->size();
	std::cerr << std::endl << "Ran " << testsCount << " test" << (testsCount == 1 ? "" : "s") << " in " << elapsedSeconds << 's' << std::endl;
	std::cerr << std::endl << "OK" << std::endl;
}
}

}
