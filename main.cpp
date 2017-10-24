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
	sys::print("Mon nom est", "personne.");
	sys::print();
	sys::print("J'ai", 22, "ans.");
	sys::err::print("Bonjour", false, true, "Bonsoir.");
	sys::err::print();
	sys::err::print();
	sys::err::print("Salut", '!');
	return 0;
}
