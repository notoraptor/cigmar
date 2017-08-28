#include "utils.hpp"

_LAST LAST;

namespace numbers {
RNG rng;
}

const size_t String::UNKNOWN = std::string::npos;

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
