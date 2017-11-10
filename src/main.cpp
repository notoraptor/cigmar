#include <iostream>
#include <cigmar/classes/String.hpp>
#include <cigmar/unittests.hpp>
#include <cigmar/hex.hpp>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using namespace cigmar;

/*Notes: write a long multi-lines raw string in C++11+:

const char* jstring = R"(
{
"a": true,
"b": 1,
"salue": -2.9,
"hello": "merci"
}
)";

*/

int main() {
	tests::run();
	return 0;
}
