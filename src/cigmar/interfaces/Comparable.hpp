#ifndef CIGMAR_COMPARABLE
#define CIGMAR_COMPARABLE

namespace cigmar {

template<typename T>
struct Comparable {
	virtual ~Comparable() {}
	virtual int compare(const T& other) const = 0;
	bool operator==(const T& other) const {return compare(other) == 0;};
	bool operator!=(const T& other) const {return compare(other) != 0;};
	bool operator<(const T& other) const {return compare(other) < 0;};
	bool operator>(const T& other) const {return compare(other) > 0;};
	bool operator<=(const T& other) const {return compare(other) <= 0;};
	bool operator>=(const T& other) const {return compare(other) >= 0;};
};

}

#endif // CIGMAR_INTERFACES
