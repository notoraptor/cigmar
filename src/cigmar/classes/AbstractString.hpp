//
// Created by notoraptor on 17-11-30.
//

#ifndef SRC_CIGMAR_ABSTRACTSTRING_HPP
#define SRC_CIGMAR_ABSTRACTSTRING_HPP

#include <cstring>
#include <sstream>
#include <algorithm>
#include <cigmar/interfaces/Streamable.hpp>
#include <cigmar/interfaces/Hashable.hpp>
#include <cigmar/interfaces/Comparable.hpp>
#include <cigmar/interfaces/CrossComparable.hpp>
#include <cigmar/classes/ArrayList.hpp>
#include <cigmar/classes/Char.hpp>
#include <cigmar/unicode.hpp>

// TODO: Unicode empty characters, unicoe class characters ...

namespace cigmar {
	template <typename Character>
	class AbstractString: public Streamable, public Hashable, public Comparable<AbstractString<Character>> {
		class LinesIterator {
			const AbstractString* ptr;
			AbstractString line;
			Character newline[3];
			size_t pos = 0;
			void defineNewline(size_t pos) {
				switch((*ptr)[pos]) {
					case '\r':
						if (pos == ptr->length() - 1 || (*ptr)[pos + 1] != '\n')
							newline[1] = '\0';
						break;
					case '\n':
						newline[0] = '\n';
						newline[1] = '\0';
						break;
					default: break;
				}
			}
		public:
			LinesIterator(): ptr(nullptr), line(), newline() {}
			explicit LinesIterator(const AbstractString& s): ptr(&s), line(), newline() {
				newline[0] = '\r';
				newline[1] = '\n';
				newline[2] = '\0';
				this->operator++();
			}
			bool operator==(const LinesIterator& o) const {return ptr == o.ptr;}
			bool operator!=(const LinesIterator& o) const {return ptr != o.ptr;}
			AbstractString& operator*() {return line;}
			LinesIterator& operator++() {
				if (ptr) {
					if (pos == string_t::npos)
						ptr = nullptr;
					else {
						size_t next_position;
						if (pos) {
							next_position = ptr->cppstring().find(newline, pos);
						} else {
							next_position = ptr->cppstring().find_first_of(newline, pos);
							if (next_position != string_t::npos)
								defineNewline(next_position);
						}
						if (next_position == string_t::npos) {
							line.assign(*ptr, pos);
							pos = next_position;
						} else {
							line.assign(*ptr, pos, next_position - pos);
							pos = next_position + Char::stringlength(newline);
						}
					}
				}
				return *this;
			}
		};
		class Lines {
			const AbstractString& s;
		public:
			explicit Lines(const AbstractString& str): s(str) {}
			LinesIterator begin() const {return LinesIterator(s);}
			LinesIterator end() const {return LinesIterator();}
		};
		class SplitIterator {
			const AbstractString* ptr;
			AbstractString sep;
			AbstractString piece;
			bool skip_escaped;
			size_t pos;
		public:
			SplitIterator(): ptr(nullptr), sep(), piece(), skip_escaped(false), pos(0) {}
			explicit SplitIterator(const AbstractString& str, const AbstractString& delimiter, bool skipEscaped):
					ptr(nullptr), sep(), piece(), skip_escaped(skipEscaped), pos(0) {
				if (delimiter) {
					ptr = &str;
					sep = delimiter;
					this->operator++();
				}
			}
			bool operator==(const SplitIterator& o) const {return ptr == o.ptr;}
			bool operator!=(const SplitIterator& o) const {return ptr != o.ptr;}
			AbstractString& operator*() {return piece;}
			AbstractString* operator->() {return &piece;}
			SplitIterator& operator++() {
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
		class Split {
			const AbstractString& s;
			AbstractString sep;
			bool skip_escape;
		public:
			explicit Split(const AbstractString& str, const AbstractString& delimiter, bool skipEscape):
					s(str), sep(delimiter), skip_escape(skipEscape) {}
			SplitIterator begin() const {return SplitIterator(s, sep, skip_escape);}
			SplitIterator end() const {return SplitIterator();}
		};
	public:
		ASSERT_CHARTYPE(Character);
		const Character empty_characters[];
		const Character* endl[];
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
			len = std::min(len, Char::stringlength(s + pos));
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
			size_t to_len = Char::stringlength(to);
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
			size_t from_len = Char::stringlength(from);
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
			size_t from_len = Char::stringlength(from);
			size_t to_len = Char::stringlength(to);
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
			std::transform(member.begin(), member.end(), member.begin(), Char::lower);
			return *this;
		}
		AbstractString& upper() {
			std::transform(member.begin(), member.end(), member.begin(), Char::upper);
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
			size_t sep_len = Char::stringlength(delimiter);
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
			return member.compare(0, Char::stringlength(s), s) == 0;
		}
		bool endsWith(const AbstractString& s) const {
			size_t len_m = member.length();
			size_t len_s = s.member.length();
			return len_m >= len_s && member.compare(len_m - len_s, len_s, s.member) == 0;
		}
		bool endsWith(const Character* s) const {
			size_t len_m = member.length();
			size_t len_s = Char::stringlength(s);
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

		Lines lines() const {return Lines(*this);}
		Split splits(const AbstractString& delimiter, bool skipEscaped = false) const {
			return Split(*this, delimiter, skipEscaped);
		}
		void toStream(std::ostream& o) const override {
			std::string out;
			unicode::convert(member, out);
			o << out;
		}
		size_t hash() const override {
			return std::hash<string_t>()(member);
		}
		int compare(const AbstractString& other) const override {
			return member.compare(other.member);
		}
		int compareString(const Character* other) const {
			return member.compare(other);
		}
		template <typename OtherCharacter> int compareString(const OtherCharacter* other) const {
			ASSERT_CHARTYPE(OtherCharacter);
			// TODO: Improve unicode handling by using iterations.
			return compare(other);
		}
	};

	template <typename Character>
	const Character AbstractString<Character>::empty_characters[] = EMPTY_CHARACTERS;
	template <typename Character>
	const Character AbstractString<Character>::endl[] = ENDL;

	template <typename A, typename B> bool operator==(const AbstractString<A>& a, const B* b) { return a.compareString(b) == 0; };
	template <typename A, typename B> bool operator!=(const AbstractString<A>& a, const B* b) { return a.compareString(b) != 0; };
	template <typename A, typename B> bool operator<=(const AbstractString<A>& a, const B* b) { return a.compareString(b) <= 0; };
	template <typename A, typename B> bool operator>=(const AbstractString<A>& a, const B* b) { return a.compareString(b) >= 0; };
	template <typename A, typename B> bool operator<(const AbstractString<A>& a, const B* b) { return a.compareString(b) < 0; };
	template <typename A, typename B> bool operator>(const AbstractString<A>& a, const B* b) { return a.compareString(b) > 0; };

	template <typename A, typename B> bool operator==(const A* a, const AbstractString<B>& b) { return b.compareString(a) == 0; };
	template <typename A, typename B> bool operator!=(const A* a, const AbstractString<B>& b) { return b.compareString(a) != 0; };
	template <typename A, typename B> bool operator<=(const A* a, const AbstractString<B>& b) { return b.compareString(a) >= 0; };
	template <typename A, typename B> bool operator>=(const A* a, const AbstractString<B>& b) { return b.compareString(a) <= 0; };
	template <typename A, typename B> bool operator<(const A* a, const AbstractString<B>& b) { return b.compareString(a) > 0; };
	template <typename A, typename B> bool operator>(const A* a, const AbstractString<B>& b) { return b.compareString(a) < 0; };
}

#endif //SRC_CIGMAR_ABSTRACTSTRING_HPP
