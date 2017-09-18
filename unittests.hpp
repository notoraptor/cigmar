#ifndef CIGMAR_UNITTESTS
#define CIGMAR_UNITTESTS

#include <functional>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
using std::map;
using std::ostringstream;
using std::string;
using std::cerr;
using std::endl;

typedef std::function<void(void**)> UnitTestMethod;
typedef std::function<void(void*)> TestFunction;

inline void utt_assert_bool(bool condition, const char* else_message = nullptr) {
	if (!condition) {
		ostringstream s;
		s << "Error occurred with seed " << numbers::rng.seed() << endl;
		if (else_message)
			s << "Assertion failed:" << endl << else_message;
		else
			s << "Assertion failed.";
		throw s.str();
	}
}

#define utt_assert(condition_code) utt_assert_bool((bool)(condition_code), #condition_code)

class UnitTest {
protected:
	string name;
	map<string, TestFunction> tests;
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
		for (map<string, TestFunction>::iterator it = tests.begin(); it != tests.end(); ++it) {
			try {
				cerr << name << '.' << it->first << " ...";
				it->second(data);
				cerr << " ok" << endl;
			} catch (string& s) {
				ostringstream os;
				cerr << " ERROR" << endl;
				os << "ERROR: " << name << '.' << it->first << endl << s;
				throw os.str();
			} catch (...) {
				cerr << " ERROR" << endl << endl;
				cerr << "ERROR: " << name << '.' << it->first << endl;
				throw;
			}
		}
		size_t test_size = tests.size();
		cerr << endl << "Ran " << test_size << (test_size == 1 ? " test" : " tests") << endl;
		cerr << endl << "OK" << endl;
	}
	void operator()() {
		try {
			run();
		} catch (string& s) {
			cerr << endl << s << endl;
			exit(EXIT_FAILURE);
		} catch(std::exception& e) {
			cerr << endl << e.what() << endl;
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

#endif // CIGMAR_UNITTESTS
