#ifndef CIGMAR_COMPARABLE
#define CIGMAR_COMPARABLE

namespace cigmar {

template<typename T>
class Comparable {
public:
	virtual ~Comparable() {}
	virtual int compare(const T& other) const = 0;
	bool operator==(const T& other) const {return compare(other) == 0;};
	bool operator!=(const T& other) const {return compare(other) != 0;};
	bool operator<(const T& other) const {return compare(other) < 0;};
	bool operator>(const T& other) const {return compare(other) > 0;};
	bool operator<=(const T& other) const {return compare(other) <= 0;};
	bool operator>=(const T& other) const {return compare(other) >= 0;};
};

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
