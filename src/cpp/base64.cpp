//
// Created by notoraptor on 31/10/2017.
//

#include <libraries/base64/base64.hpp>
#include <cigmar/base64.hpp>

namespace cigmar {
	namespace base64 {
		String encode(const String& in) {
			String out;
			Base64::Encode(in.cppstring(), out.cppstring());
			return out;
		}
		String decode(const String& in) {
			String out;
			Base64::Decode(in.cppstring(), out.cppstring());
			return out;
		}
		namespace bytes {
			String encode(const byte_t* in, size_t len) {
				ArrayList<char> out(Base64::EncodedLength(len) + 1, '\0');
				Base64::Encode((const char*)in, len, (char*)out, out.size());
				return String((const char*)out);
			};
			ArrayList<byte_t> decode(const String& in) {
				ArrayList<byte_t> out((size_t)Base64::DecodedLength((const char*)in, in.length()));
				Base64::Decode((const char*)in, in.length(), (char*)(byte_t*)out, out.size());
				return out;
			};
		}
	}
}
