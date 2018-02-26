#ifndef CIGMAR_CHAR
#define CIGMAR_CHAR

namespace cigmar {
	struct Char {
		static char lower(char c);
		static char upper(char c);
		template <typename Character>
		static size_t stringlength(const Character* s) {
			size_t len = 0;
			while (*s) {
				++len;
				++s;
			}
			return len;
		}
	};
}

#endif
