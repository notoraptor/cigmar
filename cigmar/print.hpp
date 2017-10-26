#ifndef CIGMAR_PRINT
#define CIGMAR_PRINT

#include <iostream>

namespace cigmar {
namespace sys {
	template<typename... Args> void print(Args... args);
	template<typename... Args> void print_r(Args... args);
	template<typename... Args> void write(Args... args);
	template<typename... Args> void write_r(Args... args);
	namespace err {
		template<typename... Args> void print(Args... args);
		template<typename... Args> void print_r(Args... args);
		template<typename... Args> void write(Args... args);
		template<typename... Args> void write_r(Args... args);
	}
}
}

namespace cigmar {
namespace sys {
	inline void writeRawElement(std::ostream& o) {}
	template<typename T, typename... Args>
	void writeRawElement(std::ostream& o, T variable, Args... args) {
		o << variable;
		writeRawElement(o, args...);
	}

	inline void printElement(std::ostream& o) {};
	inline void printFirstElement(std::ostream& o) {};
	template<typename T, typename... Args>
	void printElement(std::ostream& o, T variable, Args... args) {
		o << ' ' << variable;
		printElement(o, args...);
	}
	template<typename T, typename... Args>
	void printFirstElement(std::ostream& o, T variable, Args... args) {
		o << variable;
		printElement(o, args...);
	}
	//
	template<typename... Args> void println(Args... args) {
		printFirstElement(std::cout, args...);
		std::cout << std::endl;
	};
	template<typename... Args> void print(Args... args) {
		printFirstElement(std::cout, args...);
	};
	template<typename... Args> void writeln(Args... args) {
		writeRawElement(std::cout, args...);
		std::cout << std::endl;
	};
	template<typename... Args> void write(Args... args) {
		writeRawElement(std::cout, args...);
	};
	namespace err {
		template<typename... Args> void println(Args... args) {
			printFirstElement(std::cerr, args...);
			std::cerr << std::endl;
		};
		template<typename... Args> void print(Args... args) {
			printFirstElement(std::cerr, args...);
		};
		template<typename... Args> void writeln(Args... args) {
			writeRawElement(std::cerr, args...);
			std::cerr << std::endl;
		};
		template<typename... Args> void write(Args... args) {
			writeRawElement(std::cerr, args...);
		};
	}
}
}

#endif
