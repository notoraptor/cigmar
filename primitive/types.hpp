#ifndef CIGMAR_PRIMITIVE_TYPES
#define CIGMAR_PRIMITIVE_TYPES
#include <cstdint>
#include <ostream>
#include "types/pos_t.hpp"
#include "types/return_t.hpp"

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


std::ostream& operator<<(std::ostream& o, byte b);
std::ostream& operator<<(std::ostream& o, ubyte b);

#endif // CIGMAR_PRIMITIVE_TYPES
