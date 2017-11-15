//
// Created by notoraptor on 31/10/2017.
//

#ifndef CIGMAR_BASE64_HPP
#define CIGMAR_BASE64_HPP

#include <cigmar/classes/String.hpp>

namespace cigmar {
	namespace base64 {
		String encode(const String& in);
		String decode(const String& in);
	}
}

#endif //CIGMAR_BASE64_HPP_HPP