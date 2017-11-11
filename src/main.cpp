#include <iostream>
#include <cigmar/classes/String.hpp>
#include <cigmar/unittests.hpp>
#include <cigmar/print.hpp>
#include <cigmar/classes/Exception.hpp>
#include <cigmar/interfaces/Streamable.hpp>

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

class GenericString: public Streamable {
private:
	enum class DataType {STRING, CSTRING, CPPSTRING};
	union Data {
		const char* cstring;
		String string;
		std::string cppstring;
		explicit Data(const char* s) : cstring(s) {};
		explicit Data(const String& s): string(s) {};
		explicit Data(const std::string& s): cppstring(s) {};
		explicit Data(String&& s) noexcept : string(std::move(s)) {};
		explicit Data(std::string&& s) noexcept : cppstring(std::move(s)) {};
		~Data() {}
	} data;
	DataType type;
public:
	GenericString(const String& s): data(s), type(DataType::STRING) {};
	GenericString(const std::string& s): data(s), type(DataType::CPPSTRING) {};
	GenericString(const char* s): data(s), type(DataType::CSTRING) {};
	GenericString(String&& s): data(std::move(s)), type(DataType::STRING) {};
	GenericString(std::string&& s): data(std::move(s)), type(DataType::CPPSTRING) {};
	~GenericString() {
		switch (type) {
			case DataType::STRING:
				data.string.~String();
				sys::err::println("Destroyed string member.");
				break;
			case DataType::CPPSTRING:
				data.cppstring.~string();
				sys::err::println("Destroyed C++ string member.");
				break;
			case DataType::CSTRING:
				sys::err::println("No destruction needed for c-string.");
				break;
		}
	}
	bool isCString() const {return type == DataType::CSTRING;};
	bool isCppString() const {return type == DataType::CPPSTRING;};
	bool isString() const {return type == DataType::STRING;};
	String& getString() {
		if (!isString())
			throw Exception("not a String.");
		return data.string;
	}
	std::string& getCppString() {
		if (!isCppString())
			throw Exception("Not a C++ string.");
		return data.cppstring;
	}
	const char* getCString() const {
		if (!isCString())
			throw Exception("Not a C-string.");
		return data.cstring;
	}
	const String& getString() const {
		if (!isString())
			throw Exception("not a String.");
		return data.string;
	}
	const std::string& getCppString() const {
		if (!isCppString())
			throw Exception("Not a C++ string.");
		return data.cppstring;
	}
	String toString() const {
		switch (type) {
			case DataType::CSTRING:
				return String(data.cstring);
				break;
			case DataType::STRING:
				return data.string;
				break;
			case DataType::CPPSTRING:
				return String(data.cppstring);
				break;
		}
	}
	std::string toCppString() const {
		switch (type) {
			case DataType::CSTRING:
				return std::string(data.cstring);
				break;
			case DataType::STRING:
				return data.string.cppstring();
				break;
			case DataType::CPPSTRING:
				return data.cppstring;
				break;
		}
	}
	const char* toCString() const {
		switch (type) {
			case DataType::CSTRING:
				return data.cstring;
				break;
			case DataType::STRING:
				return data.string.cstring();
				break;
			case DataType::CPPSTRING:
				return data.cppstring.c_str();
				break;
		}
	}
	void toStream(std::ostream& o) const override {
		switch (type) {
			case DataType::STRING:
				data.string.toStream(o);
				break;
			case DataType::CSTRING:
				o << data.cstring;
				break;
			case DataType::CPPSTRING:
				o << data.cppstring;
				break;
		}
	}
};

void check(const GenericString& g) {
	if (g.isString()) sys::err::print("is a String: ");
	if (g.isCString()) sys::err::print("is a C-string: ");
	if (g.isCppString()) sys::err::print("is a C++ string: ");
	sys::err::println(g);
}

int main() {
	check("I am a C-string.");
	check(String("I am a Cigmar String."));
	check(std::string("I am a C++ string."));
	tests::run();
	return 0;
}

