#include <iostream>
#include <cigmar/std.hpp>
#include <cigmar/print.hpp>
#include <cigmar/filesystem.hpp>
#include <cigmar/unittests.hpp>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using namespace cigmar;

// TODO: Exceptions with easy-to-write message strings.

int main() {
	tests::run();
	return 0;
}
