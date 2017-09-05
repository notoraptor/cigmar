#include <locale>
#include "utils.hpp"

/** Initialization of global symbols and variables. **/

_LAST LAST;

CigmarInit cigmar_init;

namespace numbers {
    RNG rng;
}

/** Others définitions. **/

const char String::endl = '\n';

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

char String::to_lower(char c) {
	return std::tolower(c, loc);
}

char String::to_upper(char c) {
	return std::toupper(c, loc);
}

