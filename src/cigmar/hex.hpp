//
// Created by notoraptor on 09/11/2017.
//

#ifndef CIGMAR_HEX_HPP
#define CIGMAR_HEX_HPP

#include <cigmar/classes/String.hpp>

namespace cigmar::hex {
	namespace {
		const char* hexDigits = "0123456789ABCDEF";

		inline unsigned int hexup(unsigned char c) {
			return c >> 4;
		}

		inline unsigned int hexdown(unsigned char c) {
			return c & (((unsigned int) 1 << 4) - 1);
		}
	}

	template<typename T>
	String encode(T value, bool trimFirstZeroPairs = true) {
		static_assert(std::is_integral<T>{}, "cigmar::hex::encode() expects an integral type.");
		size_t n_bytes = sizeof(value);
		String out(2 * n_bytes, '0');
		for (size_t i = 0; i < n_bytes; ++i) {
			unsigned char c = value & ((1 << 8) - 1);
			out[2 * n_bytes - 2 - 2 * i] = hexDigits[hexup(c)];
			out[2 * n_bytes - 1 - 2 * i] = hexDigits[hexdown(c)];
			value = (value >> 8);
		}
		if (trimFirstZeroPairs) {
			size_t first_zeros = out.leftTrimmable("0");
			if (first_zeros) {
				if (first_zeros == out.length())
					out = "00";
				else if (first_zeros % 2 == 0)
					out.trimLeft("0");
				else
					out = String(out, first_zeros - 1);
			}
		}
		return out;
	}
}

#endif //CIGMAR_HEX_HPP
