#ifndef CIGMAR_INTERFACES
#define CIGMAR_INTERFACES

#include <iostream>
#include <functional>
#include <cigmar/primitive/interfaces/Streamable.hpp>

namespace cigmar {

template<typename T>
class Comparable {
public:
	virtual int compare(const T& other) const = 0;
	bool operator==(const T& other) const {return compare(other) == 0;};
	bool operator!=(const T& other) const {return compare(other) != 0;};
	bool operator<(const T& other) const {return compare(other) < 0;};
	bool operator>(const T& other) const {return compare(other) > 0;};
	bool operator<=(const T& other) const {return compare(other) <= 0;};
	bool operator>=(const T& other) const {return compare(other) >= 0;};
};

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

#define COMPARABLE(compClass, compType) \
inline bool operator==(const compClass& a, compType b) {return a.compare(b) == 0;} \
inline bool operator!=(const compClass& a, compType b) {return a.compare(b) != 0;} \
inline bool operator<=(const compClass& a, compType b) {return a.compare(b) <= 0;} \
inline bool operator>=(const compClass& a, compType b) {return a.compare(b) >= 0;} \
inline bool operator<(const compClass& a, compType b) {return a.compare(b) < 0;} \
inline bool operator>(const compClass& a, compType b) {return a.compare(b) > 0;} \
inline bool operator==(compType b, const compClass& a) {return a.compare(b) == 0;} \
inline bool operator!=(compType b, const compClass& a) {return a.compare(b) != 0;} \
inline bool operator<=(compType b, const compClass& a) {return a.compare(b) >= 0;} \
inline bool operator>=(compType b, const compClass& a) {return a.compare(b) <= 0;} \
inline bool operator<(compType b, const compClass& a) {return a.compare(b) > 0;} \
inline bool operator>(compType b, const compClass& a) {return a.compare(b) < 0;}

}

#endif // CIGMAR_INTERFACES
