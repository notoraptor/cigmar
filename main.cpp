#include <iostream>
#include <sstream>
#include <string>
#include "utils.hpp"
using namespace std;

int main() {
	cout << boolalpha;
	array_t<int, 24> l;
	int start = 0;
	int sum = 0;
	size_t prod = 1;
	l.each([&start](int& i) {i = ++start;});
	l.each([&sum](int& i) {sum += i;});
	l.each([&prod](int& i) {prod *= i;});
	cout << "hello" << endl;
	l.each([](int& i) {cout << i << endl;});
	cout << "sum:  " << sum  << endl;
	cout << "prod: " << prod << endl;
	return 0;
}
