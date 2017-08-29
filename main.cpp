#include <iostream>
#include <sstream>
#include <string>
#include "utils.hpp"
using std::cout;
using std::boolalpha;
using std::endl;

int main() {
    String s = "Bonjour";
    String t = "bonsoir";
    cout << (s < t) << endl;
    cout << (s >= t) << endl;
    cout << (s == "Bonjour") << endl;
    cout << ("Bontour" > s) << endl;
	return 0;
}
