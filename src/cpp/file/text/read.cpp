//
// Created by notoraptor on 17/11/2017.
//

#include <cigmar/file/text/read.hpp>
#include <cigmar/file/Lines.hpp>

namespace cigmar::file::text {
	String read(const char* filename) {
		String text;
		file::Lines file(filename);
		for (const String& line : file) {
			text << line;
		}
		return text;
	};
	String read(const std::string& filename){
		String text;
		file::Lines file(filename);
		for (const String& line : file) text << line;
		return text;
	};
	String read(const String& filename) {
		String text;
		file::Lines file(filename);
		for (const String& line : file) text << line;
		return text;
	};
}
