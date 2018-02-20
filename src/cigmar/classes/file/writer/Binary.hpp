//
// Created by HPPC on 12/11/2017.
//

#ifndef SRC_CIGMAR_BINARY_HPP
#define SRC_CIGMAR_BINARY_HPP

#include <fstream>
#include <cigmar/classes/String.hpp>
#include <cigmar/interfaces/Streamable.hpp>
#include <cigmar/classes/exception/Exception.hpp>

namespace cigmar::file::writer {
	class Binary {
	private:
		std::ofstream ofs;
	public:
		explicit Binary(const char* filename): ofs(filename, std::ios::out | std::ios::binary) {
			if (!ofs.is_open()) throw Exception("Unable to open file (mode write|binary):", filename);
		};
		explicit Binary(const std::string& filename): ofs(filename, std::ios::out | std::ios::binary) {
			if (!ofs.is_open()) throw Exception("Unable to open file (mode write):", filename);
		};
		explicit Binary(const String& filename): ofs(filename.cppstring(), std::ios::out | std::ios::binary) {
			if (!ofs.is_open()) throw Exception("Unable to open file (mode write):", filename);
		};
		Binary& write(const char* data, size_t size) {
			ofs.write(data, size);
			return *this;
		};
		template<typename T> Binary& operator<<(const T& value) {
			ofs << value;
			return *this;
		};
		template<typename T> Binary& operator<<(T&& value) {
			ofs << std::move(value);
			return *this;
		};
	};
}

#endif //SRC_CIGMAR_BINARY_HPP
