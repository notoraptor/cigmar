//
// Created by notoraptor on 01/11/2017.
//

#include <limits>
#include <cigmar/classes/Exception.hpp>
#include <cigmar/whirlpool.hpp>
#include <libraries/whirlpool/nessie.h>

static const char* hexDigits = "0123456789ABCDEF";

namespace cigmar::crypto::hash {
	// TODO: Move it in cigmar hex module.
	static String toHex(const u8 array[], size_t length) {
		String hexString(2 * length, '0');
		for (size_t i = 0; i < length; ++i) {
			hexString[2 * i] = hexDigits[array[i] >> 4];
			hexString[2 * i + 1] = hexDigits[array[i] & ((1 << 4) - 1)];
		}
		return hexString;
	}

	String whirlpool(const String& in) {
		if (in.length() > std::numeric_limits<unsigned long>::max() / 8) {
			throw Exception("Current implementation of Whirlpool hashing algorithm supports maximum input size ",
							std::numeric_limits<unsigned long>::max() / 8, ", got ", in.length(), '.');
		}
		u8 digest[DIGESTBYTES];
		struct NESSIEstruct w;
		NESSIEinit(&w);
		NESSIEadd((const unsigned char*)in.cstring(), 8 * (unsigned long)in.length(), &w);
		NESSIEfinalize(&w, digest);
		return toHex(digest, DIGESTBYTES);
	};
}
