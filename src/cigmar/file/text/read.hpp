//
// Created by notoraptor on 17/11/2017.
//

#ifndef SRC_CIGMAR_READ_HPP
#define SRC_CIGMAR_READ_HPP

#include <cigmar/classes/String.hpp>

namespace cigmar::file::text {
	String read(const char* filename);
	String read(const std::string& filename);
	String read(const String& filename);
}

#endif //SRC_CIGMAR_READ_HPP
