//
// Created by notoraptor on 20/02/2018.
//

#ifndef SRC_CIGMAR_INTERNAL_DEFINITIONS_HPP
#define SRC_CIGMAR_INTERNAL_DEFINITIONS_HPP

#include <ostream>

namespace cigmar {
	/** Primitive types. **/
	using byte_t = int8_t;
	using ubyte_t = uint8_t;

	/** Classes for unique special symbols. **/
	class last_t {} LAST;

	/** Global methods. **/
	template <typename C>
	std::basic_ostream<C>& operator<<(std::basic_ostream<C>& o, byte_t b) {
		return (o << (int) b);
	}
	template <typename C>
	std::basic_ostream<C>& operator<<(std::basic_ostream<C>& o, ubyte_t b) {
		return (o << (unsigned int) b);
	}
}

#endif //SRC_CIGMAR_INTERNAL_DEFINITIONS_HPP
