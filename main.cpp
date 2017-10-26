#include <iostream>
#include <string>
#include <cigmar/std.hpp>
#include <cigmar/unittests.hpp>
#include <cigmar/print.hpp>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using namespace cigmar;

// TODO: Exceptions with easy-to-write message strings.


int main() {
	// tests::run();
	sys::println("Nom nom est", "Steven", "et", "j'ai", 22, "ans.");
	sys::writeln("Nom nom est", "Steven", "et", "j'ai", 22, "ans.");
	sys::print("Nom nom est", "Steven", "et", "j'ai", 22, "ans.");
	sys::write("Nom nom est", "Steven", "et", "j'ai", 22, "ans.");

	sys::err::println("Nom nom est", "Steven", "et", "j'ai", 22, "ans.");
	sys::err::writeln("Nom nom est", "Steven", "et", "j'ai", 22, "ans.");
	sys::err::print("Nom nom est", "Steven", "et", "j'ai", 22, "ans.");
	sys::err::write("Nom nom est", "Steven", "et", "j'ai", 22, "ans.");
	return 0;
}
