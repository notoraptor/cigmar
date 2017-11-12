//
// Created by HPPC on 12/11/2017.
//

#ifndef SRC_CIGMAR_FILE_LINES_HPP
#define SRC_CIGMAR_FILE_LINES_HPP

#include <fstream>
#include <cigmar/classes/String.hpp>
#include <cigmar/classes/Exception.hpp>

namespace cigmar::file {
	class Lines {
	private:
		class LinesIterator {
		private:
			std::ifstream* ptr;
			String line;
		public:
			LinesIterator(): ptr(nullptr), line() {}
			explicit LinesIterator(std::ifstream& ifs): ptr(&ifs), line() {
				ptr->clear();
				ptr->seekg(0, std::ios::beg);
			}
			bool operator==(const LinesIterator& other) {
				return ptr == other.ptr;
			};
			bool operator!=(const LinesIterator& other) {
				return ptr != other.ptr;
			};
			String& operator*() {
				return line;
			}
			LinesIterator& operator++() {
				if (ptr && !std::getline(*ptr, line.cppstring())) {
					if (!ptr->eof()) throw Exception("Unable to reach file end while reading lines.");
					ptr = nullptr;
				}
				return *this;
			};
		};
	public:
		typedef LinesIterator iterator_t;
	private:
		std::ifstream ifs;
	public:
		explicit Lines(const char* filename): ifs(filename, std::ios::in | std::ios::binary) {
			if (!ifs.is_open()) throw Exception("Unable to open file (mode read|binary):", filename);
		};
		explicit Lines(const std::string& filename): ifs(filename, std::ios::in | std::ios::binary) {
			if (!ifs.is_open()) throw Exception("Unable to open file (mode read|binary):", filename);
		};
		explicit Lines(const String& filename): ifs(filename.cppstring(), std::ios::in | std::ios::binary) {
			if (!ifs.is_open()) throw Exception("Unable to open file (mode read|binary):", filename);
		};
		iterator_t begin() {return LinesIterator(ifs);}
		iterator_t end() {return LinesIterator();}
	};
}

#endif //SRC_CIGMAR_FILE_LINES_HPP
