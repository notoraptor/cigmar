//
// Created by notoraptor on 04/03/2018.
//

#ifndef SRC_CIGMAR_CHARACTERS_HPP
#define SRC_CIGMAR_CHARACTERS_HPP

#include <locale>

namespace cigmar {
	namespace characters {
		extern std::locale loc;
		template <typename Character>
		Character lower(Character c) {
			return tolower(c, loc);
		};
		template <typename Character>
		Character upper(Character c) {
			return toupper(c, loc);
		};
		template <typename Character>
		void lower(Character* s, size_t len) {
			std::use_facet<std::ctype<Character>>(loc).tolower(s, s + len);
		}
		template <typename Character>
		void upper(Character* s, size_t len) {
			std::use_facet<std::ctype<Character>>(loc).toupper(s, s + len);
		}
		template <typename Character>
		size_t length(const Character *s) {
			size_t len = 0;
			while(s[len++]);
			return len - 1;
		}
	}
}
#endif //SRC_CIGMAR_CHARACTERS_HPP
