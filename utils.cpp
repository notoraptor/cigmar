#include "utils.hpp"

/** Initialization of global symbols and variables. **/

_LAST LAST;

CigmarInit cigmar_init;

namespace numbers {
    RNG rng;
}

const size_t String::UNKNOWN = std::string::npos;

/** Others définitions. **/

static std::locale loc;

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

char to_lower(char c) {
	return std::tolower(c, loc);
}

char to_upper(char c) {
	return std::toupper(c, loc);
}
