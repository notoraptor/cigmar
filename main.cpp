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
	String s;
	bool ran = sys::run("gcc -v", s);
	if (ran)
		cout << s << endl;
	else
		cerr << "unable to run command." << endl;

	/*
	sys::println("Nom nom est", "Steven", "et", "j'ai", 22, "ans.");
	sys::writeln("Nom nom est", "Steven", "et", "j'ai", 22, "ans.");
	sys::print("Nom nom est", "Steven", "et", "j'ai", 22, "ans.");
	sys::write("Nom nom est", "Steven", "et", "j'ai", 22, "ans.");
	sys::println();

	sys::err::println("Nom nom est", "Steven", "et", "j'ai", 22, "ans.");
	sys::err::writeln("Nom nom est", "Steven", "et", "j'ai", 22, "ans.");
	sys::err::print("Nom nom est", "Steven", "et", "j'ai", 22, "ans.");
	sys::err::write("Nom nom est", "Steven", "et", "j'ai", 22, "ans.");
	sys::err::println();

	tests::run();
	*/

	return 0;
}
