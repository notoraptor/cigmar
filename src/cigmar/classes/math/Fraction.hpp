//
// Created by notoraptor on 15/11/2017.
//

#ifndef SRC_CIGMAR_FRACTION_HPP
#define SRC_CIGMAR_FRACTION_HPP

#include <cassert>
#include <cstdint>
#include <ostream>

namespace cigmar::math {
	// Greatest common divisor ([fr] Plus Grand Commun Diviseur)
	template<typename A, typename B>
	A gcd(A a, B b) {
		return a < b ? gcd(b, a) : (b == 0 ? a : gcd(b, a % b));
	};

	template<typename T>
	class AbstractFraction {
		char s;
		T num;
		T den;
		void update() {
			T greatestCommonDivisor = gcd(num, den);
			num /= greatestCommonDivisor;
			den /= greatestCommonDivisor;
		}
	public:
		AbstractFraction(char sign, T numerator, T denominator): s(sign), num(numerator), den(denominator) {
			assert(s == 1 || s == -1);
			assert(den != 0);
			update();
		};
		AbstractFraction(int64_t numerator = 0, uint64_t denominator = 1) {
			if (numerator < 0) {
				s = -1;
				num = -numerator;
			} else {
				s = 1;
				num = numerator;
			}
			den = denominator;
			assert(den != 0);
			update();
		};
		const char sign() const {return s;}
		const T& numerator() const {return num;}
		const T& denominator() const {return den;}
		explicit operator double() const {return s * (double)num/den;};
	};

	typedef AbstractFraction<uint64_t> Fraction;

	template <typename C, typename T>
	std::basic_ostream<C>& operator<<(std::basic_ostream<C>& o, const AbstractFraction<T>& f) {
		if (f.numerator()) {
			if (f.sign() == -1) o << '-';
			o << f.numerator();
			if (f.denominator() != 1) o << '/' << f.denominator();
		} else {
			// num == 0.
			o << 0;
		}
		return o;
	}
}


#endif //SRC_CIGMAR_FRACTION_HPP
