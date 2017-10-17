#ifndef CIGMAR_UNITTESTS
#define CIGMAR_UNITTESTS

#include <functional>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <cigmar/numbers.hpp>

namespace cigmar {

typedef std::function<void(void**)> UnitTestMethod;
typedef std::function<void(void*)> TestFunction;
typedef std::function<void(void)> Procedure;
typedef std::map<std::string, TestFunction> TestList;
typedef std::map<std::string, TestFunction>::iterator TestListIterator;

class UnitTest {
protected:
	std::string name;
	TestList tests;
	UnitTestMethod initializer;
	UnitTestMethod destructor;
	void* data;
public:
	bool unique = true;
public:
	UnitTest(const std::string& className):
		name(className), tests(), initializer(nullptr), destructor(nullptr), data(nullptr) {}
	~UnitTest() {
		if (destructor) destructor(&data);
	}
	const std::string& getName() const {return name;}
	size_t size() const {return tests.size();}
	void run() {
		for (TestListIterator it = tests.begin(); it != tests.end(); ++it) {
			try {
				std::cerr << name << '.' << it->first << " ...";
				it->second(data);
				std::cerr << " ok" << std::endl;
			} catch (std::string& s) {
				std::ostringstream os;
				std::cerr << " ERROR" << std::endl;
				os << "ERROR: " << name << '.' << it->first << std::endl << s;
				throw os.str();
			} catch (...) {
				std::cerr << " ERROR" << std::endl << std::endl;
				std::cerr << "ERROR: " << name << '.' << it->first << std::endl;
				throw;
			}
		}
		if (unique) {
			size_t test_size = tests.size();
			std::cerr << std::endl << "Ran " << test_size << (test_size == 1 ? " test" : " tests") << std::endl;
			std::cerr << std::endl << "OK" << std::endl;
		}
	}
	void operator()() {
		try {
			run();
		} catch (std::string& s) {
			std::cerr << std::endl << s << std::endl;
			exit(EXIT_FAILURE);
		} catch(std::exception& e) {
			std::cerr << std::endl << e.what() << std::endl;
			exit(EXIT_FAILURE);
		} catch (...) {
			std::cerr << std::endl << "(unknown exception)" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
};

void recordTest(UnitTest* test);
void runTests();

struct Executor {
	Executor(Procedure procedure) {
		procedure();
	};
};

inline void utt_assert_bool(bool condition, const char* else_message = nullptr) {
	if (!condition) {
		std::ostringstream s;
		s << "Error occurred with seed " << numbers::rng.seed() << std::endl;
		if (else_message)
			s << "Assertion failed:" << std::endl << else_message;
		else
			s << "Assertion failed.";
		throw s.str();
	}
}

#define utt_assert(condition_code) utt_assert_bool((bool)(condition_code), #condition_code)
#define utt_begin(name) namespace name {class name: public UnitTest {public: name(): UnitTest(#name) {
#define utt_init() ; initializer = [](void** data)
#define utt_clear() ; destructor = [](void** data)
#define utt(name) ; tests[ #name ] = [](void* data)
#define utt_end() ; if (initializer) initializer(&data);}} testInstance; \
	Executor recorder([]() {recordTest(&testInstance);}); };

}

#endif // CIGMAR_UNITTESTS
