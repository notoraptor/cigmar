//
// Created by HPPC on 07/11/2017.
//

#ifndef CIGMAR_CROSSCOMPARABLE_HPP
#define CIGMAR_CROSSCOMPARABLE_HPP

namespace cigmar {
	template<typename T>
	struct CrossComparable {
		virtual int crossCompare(T other) const = 0;
	};

	template<typename T> inline bool operator==(const CrossComparable<T>& a, T b) {return a.crossCompare(b) == 0;}
	template<typename T> inline bool operator!=(const CrossComparable<T>& a, T b) {return a.crossCompare(b) != 0;}
	template<typename T> inline bool operator<=(const CrossComparable<T>& a, T b) {return a.crossCompare(b) <= 0;}
	template<typename T> inline bool operator>=(const CrossComparable<T>& a, T b) {return a.crossCompare(b) >= 0;}
	template<typename T> inline bool operator<(const CrossComparable<T>& a, T b) {return a.crossCompare(b) < 0;}
	template<typename T> inline bool operator>(const CrossComparable<T>& a, T b) {return a.crossCompare(b) > 0;}
	template<typename T> inline bool operator==(T b, const CrossComparable<T>& a) {return a.crossCompare(b) == 0;}
	template<typename T> inline bool operator!=(T b, const CrossComparable<T>& a) {return a.crossCompare(b) != 0;}
	template<typename T> inline bool operator<=(T b, const CrossComparable<T>& a) {return a.crossCompare(b) >= 0;}
	template<typename T> inline bool operator>=(T b, const CrossComparable<T>& a) {return a.crossCompare(b) <= 0;}
	template<typename T> inline bool operator<(T b, const CrossComparable<T>& a) {return a.crossCompare(b) > 0;}
	template<typename T> inline bool operator>(T b, const CrossComparable<T>& a) {return a.crossCompare(b) < 0;}
}

#endif //CIGMAR_CROSSCOMPARABLE_HPP
