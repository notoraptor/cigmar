#include <iostream>
#include <sstream>
#include <string>
#include "utils.hpp"
using std::cout;
using std::endl;

int main() {
	String s = "Bonjour, mon nom est Steven, j'ai 27 ans et je vis a Montreal.";
	auto&& x = s.split(",");
	auto&& y = s.split(" ");
	cout << "(" << x.size() << ")" << endl << endl;
	cout << "[" << y.size() << "]" << endl;
	return 0;
}
