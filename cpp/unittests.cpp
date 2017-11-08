//
// Created by notoraptor on 08/11/2017.
//
#include <iostream>
#include <cigmar/unittests.hpp>
#include <cigmar/classes/TreeMap.hpp>
#include <cigmar/classes/String.hpp>
#include <cigmar/classes/Timer.hpp>

namespace cigmar::tests {
	typedef TreeMap<String, UttMethod> TestList;

	template<typename T> class Dynamic {
	public:
		T* pointer;
	public:
		~Dynamic() {
			std::cerr << "End testing." << std::endl;
			delete pointer;
		}
	};

	static Dynamic<TestList> tests;
	static bool initialized = false;

	static void init() {
		if (!initialized) {
			tests.pointer = new TestList();
			initialized = true;
		}
	}

	UttRecorder::UttRecorder(UttMethod method, const char *className, const char *methodName) {
		init();
		TestList& treemap = *tests.pointer;
		String s(className, ".", methodName);
		treemap[s] = method;
	}

	void run() {
		init();
		TestList& treemap = *tests.pointer;
		Timer timer;
		timer.start();
		for (TestList::iterator_t it = treemap.begin(); it != treemap.end(); ++it) {
			// catch
			try {
				// turn
				try {
					std::cerr << it->first << " ...";
					it->second();
					std::cerr << " ok" << std::endl;
				} catch (std::string &s) {
					std::ostringstream os;
					std::cerr << " ERROR" << std::endl;
					os << "ERROR: " << it->first << std::endl << s;
					throw os.str();
				} catch (...) {
					std::cerr << " ERROR" << std::endl << std::endl;
					std::cerr << "ERROR: " << it->first << std::endl;
					throw;
				}
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
		double elapsedSeconds = timer.microseconds() * 1e-6;
		size_t testsCount = treemap.size();
		std::cerr << std::endl << "Ran " << testsCount << " test" << (testsCount == 1 ? "" : "s") << " in " << elapsedSeconds << 's' << std::endl;
		std::cerr << std::endl << "OK" << std::endl;
	}

}
