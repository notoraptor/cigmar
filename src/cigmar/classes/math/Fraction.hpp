//
// Created by notoraptor on 15/11/2017.
//

#ifndef SRC_CIGMAR_FRACTION_HPP
#define SRC_CIGMAR_FRACTION_HPP

#include <cassert>
#include <cstdint>
#include <cigmar/interfaces/Streamable.hpp>

namespace cigmar::math {
	// Greatest common divisor ([fr] Plus Grand Commun Diviseur)
	template<typename A, typename B>
	A gcd(A a, B b) {
		return a < b ? gcd(b, a) : (b == 0 ? a : gcd(b, a % b));
	};

	template<typename T>
	class AbstractFraction: public Streamable {
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
		explicit operator double() const {return s * (double)num/den;};
		void toStream(std::ostream& o) const override {
			if (num) {
				if (s == -1) o << '-';
				o << num;
				if (den != 1) o << '/' << den;
			} else {
				// num == 0.
				o << 0;
			}
		};
	};

	typedef AbstractFraction<uint64_t> Fraction;
}


#endif //SRC_CIGMAR_FRACTION_HPP
