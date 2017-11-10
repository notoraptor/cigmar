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
	}
}
