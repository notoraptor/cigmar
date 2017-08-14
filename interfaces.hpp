#ifndef CIGMAR_INTERFACES
#define CIGMAR_INTERFACES

#include <ostream>

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

template<typename T>
class ComparableWith {
public:
	virtual int compareWith(T other) const = 0;
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

template<typename T> bool operator==(T a, const ComparableWith<T>& b) {return b.compareWith(a) == 0;};
template<typename T> bool operator!=(T a, const ComparableWith<T>& b) {return b.compareWith(a) != 0;};
template<typename T> bool operator<(T a, const ComparableWith<T>& b) {return b.compareWith(a) > 0;};
template<typename T> bool operator>(T a, const ComparableWith<T>& b) {return b.compareWith(a) < 0;};
template<typename T> bool operator<=(T a, const ComparableWith<T>& b) {return b.compareWith(a) >= 0;};
template<typename T> bool operator>=(T a, const ComparableWith<T>& b) {return b.compareWith(a) <= 0;};

#endif // CIGMAR_INTERFACES
