#ifndef CIGMAR_PRINT
#define CIGMAR_PRINT

#include <iostream>
#include <cigmar/interfaces/Streamable.hpp>

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
	inline void writeElement(std::ostream& o) {}
	template<typename T, typename... Args> void writeElement(std::ostream& o, T variable, Args&&... args) {
		AutoStreamer::print(o, variable);
		writeElement(o, std::forward<Args>(args)...);
	}
	inline void printElement(std::ostream& o) {}
	template<typename T, typename... Args> void printElement(std::ostream& o, T variable, Args&&... args) {
		o << ' ';
		AutoStreamer::print(o, variable);
		printElement(o, std::forward<Args>(args)...);
	}
	inline void printFirstElement(std::ostream& o) {}
	template<typename T, typename... Args> void printFirstElement(std::ostream& o, T variable, Args&&... args) {
		AutoStreamer::print(o, variable);
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
		writeElement(std::cout, std::forward<Args>(args)...);
		std::cout << std::endl;
	};
	template<typename... Args> void write(Args&&... args) {
		writeElement(std::cout, std::forward<Args>(args)...);
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
			writeElement(std::cerr, std::forward<Args>(args)...);
			std::cerr << std::endl;
		};
		template<typename... Args> void write(Args&&... args) {
			writeElement(std::cerr, std::forward<Args>(args)...);
		};
	}
}

#endif
