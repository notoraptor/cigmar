#include <locale>
#include <cigmar/utils.hpp>

/** Initialization of global symbols and variables. **/

namespace cigmar {

_LAST LAST;

CigmarInit cigmar_init;

namespace numbers {
    RNG rng;
}

/** Others definitions. **/

const char String::endl = '\n';

/// Global variables.

static std::locale loc;

/// Global functions.

std::ostream& operator<<(std::ostream& o, byte_t b) {
	return (o << (int)b);
}

std::ostream& operator<<(std::ostream& o, ubyte_t b) {
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

}
