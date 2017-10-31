#include <iostream>
#include <cigmar/std.hpp>
#include <cigmar/print.hpp>
#include <cigmar/filesystem.hpp>
#include <cigmar/unittests.hpp>
#include <libraries/json/json.hpp>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using namespace cigmar;
using json = nlohmann::json;

int main() {
	// tests::run();
	const char* jstring = R"(
{
"a": true,
"b": 1,
"salue": -2.9,
"hello": "merci"
}
)";
	json j = json::parse(jstring);
	sys::println(j);
	sys::println(j.size());
	sys::println(j["a"].get<bool>());
	return 0;
}
