#include <iostream>
#include <sstream>
#include <string>
#include "utils.hpp"
using std::cout;
using std::boolalpha;
using std::endl;

class CigmarInit {
public:
	CigmarInit() {
		std::cout << std::boolalpha;
		std::cerr << std::boolalpha;
	}
} cigmar_init;

int main() {
	numbers::rng.seed(3325324192);
	array_t<unsigned int, 31> arr;
	numbers::random::normal(arr, 10u, 5u);
	cout << '[' << arr.length << ']' << endl;
	for (size_t i = 0; i < arr.length; ++i) {
		cout << '(' << i << ") " << arr[i] << endl;
	}
	TreeSet<unsigned int> set(arr);
	cout << '{';
	for (unsigned int x: set) cout << x << ';';
	cout << '}' << endl;
	cout << set.contains(20) << endl;
	cout << "seed: " << numbers::rng.seed() << endl;
	return 0;
}
