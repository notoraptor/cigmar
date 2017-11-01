#include <iostream>
#include <cigmar/std.hpp>
#include <cigmar/print.hpp>
#include <cigmar/filesystem.hpp>
#include <cigmar/unittests.hpp>
#include <libraries/json/json.hpp>
#include <cigmar/whirlpool.hpp>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using namespace cigmar;
using json = nlohmann::json;

int main() {
	unsigned char x = 254;
	sys::println((int)x);
	sys::println((int)((int)x >> 4));
	sys::println((int)((int)x ^ ((x >> 4) << 4)));
	sys::println(crypto::hash::whirlpool("test"));
	// tests::run();
	/*
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
	j["ccc"] = -27.56;
	sys::println(j.type() == json::value_t::object);
	sys::println(j.dump(4));
	*/
	return 0;
}
