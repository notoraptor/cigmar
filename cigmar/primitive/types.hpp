#ifndef CIGMAR_PRIMITIVE_TYPES
#define CIGMAR_PRIMITIVE_TYPES
#include <cstdint>
#include <ostream>
#include <cigmar/primitive/types/pos_t.hpp>
#include <cigmar/primitive/types/return_t.hpp>

namespace cigmar {

/// bool
/// char
using byte_t = int8_t;
using short_t = int16_t;
using int_t = int32_t;
using long_t = int64_t;
using ubyte_t = uint8_t;
using ushort_t = uint16_t;
using uint_t = uint32_t;
using ulong_t = uint64_t;
/// float
/// double
/// size_t


std::ostream& operator<<(std::ostream& o, byte_t b);
std::ostream& operator<<(std::ostream& o, ubyte_t b);

}

#endif // CIGMAR_PRIMITIVE_TYPES
