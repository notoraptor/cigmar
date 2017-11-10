/* Work In Progress. */

#ifndef CIGMAR_CSTRING_HPP
#define CIGMAR_CSTRING_HPP

#include <cstring>
#include <algorithm>

namespace cigmar {
	class StringView {
	public:
		static const char *empty;
		static const char* empty_characters;
	private:
		const char *member;
		size_t len;
	public:
		StringView() : member(empty), len(0) {}
		StringView(const char *str) : member(str ? str : empty) {len = strlen(member);}
		const char& operator[](size_t pos) const {return member[pos];}
		operator const char*() const {return member;}
		explicit operator bool() const {return member[0] != '\0';}
		size_t length() const {return len;}
		const char* begin() const {return member;}
		const char* end() const {return member + len;}
		size_t ltrimmable(const StringView& characters = empty_characters) const {
			size_t count;
			for (count = 0; member[count] != '\0' && characters.contains(member[count]); ++count);
			return count;
		}
		size_t rtrimmable(const StringView& characters = empty_characters) const {
			size_t count;
			for (count = 0; count < len && characters.contains(member[len - 1 - count]); ++count);
			return count;
		}
		StringView& swap(StringView& other) {
			std::swap(member, other.member);
			std::swap(len, other.len);
			return *this;
		}
		bool contains(char c, size_t start = 0) const {
			while (member[start] != '\0') {
				if (member[start] == c)
					return true;
				++start;
			}
			return false;
		}
	};
}

#endif //CIGMAR_CSTRING_HPP
