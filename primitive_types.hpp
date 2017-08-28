#ifndef CIGMAR_PRIMITIVE_TYPES
#define CIGMAR_PRIMITIVE_TYPES

#include <cstdint>
#include <ostream>
#include <string>
#include "primitive_symbols.hpp"
#include "interfaces.hpp"

/// bool
/// char
using byte = int8_t;
using short_t = int16_t;
using int_t = int32_t;
using long_t = int64_t;
using ubyte = uint8_t;
using ushort = uint16_t;
using uint = uint32_t;
using ulong = uint64_t;
/// float
/// double
/// size_t

class pos_t: public Comparable<pos_t>, public Streamable {
private:
	bool valid;
	size_t value;
public:
	pos_t(): valid(false), value(-1) {}
	pos_t(size_t v): valid(true), value(v) {}
	pos_t(bool ok, size_t v): valid(ok), value(v) {}
	explicit operator bool() const {return valid;}
	int compare(const pos_t& other) const override {
		return (!valid && !other.valid) ? 0 : ( value < other.value ? -1 : value - other.value );
	}
	void toStream(std::ostream& o) const override {
		if (valid)
			o << value;
		else
			o << -1;
	}
	static pos_t _stringpos(size_t pos) {
		return pos_t(pos != std::string::npos, pos);
	}
};

std::ostream& operator<<(std::ostream& o, byte b);
std::ostream& operator<<(std::ostream& o, ubyte b);

#endif // CIGMAR_PRIMITIVE_TYPES
