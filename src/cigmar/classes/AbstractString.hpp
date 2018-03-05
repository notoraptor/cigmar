//
// Created by notoraptor on 17-11-30.
//

#ifndef SRC_CIGMAR_ABSTRACTSTRING_HPP
#define SRC_CIGMAR_ABSTRACTSTRING_HPP

#include <cstring>
#include <sstream>
#include <algorithm>
#include <cigmar/classes/ArrayList.hpp>
#include <cigmar/unicode.hpp>
#include <cigmar/symbols.hpp>

namespace cigmar {
	template <typename Character>
	struct AbstractString {
		ASSERT_CHARTYPE(Character);
		const Character empty_characters[];
		const Character endl[];
		typedef Character char_t;
		typedef std::basic_string<Character> string_t;
		typedef typename string_t::iterator iterator_t;
		typedef typename string_t::const_iterator const_iterator_t;
	private:
		string_t member;
		template <typename OtherCharacter> friend class AbstractString;
		template<typename... Args>
		void concatenate(const Character* str, Args&&... args) {
			member += str;
			concatenate(std::forward<Args>(args)...);
		}
		template<typename OtherCharacter, typename... Args>
		void concatenate(const OtherCharacter* s, Args&&... args) {
			ASSERT_CHARTYPE(OtherCharacter);
			unicode::convert(s, member);
			concatenate(std::forward<Args>(args)...);
		}
		template<typename T, typename... Args>
		void concatenate(T variable, Args&&... args) {
			std::ostringstream o;
			o << std::boolalpha << variable;
			unicode::convert(o.str(), member);
			concatenate(std::forward<Args>(args)...);
		}
		static void concatenate() {}
		template<typename... Args>
		void spaceFirst(const Character* s, Args&&... args) {
			member += s;
			spaceNext(std::forward<Args>(args)...);
		}
		template<typename OtherCharacter, typename... Args>
		void spaceFirst(const OtherCharacter* s, Args&&... args) {
			ASSERT_CHARTYPE(OtherCharacter);
			unicode::convert(s, member);
			spaceNext(args...);
		}
		template<typename T, typename... Args>
		void spaceFirst(T variable, Args&&... args) {
			std::ostringstream o;
			o << std::boolalpha << variable;
			unicode::convert(o.str(), member);
			spaceNext(std::forward<Args>(args)...);
		}
		static void spaceFirst() {}
		template<typename... Args>
		void spaceNext(const Character* s, Args&&... args) {
			member += ' ';
			member += s;
			spaceNext(std::forward<Args>(args)...);
		}
		template<typename OtherCharacter, typename... Args>
		void spaceNext(const OtherCharacter* s, Args&&... args) {
			ASSERT_CHARTYPE(OtherCharacter);
			member += ' ';
			unicode::convert(s, member);
			spaceNext(std::forward<Args>(args)...);
		}
		template<typename T, typename... Args>
		void spaceNext(T variable, Args&&... args) {
			std::ostringstream o;
			o << std::boolalpha << variable;
			member += ' ';
			unicode::convert(o.str(), member);
			spaceNext(std::forward<Args>(args)...);
		}
		static void spaceNext() {}
		static pos_t strpos(size_t pos) {
			return pos_t(pos != string_t::npos, pos);
		}
	public:
		AbstractString(): member() {}
		template <typename OtherCharacter>
		AbstractString(const OtherCharacter* s): member() {
			ASSERT_CHARTYPE(OtherCharacter);
			unicode::convert(s, member);
		}
		template <typename OtherCharacter>
		AbstractString(const OtherCharacter* s, size_t pos, size_t len = string_t::npos): member() {
			ASSERT_CHARTYPE(OtherCharacter);
			unicode::convert(s + pos, member, len);
		}
		AbstractString(const Character* s) noexcept : member(s) {}
		AbstractString(const Character* s, size_t pos, size_t len = string_t::npos): member() {
			len = std::min(len, characters::stringLength(s + pos));
			member.assign(s + pos, len);
		}
		AbstractString(size_t length, Character c): member(length, c) {}
		template <typename OtherCharacter>
		AbstractString(const AbstractString<OtherCharacter>& other): member() {
			unicode::convert(other.member, member);
		}
		AbstractString(const AbstractString& s, size_t pos, size_t len = string_t::npos): member() {
			len = std::min(len, s.length() - pos);
			member.assign(s.member, pos, len);
		}
		AbstractString(const AbstractString& s): member(s.member) {}
		AbstractString(AbstractString&& s) noexcept: member(std::move(s.member)) {}
		AbstractString(const string_t& s): member(s) {}
		AbstractString(string_t&& s) noexcept : member(std::move(s)) {}

		template<typename... Args> static AbstractString println(Args&&... args) {
			AbstractString s;
			s.spaceFirst(std::forward<Args>(args)...);
			s << ENDL;
			return s;
		};
		template<typename... Args> static AbstractString print(Args&&... args) {
			AbstractString s;
			s.spaceFirst(std::forward<Args>(args)...);
			return s;
		};
		template<typename... Args> static AbstractString writeln(Args&&... args) {
			AbstractString s;
			s.concatenate(std::forward<Args>(args)...);
			s << ENDL;
			return s;
		};
		template<typename... Args> static AbstractString write(Args&&... args) {
			AbstractString s;
			s.concatenate(std::forward<Args>(args)...);
			return s;
		}
		template <typename C> static AbstractString join(const C& collection, const AbstractString& delimiter) {
			AbstractString out;
			auto it = collection.begin(), end = collection.end();
			if (it != end) {
				out << *it;
				while ((++it) != end) out << delimiter << *it;
			}
			return out;
		};
		template <typename C> static AbstractString join(const C& collection, char delimiter) {
			AbstractString out;
			auto it = collection.begin(), end = collection.end();
			if (it != end) {
				out << *it;
				while ((++it) != end) out << delimiter << *it;
			}
			return out;
		};
		template <typename C> static AbstractString join(const C& collection) {
			AbstractString out;
			for (auto& x: collection) out << x;
			return out;
		};

		AbstractString& operator=(const Character* s) {
			member = s;
			return *this;
		}
		template <typename OtherCharacter>
		AbstractString& operator=(const OtherCharacter* s) {
			ASSERT_CHARTYPE(OtherCharacter);
			member.clear();
			unicode::convert(s, member);
			return *this;
		}
		AbstractString& operator=(const AbstractString& other) {
			member = other.member;
			return *this;
		}
		AbstractString& operator=(AbstractString&& s) noexcept {
			member = std::move(s.member);
			return *this;
		};
		AbstractString& operator=(const string_t& s) {
			member = s;
			return *this;
		}
		AbstractString& operator=(string_t&& s) noexcept {
			member = std::move(s);
			return *this;
		}

		template<typename T>
		AbstractString& operator<<(T variable) {
			std::ostringstream o;
			o << variable;
			unicode::convert(o.str(), member);
			return *this;
		}
		AbstractString& operator<<(const Character* s) {
			member += s;
			return *this;
		}
		template <typename OtherCharacter>
		AbstractString& operator<<(const OtherCharacter* s) {
			ASSERT_CHARTYPE(OtherCharacter);
			unicode::convert(s, member);
			return *this;
		}
		AbstractString& operator<<(const AbstractString& s) {
			member += s.member;
			return *this;
		}
		AbstractString& operator<<(bool val) {
			Character trueString[] = {'t', 'r', 'u', 'e', '\0'};
			Character falseString[] = {'f', 'a', 'l', 's', 'e', '\0'};
			member += (val ? trueString : falseString);
			return *this;
		}

		Character& operator[](size_t pos) {return member[pos];}
		Character& operator[](last_t) {return member.back();}
		const Character& operator[](size_t pos) const {return member[pos];}
		const Character& operator[](last_t) const {return member.back();}

		explicit operator const Character*() const {return member.c_str();}
		explicit operator bool() const {return (bool)member.length();}

		AbstractString operator()() const {return AbstractString(*this);}
		AbstractString operator()(size_t pos, size_t len) const {return AbstractString(*this, pos, len);}
		AbstractString operator()(size_t pos, last_t last) const {return AbstractString(*this, pos);}

		string_t& cppstring() {return member;}
		const string_t& cppstring() const {return member;}
		const Character* cstring() const {return member.c_str();}
		size_t length() const {return member.length();}
		iterator_t begin() {return member.begin();}
		iterator_t end() {return member.end();}
		const_iterator_t begin() const {return member.begin();}
		const_iterator_t end() const {return member.end();}

		AbstractString& assign(const AbstractString& other, size_t pos, size_t len = string_t::npos) {
			len = std::min(len, other.length() - pos);
			member.assign(other.member, pos, len);
			return *this;
		}
		AbstractString& clear() {
			member.clear();
			return *this;
		}
		AbstractString& swap(AbstractString& other) {
			member.swap(other.member);
			return *this;
		}
		AbstractString& swap(string_t& other) {
			member.swap(other);
			return *this;
		}
		AbstractString& insert(size_t pos, const AbstractString& s) {
			member.insert(pos, s.member);
			return *this;
		}
		AbstractString& insert(size_t pos, const Character* s) {
			member.insert(pos, s);
			return *this;
		}
		AbstractString& replace(size_t pos, size_t len, const AbstractString& s) {
			member.replace(pos, len, s.member);
			return *this;
		}
		AbstractString& replace(size_t pos, size_t len, const Character* s) {
			member.replace(pos, len, s);
			return *this;
		}
		AbstractString& replace(const AbstractString& from, const AbstractString& to) {
			size_t from_len = from.member.length();
			size_t to_len = to.member.length();
			size_t pos = 0;
			do {
				pos = member.find(from.member, pos);
				if (pos != string_t::npos) {
					member.replace(pos, from_len, to.member);
					pos += to_len;
				}
			} while(pos != string_t::npos);
			return *this;
		}
		AbstractString& replace(const AbstractString& from, const Character* to) {
			size_t from_len = from.member.length();
			size_t to_len = characters::stringLength(to);
			size_t pos = 0;
			do {
				pos = member.find(from.member, pos);
				if (pos != string_t::npos) {
					member.replace(pos, from_len, to);
					pos += to_len;
				}
			} while(pos != string_t::npos);
			return *this;
		}
		AbstractString& replace(const Character* from, const AbstractString& to) {
			size_t from_len = characters::stringLength(from);
			size_t to_len = to.member.length();
			size_t pos = 0;
			do {
				pos = member.find(from, pos);
				if (pos != string_t::npos) {
					member.replace(pos, from_len, to.member);
					pos += to_len;
				}
			} while(pos != string_t::npos);
			return *this;
		}
		AbstractString& replace(const Character* from, const Character* to) {
			size_t from_len = characters::stringLength(from);
			size_t to_len = characters::stringLength(to);
			size_t pos = 0;
			do {
				pos = member.find(from, pos);
				if (pos != string_t::npos) {
					member.replace(pos, from_len, to);
					pos += to_len;
				}
			} while(pos != string_t::npos);
			return *this;
		}
		AbstractString& replace(Character from, Character to) {
			for (Character& character: member) if (character == from) character = to;
			return *this;
		}
		AbstractString& remove(size_t pos, size_t len) {
			member.erase(pos, len);
			return *this;
		}
		size_t leftTrimmable(const AbstractString &characters = empty_characters) const {
			size_t pos = member.find_first_not_of(characters.member);
			return pos == string_t::npos ? length() : pos;
		}
		size_t rightTrimmable(const AbstractString &characters = empty_characters) const {
			size_t pos = member.find_last_not_of(characters.member);
			return pos == string_t::npos ? length() : (member.length() - pos - 1);
		}
		AbstractString& trimLeft(const AbstractString &characters = empty_characters) {
			size_t pos = member.find_first_not_of(characters.member);
			if (pos == string_t::npos)
				member.clear();
			else
				member.erase(0, pos);
			return *this;
		}
		AbstractString& trimRight(const AbstractString &characters = empty_characters) {
			size_t pos = member.find_last_not_of(characters.member);
			if (pos == string_t::npos)
				member.clear();
			else
				member.erase(pos + 1);
			return *this;
		}
		AbstractString& trim(const AbstractString& characters = empty_characters) {
			trimLeft(characters);
			trimRight(characters);
			return *this;
		}
		AbstractString& lower() {
			if (member.length())
				characters::lower(member.data(), member.length());
			return *this;
		}
		AbstractString& upper() {
			if (member.length())
				characters::upper(member.data(), member.length());
			return *this;
		}
		size_t substringInto(Character* out, size_t pos, size_t len = string_t::npos) const {
			size_t copied = member.copy(out, len, pos);
			out[copied] = '\0';
			return copied;
		}
		/**< out must be large enough to contains len characters + 1 null-terminal character. **/
		size_t charactersInto(Character* out, size_t pos, size_t len = string_t::npos) const {
			return member.copy(out, len, pos);
		}
		/**< out must be large enough to contains just len characters. **/
		ArrayList<AbstractString> split(const AbstractString& delimiter) const {
			ArrayList<AbstractString> pieces;
			if (delimiter) {
				size_t piece_start = 0;
				size_t next_start = 0;
				do {
					next_start = member.find(delimiter.member, piece_start);
					size_t piece_len = next_start - piece_start;
					pieces.add(AbstractString(*this, piece_start, piece_len));
					piece_start = next_start + delimiter.member.length();
				} while (next_start != string_t::npos);
			}
			return pieces;
		}
		ArrayList<AbstractString> split(const Character* delimiter) const {
			ArrayList<AbstractString> pieces;
			size_t sep_len = characters::stringLength(delimiter);
			if (sep_len) {
				size_t piece_start = 0;
				size_t next_start = 0;
				do {
					next_start = member.find(delimiter, piece_start);
					pieces.add(AbstractString(*this, piece_start, next_start - piece_start));
					piece_start = next_start + sep_len;
				} while (next_start != string_t::npos);
			}
			return pieces;
		}
		ArrayList<AbstractString> split(char delimiter) const {
			ArrayList<AbstractString> pieces;
			if (delimiter) {
				size_t piece_start = 0;
				size_t next_start = 0;
				do {
					next_start = member.find(delimiter, piece_start);
					pieces.add(AbstractString(*this, piece_start, next_start - piece_start));
					piece_start = next_start + 1;
				} while (next_start != string_t::npos);
			}
			return pieces;
		}
		bool isEmpty() const {return member.empty();}
		bool contains(const AbstractString& s, size_t start = 0) const {
			return member.find(s.member, start) != string_t::npos;
		}
		bool contains(const Character* s, size_t start = 0) const {
			return member.find(s, start) != string_t::npos;
		}
		bool contains(Character c, size_t start = 0) const {
			return member.find(c, start) != string_t::npos;
		}
		bool startsWith(const AbstractString& s) const {
			return member.compare(0, s.member.length(), s.member) == 0;
		}
		bool startsWith(const Character* s) const {
			return member.compare(0, characters::stringLength(s), s) == 0;
		}
		bool endsWith(const AbstractString& s) const {
			size_t len_m = member.length();
			size_t len_s = s.member.length();
			return len_m >= len_s && member.compare(len_m - len_s, len_s, s.member) == 0;
		}
		bool endsWith(const Character* s) const {
			size_t len_m = member.length();
			size_t len_s = characters::stringLength(s);
			return len_m >= len_s && member.compare(len_m - len_s, len_s, s) == 0;
		}
		pos_t indexOf(const AbstractString& s, size_t start = 0) const {
			return strpos(member.find(s.member, start));
		}
		pos_t indexOf(const Character* s, size_t start = 0) const {
			return strpos(member.find(s, start));
		}
		pos_t indexOf(Character c, size_t start = 0) const {
			return strpos(member.find(c, start));
		}
		pos_t lastIndexOf(const AbstractString& s, size_t start = string_t::npos) const {
			return strpos(member.rfind(s.member, start));
		}
		pos_t lastIndexOf(const Character* s, size_t start = string_t::npos) const {
			return strpos(member.rfind(s, start));
		}
		pos_t lastIndexOf(Character c, size_t start = string_t::npos) const {
			return strpos(member.rfind(c, start));
		}
		size_t hash() const {
			return std::hash<string_t>()(member);
		}
		int compare(const AbstractString& other) const {
			return member.compare(other.member);
		}
		int compare(const Character* other) const {
			return member.compare(other);
		}
		template <typename OtherCharacter>
		int compare(const OtherCharacter* other) const {
			ASSERT_CHARTYPE(OtherCharacter);
			// TODO: Improve unicode handling by using iterations.
			return compare(other);
		}
	};
	template <typename Character> const Character AbstractString<Character>::empty_characters[] = EMPTY_CHARACTERS;
	template <typename Character> const Character AbstractString<Character>::endl[] = ENDL;
	MAKE_TEMPLATE_AUTO_COMPARABLE((template<typename A>), AbstractString<A>);
	MAKE_TEMPLATE_COMPARABLE((template<typename A, typename B>), AbstractString<A>, const B*);
	template<typename C1, typename C2>
	std::basic_ostream<C1>& operator<<(std::basic_ostream<C1>& o, const AbstractString<C2>& s) {
		unicode::convert(s.cppstring(), o);
		return o;
	};

	template <typename C>
	class AbstractStringLinesIterator {
		typedef AbstractString<C> abstract_striing_t;
		typedef typename abstract_striing_t::string_t string_t;
		const abstract_striing_t* ptr;
		abstract_striing_t line;
		C newline[3];
		size_t pos;
		void defineNewline(size_t position) {
			switch((*ptr)[position]) {
				case '\r':
					if (position == ptr->length() - 1 || (*ptr)[position + 1] != '\n')
						newline[1] = '\0';
					break;
				case '\n':
					newline[0] = '\n';
					newline[1] = '\0';
					break;
				default: break;
			}
		}
		void update(size_t next_position) {
			if (next_position == string_t::npos) {
				line.assign(*ptr, pos);
				pos = next_position;
			} else {
				line.assign(*ptr, pos, next_position - pos);
				pos = next_position + characters::stringLength(newline);
			}
		}
	public:
		AbstractStringLinesIterator(): ptr(nullptr), line(), newline(), pos(0) {}
		explicit AbstractStringLinesIterator(const AbstractString& s): ptr(&s), line(), newline(), pos(0) {
			newline[0] = '\r';
			newline[1] = '\n';
			newline[2] = '\0';
			size_t next_position = ptr->cppstring().find_first_of(newline, pos);
			if (next_position != string_t::npos)
				defineNewline(next_position);
			update(next_position);
		}
		bool operator==(const AbstractStringLinesIterator& o) const {return ptr == o.ptr && pos == o.pos;}
		bool operator!=(const AbstractStringLinesIterator& o) const {return ptr != o.ptr || pos != o.pos;}
		AbstractString& operator*() {return line;}
		AbstractStringLinesIterator& operator++() {
			if (ptr) {
				if (pos == string_t::npos)
					ptr = nullptr;
				else
					update(ptr->cppstring().find(newline, pos));
			}
			return *this;
		}
	};
	template <typename C>
	class AbstractStringLines {
		const AbstractString<C>& s;
	public:
		explicit AbstractStringLines(const AbstractString<C>& str): s(str) {}
		AbstractStringLinesIterator<C> begin() const {return AbstractStringLinesIterator<C>(s);}
		AbstractStringLinesIterator<C> end() const {return AbstractStringLinesIterator<C>();}
	};
	template <typename C>
	class AbstractStringSplitIterator {
		typedef AbstractString<C> abstract_string_t;
		typedef typename abstract_string_t::string_t string_t;
		const abstract_string_t* ptr;
		abstract_string_t sep;
		abstract_string_t piece;
		bool skip_escaped;
		size_t pos;
	public:
		AbstractStringSplitIterator(): ptr(nullptr), sep(), piece(), skip_escaped(false), pos(0) {}
		explicit AbstractStringSplitIterator(const abstract_string_t& str, const abstract_string_t& delimiter, bool skipEscaped):
				ptr(nullptr), sep(), piece(), skip_escaped(skipEscaped), pos(0) {
			if (delimiter) {
				ptr = &str;
				sep = delimiter;
				this->operator++();
			}
		}
		bool operator==(const AbstractStringSplitIterator& o) const {return ptr == o.ptr;}
		bool operator!=(const AbstractStringSplitIterator& o) const {return ptr != o.ptr;}
		abstract_string_t& operator*() {return piece;}
		abstract_string_t* operator->() {return &piece;}
		AbstractStringSplitIterator& operator++() {
			if (ptr) {
				if (pos == string_t::npos)
					ptr = nullptr;
				else {
					size_t next_position = ptr->cppstring().find(sep.cppstring(), pos);
					if (next_position && next_position != string_t::npos && skip_escaped) {
						bool found = false;
						do {
							if ((*ptr)[next_position - 1] == '\\')
								next_position = ptr->cppstring().find(sep.cppstring(), next_position + 1);
							else
								found = true;
						} while (!found && next_position != string_t::npos);
					}
					if (next_position == string_t::npos) {
						piece.assign(*ptr, pos);
						pos = next_position;
					} else {
						piece.assign(*ptr, pos, next_position - pos);
						pos = next_position + sep.length();
					}
				}
			}
			return *this;
		}
	};
	template <typename C>
	class AbstractStringSplit {
		const AbstractString<C>& s;
		AbstractString<C> sep;
		bool skip_escape;
	public:
		explicit AbstractStringSplit(const AbstractString<C>& str, const AbstractString<C>& delimiter, bool skipEscape):
				s(str), sep(delimiter), skip_escape(skipEscape) {}
		AbstractStringSplitIterator begin() const {return AbstractStringSplitIterator<C>(s, sep, skip_escape);}
		AbstractStringSplitIterator end() const {return AbstractStringSplitIterator<C>();}
	};
	template <typename C>
	AbstractStringLines<C> lines(const AbstractString<C>& s) {
		return AbstractStringLines<C>(&s);
	}
	template <typename C>
	AbstractStringSplit<C> split(const AbstractString<C>& s, const AbstractString<C>& delimiter, bool skipEscaped = false) {
		return AbstractStringSplit(&s, delimiter, skipEscaped);
	}
}

#endif //SRC_CIGMAR_ABSTRACTSTRING_HPP
