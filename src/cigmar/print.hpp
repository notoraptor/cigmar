#ifndef CIGMAR_PRINT
#define CIGMAR_PRINT

#include <iostream>

namespace cigmar::sys {
	template<typename... Args> void println(Args&&... args);
	template<typename... Args> void print(Args&&... args);
	template<typename... Args> void writeln(Args&&... args);
	template<typename... Args> void write(Args&&... args);
	namespace err {
		template<typename... Args> void println(Args&&... args);
		template<typename... Args> void print(Args&&... args);
		template<typename... Args> void writeln(Args&&... args);
		template<typename... Args> void write(Args&&... args);
	}
}

namespace cigmar::sys {
	inline void writeRawElement(std::ostream& o) {}
	inline void printElement(std::ostream& o) {}
	inline void printFirstElement(std::ostream& o) {}
	template<typename T, typename... Args>
	void writeRawElement(std::ostream& o, T&& variable, Args&&... args) {
		o << variable;
		writeRawElement(o, std::forward<Args>(args)...);
	}
	template<typename... Args>
	void writeRawElement(std::ostream& o, bool&& variable, Args&&... args) {
		o << (variable ? "true" : "false");
		writeRawElement(o, std::forward<Args>(args)...);
	}
	template<typename T, typename... Args>
	void printElement(std::ostream& o, T&& variable, Args&&... args) {
		o << ' ' << variable;
		printElement(o, std::forward<Args>(args)...);
	}
	template<typename... Args>
	void printElement(std::ostream& o, bool&& variable, Args&&... args) {
		o << ' ' << (variable ? "true" : "false");
		printElement(o, std::forward<Args>(args)...);
	}
	template<typename T, typename... Args>
	void printFirstElement(std::ostream& o, T&& variable, Args&&... args) {
		o << variable;
		printElement(o, std::forward<Args>(args)...);
	}
	template<typename... Args>
	void printFirstElement(std::ostream& o, bool&& variable, Args&&... args) {
		o << (variable ? "true" : "false");
		printElement(o, std::forward<Args>(args)...);
	}
	//
	template<typename... Args> void println(Args&&... args) {
		printFirstElement(std::cout, std::forward<Args>(args)...);
		std::cout << std::endl;
	};
	template<typename... Args> void print(Args&&... args) {
		printFirstElement(std::cout, std::forward<Args>(args)...);
	};
	template<typename... Args> void writeln(Args&&... args) {
		writeRawElement(std::cout, std::forward<Args>(args)...);
		std::cout << std::endl;
	};
	template<typename... Args> void write(Args&&... args) {
		writeRawElement(std::cout, std::forward<Args>(args)...);
	};
	namespace err {
		template<typename... Args> void println(Args&&... args) {
			printFirstElement(std::cerr, std::forward<Args>(args)...);
			std::cerr << std::endl;
		};
		template<typename... Args> void print(Args&&... args) {
			printFirstElement(std::cerr, std::forward<Args>(args)...);
		};
		template<typename... Args> void writeln(Args&&... args) {
			writeRawElement(std::cerr, std::forward<Args>(args)...);
			std::cerr << std::endl;
		};
		template<typename... Args> void write(Args&&... args) {
			writeRawElement(std::cerr, std::forward<Args>(args)...);
		};
	}
}

#endif
