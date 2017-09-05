#ifndef CIGMAR_INTERFACES
#define CIGMAR_INTERFACES

#include <ostream>
#include <iostream>
#include <functional>

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

class Streamable {
public:
	virtual void toStream(std::ostream& o) const = 0;
};

class Hashable {
public:
	virtual size_t hash() const = 0;
};

std::ostream& operator<<(std::ostream& o, const Streamable& s);

#define HASHABLE(C) namespace std { template<> struct hash<C> \
					{size_t operator()(const C& o) const {return o.hash();}}; }

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

#endif // CIGMAR_INTERFACES
