#include <locale>
#include "utils.hpp"

/** Initialization of global symbols and variables. **/

_LAST LAST;

CigmarInit cigmar_init;

namespace numbers {
    RNG rng;
}

const size_t String::UNKNOWN = std::string::npos;

/** Others définitions. **/

/// Global variables.

static std::locale loc;

/// Global functions.

std::ostream& operator<<(std::ostream& o, byte b) {
	return (o << (int)b);
}

std::ostream& operator<<(std::ostream& o, ubyte b) {
	return (o << (unsigned int)b);
}

std::ostream& operator<<(std::ostream& o, const Streamable& s) {
	s.toStream(o);
	return o;
};

inline char to_lower(char c) {
	return std::tolower(c, loc);
}

inline char to_upper(char c) {
	return std::toupper(c, loc);
}

inline pos_t operator+(size_t a, const pos_t& b) {
	return b + a;
}

inline pos_t operator-(size_t a, const pos_t& b) {
	if (b && a > (size_t)b)
		return pos_t(a - (size_t)b);
	return pos_t();
}

inline pos_t operator*(size_t a, const pos_t& b) {
	return b * a;
}

inline pos_t operator/(size_t a, const pos_t& b) {
	if (b)
		return pos_t(a / (size_t)b);
	return pos_t();
}

inline pos_t operator%(size_t a, const pos_t& b) {
	if (b)
		return pos_t(a % (size_t)b);
	return pos_t();
}
