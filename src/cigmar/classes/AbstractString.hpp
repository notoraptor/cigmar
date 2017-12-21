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

#define EMPTY_CHARACTERS " \f\n\r\t\v"

namespace cigmar {

	class CharString {
	public:
		const char* member;
		CharString(const char* s): member(s) {}
	};

	template <typename Character>
	class AbstractString: public Streamable, public Hashable, public Comparable<AbstractString<Character>>,
						  public CrossComparable<const Character*> {
	public:
		typedef Character char_t;
		typedef std::basic_string<Character> string_t;
		typedef typename string_t::iterator iterator_t;
		typedef typename string_t::const_iterator const_iterator_t;
	private:
		string_t member;
		template<typename... Args>
		void concatenate(const Character* str, Args... args) {
			member += str;
			concatenate(args...);
		}
		template<typename... Args>
		void concatenate(const CharString& s, Args... args) {
			unicode::convert(s.member, member);
			concatenate( args...);
		}
		template<typename T, typename... Args>
		void concatenate(T variable, Args... args) {
			std::ostringstream o;
			o << std::boolalpha << variable;
			unicode::convert(o.str(), member);
			concatenate(args...);
		}
		static void concatenate() {}
		template<typename... Args>
		void spaceFirst(const Character* s, Args... args) {
			member += s;
			spaceNext(args...);
		}
		template<typename... Args>
		void spaceFirst(const CharString& s, Args... args) {
			unicode::convert(s.member, member);
			spaceNext(args...);
		}
		template<typename T, typename... Args>
		void spaceFirst(T variable, Args... args) {
			std::ostringstream o;
			o << std::boolalpha << variable;
			unicode::convert(o.str(), member);
			spaceNext(args...);
		}
		static void spaceFirst() {}
		template<typename... Args>
		void spaceNext(const Character* s, Args... args) {
			member += ' ';
			member += s;
			spaceNext(args...);
		}
		template<typename... Args>
		void spaceNext(const CharString& s, Args... args) {
			member += ' ';
			unicode::convert(s.member, member);
			spaceNext(args...);
		}
		template<typename T, typename... Args>
		void spaceNext(T variable, Args... args) {
			std::ostringstream o;
			o << std::boolalpha << variable;
			member += ' ';
			unicode::convert(o.str(), member);
			spaceNext(args...);
		}
		static void spaceNext() {}
		static pos_t strpos(size_t pos) {
			return pos_t(pos != string_t::npos, pos);
		}
	public:
		AbstractString(): member() {}
		AbstractString(const Character* s): member(s) {}
		AbstractString(const Character* s, size_t pos, size_t len = string_t::npos): member() {
			len = std::min(len, Char::stringlength(s + pos));
			member.assign(s + pos, len);
		}
		AbstractString(CharString s): member() {
			unicode::convert(s.member, member);
		}
		AbstractString(const AbstractString& s): member(s.member) {}
		AbstractString(AbstractString&& s) noexcept: member(std::move(s.member)) {}
		AbstractString(const AbstractString& s, size_t pos, size_t len = string_t::npos): member(s.member, pos, len) {}
		AbstractString(CharString& s, size_t pos, size_t len = string_t::npos): member() {
			s.member += pos;
			size_t real_len = strlen(s.member);
			if (real_len > len) real_len = len;
			auto sv = utf::make_stringview(s.member, s.member + real_len);
			sv.template to<Character>(std::back_inserter(member));
		}
		AbstractString(size_t length, Character c): member(length, c) {}
		AbstractString(const string_t& s): member(s) {}
		AbstractString(string_t&& s) noexcept : member(std::move(s)) {}

		template<typename... Args> static AbstractString println(Args... args) {
			AbstractString s;
			s.spaceFirst(args...);
			s << ENDL;
			return s;
		};
		template<typename... Args> static AbstractString print(Args... args) {
			AbstractString s;
			s.spaceFirst(args...);
			return s;
		};
		template<typename... Args> static AbstractString writeln(Args... args) {
			AbstractString s;
			s.concatenate(args...);
			s << ENDL;
			return s;
		};
		template<typename... Args> static AbstractString write(Args... args) {
			AbstractString s;
			s.concatenate(args...);
			return s;
		}
		template <typename C> static AbstractString join(const C& collection, const AbstractString& delimiter = "") {
			AbstractString out;
			for (auto& x: collection) {
				if (out) out << delimiter;
				out << x;
			}
			return out;
		};

		AbstractString& operator=(const Character* s) {
			member = s;
			return *this;
		}
		AbstractString& operator=(const CharString& s) {
			member.clear();
			unicode::convert(s.member, member);
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
		AbstractString& operator<<(const CharString& s) {
			unicode::convert(s.member, member);
			return *this;
		}
		AbstractString& operator<<(const AbstractString& s) {
			member += s.member;
			return *this;
		}
		AbstractString& operator<<(bool val) {
			member += (val ? "true" : "false");
			return *this;
		}

		Character& operator[](size_t pos) {return member[pos];}
		Character& operator[](last_t) {return member.back();}
		const Character& operator[](size_t pos) const {return member[pos];}
		const Character& operator[](last_t) const {return member.back();}

		explicit operator const Character*() const {return member.c_str();}
		explicit operator bool() const {return !member.empty();}

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
		AbstractString& insert(size_t pos, const CharString& s) {
			return insert(pos, AbstractString(s));
		}
		AbstractString& replace(size_t pos, size_t len, const AbstractString& s) {
			member.replace(pos, len, s.member);
			return *this;
		}
		AbstractString& replace(size_t pos, size_t len, const Character* s) {
			member.replace(pos, len, s);
			return *this;
		}
		AbstractString& replace(size_t pos, size_t len, const CharString& s) {
			return member.replace(pos, len, AbstractString(s));
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
		AbstractString& replace(const AbstractString& from, const CharString& to) {
			return replace(from, AbstractString(to));
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
		AbstractString& replace(const CharString& from, const AbstractString& to) {
			return replace(AbstractString(from), to);
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
		AbstractString& replace(const CharString& from, const Character* to) {
			return replace(AbstractString(from), to);
		}
		AbstractString& replace(const Character* from, const CharString& to) {
			return replace(from, AbstractString(to));
		}
		AbstractString& replace(const CharString& from, const CharString& to) {
			return replace(AbstractString(from), AbstractString(to));
		}
		AbstractString& remove(size_t pos, size_t len) {
			member.erase(pos, len);
			return *this;
		}
		size_t leftTrimmable(const AbstractString &characters = EMPTY_CHARACTERS) const {
			size_t pos = member.find_first_not_of(characters.member);
			return pos == string_t::npos ? length() : pos;
		}
		size_t rightTrimmable(const AbstractString &characters = EMPTY_CHARACTERS) const {
			size_t pos = member.find_last_not_of(characters.member);
			return pos == string_t::npos ? length() : (member.length() - pos - 1);
		}
		AbstractString& trimLeft(const AbstractString &characters = EMPTY_CHARACTERS) {
			size_t pos = member.find_first_not_of(characters.member);
			if (pos == string_t::npos)
				member.clear();
			else
				member.erase(0, pos);
			return *this;
		}
		AbstractString& trimRight(const AbstractString &characters = EMPTY_CHARACTERS) {
			size_t pos = member.find_last_not_of(characters.member);
			if (pos == string_t::npos)
				member.clear();
			else
				member.erase(pos + 1);
			return *this;
		}
		AbstractString& trim(const AbstractString& characters = EMPTY_CHARACTERS) {
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
		size_t substringTo(Character* out, size_t pos, size_t len = string_t::npos) const {
			size_t copied = member.copy(out, len, pos);
			out[copied] = '\0';
			return copied;
		}
		/**< out must be large enough to contains len characters + 1 null-terminal character. **/
		size_t charactersTo(Character* out, size_t pos, size_t len = string_t::npos) const {
			return member.copy(out, len, pos);
		}
		/**< out must be large enough to contains just len characters. **/
		AbstractString substring(size_t pos, size_t len=string_t::npos) const {
			return AbstractString(*this, pos, len);
		}
		ArrayList<AbstractString> split(const AbstractString& delimiter) const {
			ArrayList<AbstractString> pieces;
			size_t sep_len = delimiter.member.length();
			size_t piece_start = 0;
			size_t next_start = 0;
			do {
				next_start = member.find(delimiter.member, piece_start);
				size_t piece_len = next_start - piece_start;
				pieces.add(AbstractString(*this, piece_start, piece_len));
				piece_start = next_start + sep_len;
			} while (next_start != string_t::npos);
			return pieces;
		}
		ArrayList<AbstractString> split(const Character* delimiter) const {
			ArrayList<AbstractString> pieces;
			size_t sep_len = Char::stringlength(delimiter);
			size_t piece_start = 0;
			size_t next_start = 0;
			do {
				next_start = member.find(delimiter, piece_start);
				pieces.add(AbstractString(*this, piece_start, next_start - piece_start));
				piece_start = next_start + sep_len;
			} while (next_start != string_t::npos);
			return pieces;
		}
		ArrayList<AbstractString> split(const CharString& delimiter) const {
			return split(AbstractString(delimiter));
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
		bool contains(const CharString& s, size_t start = 0) const {
			return contains(AbstractString(s));
		}
		bool startsWith(const AbstractString& s) const {
			return member.compare(0, s.member.length(), s.member) == 0;
		}
		bool startsWith(const Character* s) const {
			return member.compare(0, Char::stringlength(s), s) == 0;
		}
		bool startsWith(const CharString& s) const {
			return startsWith(AbstractString(s));
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
		bool endsWith(const CharString& s) const {
			return endsWith(AbstractString(s));
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
		pos_t indexOf(const CharString& s, size_t start = 0) const {
			return indexOf(AbstractString(s), start);
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
		pos_t lastIndexOf(const CharString& s, size_t start = string_t::npos) const {
			return lastIndexOf(AbstractString(s), start);
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
		int crossCompare(const Character* other) const override {
			return member.compare(other);
		}
	};

}

#endif //SRC_CIGMAR_ABSTRACTSTRING_HPP
