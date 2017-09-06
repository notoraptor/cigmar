#ifndef CIGMAR_PRIMITIVE_SYMBOLS
#define CIGMAR_PRIMITIVE_SYMBOLS

#include <iterator>

class _LAST {};

extern _LAST LAST;

template<typename C>
size_t countof(C& iterable) {
	return std::distance(iterable.begin(), iterable.end());
}

#endif // CIGMAR_PRIMITIVE_SYMBOLS
