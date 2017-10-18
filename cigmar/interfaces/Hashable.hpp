#ifndef CIGMAR_HASHABLE
#define CIGMAR_HASHABLE

#include <cstddef>

namespace cigmar {
class Hashable {
public:
	virtual size_t hash() const = 0;
};

#define HASHABLE(C) namespace std { template<> struct hash<C> \
					{size_t operator()(const C& o) const {return o.hash();}}; }
/**< Make class C hashable-compliant with C++ standard library.
This macro generates a class that inherits from std::hash and puts the new class
into std namespace. To make it work correctly, this macro must be called outside
of any namespace.
**/
}

#endif
