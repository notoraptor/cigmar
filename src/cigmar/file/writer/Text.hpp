//
// Created by HPPC on 12/11/2017.
//

#ifndef SRC_CIGMAR_FILE_WRITER_TEXT_HPP
#define SRC_CIGMAR_FILE_WRITER_TEXT_HPP

#include <fstream>
#include <cigmar/classes/String.hpp>
#include <cigmar/exception/Exception.hpp>

namespace cigmar::file::writer {
	class Text {
	private:
		std::ofstream ofs;
	public:
		explicit Text(const char* filename): ofs(filename) {
			if (!ofs.is_open()) throw Exception("Unable to open file (mode write):", filename);
		};
		explicit Text(const std::string& filename): ofs(filename) {
			if (!ofs.is_open()) throw Exception("Unable to open file (mode write):", filename);
		};
		explicit Text(const String& filename): ofs(filename.cppstring()) {
			if (!ofs.is_open()) throw Exception("Unable to open file (mode write):", filename);
		};
		template<typename T> Text& operator<<(const T& value) {
			ofs << value;
			return *this;
		};
		template<typename T> Text& operator<<(T&& value) {
			ofs << std::move(value);
			return *this;
		};
	};
}


#endif //SRC_CIGMAR_FILE_WRITER_TEXT_HPP
