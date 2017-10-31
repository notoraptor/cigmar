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

int main() {
	// sys::println(sys::run("python -c 'import sys; print(len(sys.argv));'"));
	tests::run();
	return 0;
}
