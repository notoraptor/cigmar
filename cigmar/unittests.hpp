#ifndef CIGMAR_UNITTESTS
#define CIGMAR_UNITTESTS

#include <functional>
#include <iostream>
#include <string>
#include <sstream>
#include <map>

namespace cigmar {

typedef std::function<void(void**)> UnitTestMethod;
typedef std::function<void(void*)> TestFunction;

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

class UnitTest {
protected:
	std::string name;
	std::map<std::string, TestFunction> tests;
	UnitTestMethod initializer;
	UnitTestMethod destructor;
	void* data;
public:
	UnitTest(const char* uttName):
		name(uttName), tests(), initializer(nullptr), destructor(nullptr), data(nullptr) {}
	~UnitTest() {
		if (destructor) destructor(&data);
	}
	void run() {
		for (std::map<std::string, TestFunction>::iterator it = tests.begin(); it != tests.end(); ++it) {
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
		size_t test_size = tests.size();
		std::cerr << std::endl << "Ran " << test_size << (test_size == 1 ? " test" : " tests") << std::endl;
		std::cerr << std::endl << "OK" << std::endl;
	}
	void operator()() {
		try {
			run();
		} catch (std::string& s) {
			std::cerr << std::endl << s << std::endl;
			exit(EXIT_FAILURE);
		} catch(std::exception& e) {
			std::cerr << std::endl << e.what() << std::endl;
		} catch (...) {
			exit(EXIT_FAILURE);
		}
	}
};

#define utt_begin(name) \
class name: public UnitTest { \
public: \
	name(): UnitTest(#name) {

#define utt_init() ; initializer = [](void** data)
#define utt_clear() ; destructor = [](void** data)
#define utt(name) ; tests[ #name ] = [](void* data)

#define utt_end() \
		; if (initializer) initializer(&data); \
	} \
};

}

#endif // CIGMAR_UNITTESTS
