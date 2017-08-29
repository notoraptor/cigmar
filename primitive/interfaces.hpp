#ifndef CIGMAR_INTERFACES
#define CIGMAR_INTERFACES

#include <ostream>
#include <functional>

#define HASHABLE(C) namespace std { template<> struct hash<C> \
					{size_t operator()(const C& o) const {return o.hash();}}; \
					}

template<typename T>
class AutoComparable {
public:
	virtual int compare(const T& other) const = 0;
	bool operator==(const T& other) const {return compare(other) == 0;};
	bool operator!=(const T& other) const {return compare(other) != 0;};
	bool operator<(const T& other) const {return compare(other) < 0;};
	bool operator>(const T& other) const {return compare(other) > 0;};
	bool operator<=(const T& other) const {return compare(other) <= 0;};
	bool operator>=(const T& other) const {return compare(other) >= 0;};
};

template<typename T>
class Comparable {
public:
	virtual int compare(T other) const = 0;
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

template<typename T> bool operator==(T a, const Comparable<T>& b) {return b.compare(a) == 0;};
template<typename T> bool operator!=(T a, const Comparable<T>& b) {return b.compare(a) != 0;};
template<typename T> bool operator<(T a, const Comparable<T>& b) {return b.compare(a) > 0;};
template<typename T> bool operator>(T a, const Comparable<T>& b) {return b.compare(a) < 0;};
template<typename T> bool operator<=(T a, const Comparable<T>& b) {return b.compare(a) >= 0;};
template<typename T> bool operator>=(T a, const Comparable<T>& b) {return b.compare(a) <= 0;};

#endif // CIGMAR_INTERFACES
