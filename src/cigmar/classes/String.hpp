#ifndef CIGMAR_STRING
#define CIGMAR_STRING

#include <algorithm>
#include <string>
#include <sstream>
#include <cstring>
#include <cigmar/interfaces/Streamable.hpp>
#include <cigmar/interfaces/Hashable.hpp>
#include <cigmar/interfaces/Comparable.hpp>
#include <cigmar/types/pos_t.hpp>
#include <cigmar/classes/ArrayList.hpp>
#include <cigmar/classes/Char.hpp>
#include <cigmar/symbols.hpp>
#include <cigmar/interfaces/CrossComparable.hpp>

#define EMPTY_CHARACTERS " \f\n\r\t\v"

namespace cigmar {

// Motion fully-defined.

class String:
		public Streamable,
		public Hashable,
		public Comparable<String>,
		public CrossComparable<const char*> {
private:
	std::string member;
public:
	typedef std::string::iterator iterator_t;
	typedef std::string::const_iterator const_iterator_t;
private:
	static void concatenate(std::ostringstream& o) {}
	template<typename T, typename... Args>
	static void concatenate(std::ostringstream& o, T variable, Args... args) {
		o << variable;
		concatenate(o, args...);
	}
	template<typename... Args>
	static void concatenate(std::ostringstream& o, const char* s, Args... args) {
		o << s;
		concatenate(o, args...);
	}
	template<typename... Args>
	static void spaceFirst(std::ostringstream& o, const char* s, Args... args) {
		o << s;
		spaceNext(o, args...);
	}
	template<typename T, typename... Args>
	static void spaceFirst(std::ostringstream& o, T variable, Args... args) {
		o << variable;
		spaceNext(o, args...);
	}
	template<typename... Args>
	static void spaceNext(std::ostringstream& o, const char* s, Args... args) {
		o << ' ' << s;
		spaceNext(o, args...);
	}
	template<typename T, typename... Args>
	static void spaceNext(std::ostringstream& o, T variable, Args... args) {
		o << ' ' << variable;
		spaceNext(o, args...);
	}
	static void spaceNext(std::ostringstream& o) {}
public:
	String(): member() {}
	String(const char* s): member(s) {}
	String(const String& s): member(s.member) {}
	String(String&& s) noexcept: member(std::move(s.member)) {}
	String(const String& s, size_t pos, size_t len = std::string::npos): member(s.member, pos, len) {}
	String(const char* s, size_t pos, size_t len = std::string::npos): member(s + pos, len) {}
	String(size_t length, char c): member(length, c) {}
	// STL
	String(const std::string& s): member(s) {}
	String(std::string&& s): member(std::move(s)) {}

	template<typename... Args> static String println(Args... args) {
		std::ostringstream o;
		o << std::boolalpha;
		spaceFirst(o, args...);
		o << std::endl;
		return String(o.str());
	};
	template<typename... Args> static String print(Args... args) {
		std::ostringstream o;
		o << std::boolalpha;
		spaceFirst(o, args...);
		return String(o.str());
	};
	template<typename... Args> static String writeln(Args... args) {
		std::ostringstream o;
		o << std::boolalpha;
		concatenate(o, args...);
		o << std::endl;
		return String(o.str());
	};
	template<typename... Args> static String write(Args... args) {
		std::ostringstream o;
		o << std::boolalpha;
		concatenate(o, args...);
		return String(o.str());
	}

	String& operator=(const char* s) {
		member = s;
		return *this;
	}
	String& operator=(const String& other) {
		member = other.member;
		return *this;
	}
	String& operator=(String&& s) noexcept {
		member = std::move(s.member);
		return *this;
	};
	// STL
	String& operator=(const std::string& s) {
		member = s;
		return *this;
	}
	// STL
	String& operator=(std::string&& s) noexcept {
		member = std::move(s);
		return *this;
	}

	template<typename T>
	String& operator<<(T variable) {
		std::ostringstream o;
		o << variable;
		member += o.str();
		return *this;
	}
	String& operator<<(const char* s) {
		member += s;
		return *this;
	}
	String& operator<<(const String& s) {
		member += s.member;
		return *this;
	}
	String& operator<<(bool val) {
		member += (val ? "true" : "false");
		return *this;
	}

	char& operator[](size_t pos) {return member[pos];}
	char& operator[](last_t) {return member.back();}
	const char& operator[](size_t pos) const {return member[pos];}
	const char& operator[](last_t) const {return member.back();}

	explicit operator const char*() const {return member.c_str();}
	explicit operator bool() const {return !member.empty();}

	String operator()() const {return String(*this);}
	String operator()(size_t pos, size_t len) const {return String(*this, pos, len);}
	String operator()(size_t pos, last_t last) const {return String(*this, pos);}

	std::string& cppstring() {return member;}
	/**< Give read-write access to internal wrapped std::string object.
	 * **NB**: This is acceptable as long as this class uses only a std::string as
	 * internal object and does not have to update any other object when its
	 * std::string is updated. **/
	const std::string& cppstring() const {return member;}
	/**< Give read access to internal wrapped std::string object.
	 * This allows to use String everywhere a std::string is required. **/
	const char* cstring() const {return member.c_str();}
	size_t length() const {return member.length();}
	iterator_t begin() {return member.begin();}
	iterator_t end() {return member.end();}
	const_iterator_t begin() const {return member.begin();}
	const_iterator_t end() const {return member.end();}

	String& clear() {
		member.clear();
		return *this;
	}
	String& swap(String& other) {
		member.swap(other.member);
		return *this;
	}
	// STL
	String& swap(std::string& other) {
		member.swap(other);
		return *this;
	}
	String& insert(size_t pos, const String& s) {
		member.insert(pos, s.member);
		return *this;
	}
	String& insert(size_t pos, const char* s) {
		member.insert(pos, s);
		return *this;
	}
	String& replace(size_t pos, size_t len, const String& s) {
		member.replace(pos, len, s.member);
		return *this;
	}
	String& replace(size_t pos, size_t len, const char* s) {
		member.replace(pos, len, s);
		return *this;
	}
	String& replace(const String& from, const String& to) {
		size_t from_len = from.member.length();
		size_t to_len = to.member.length();
		size_t pos = 0;
		do {
			pos = member.find(from.member, pos);
			if (pos != std::string::npos) {
				member.replace(pos, from_len, to.member);
				pos += to_len;
			}
		} while(pos != std::string::npos);
		return *this;
	}
	String& replace(const String& from, const char* to) {
		size_t from_len = from.member.length();
		size_t to_len = strlen(to);
		size_t pos = 0;
		do {
			pos = member.find(from.member, pos);
			if (pos != std::string::npos) {
				member.replace(pos, from_len, to);
				pos += to_len;
			}
		} while(pos != std::string::npos);
		return *this;
	}
	String& replace(const char* from, const String& to) {
		size_t from_len = strlen(from);
		size_t to_len = to.member.length();
		size_t pos = 0;
		do {
			pos = member.find(from, pos);
			if (pos != std::string::npos) {
				member.replace(pos, from_len, to.member);
				pos += to_len;
			}
		} while(pos != std::string::npos);
		return *this;
	}
	String& replace(const char* from, const char* to) {
		size_t from_len = strlen(from);
		size_t to_len = strlen(to);
		size_t pos = 0;
		do {
			pos = member.find(from, pos);
			if (pos != std::string::npos) {
				member.replace(pos, from_len, to);
				pos += to_len;
			}
		} while(pos != std::string::npos);
		return *this;
	}
	size_t ltrimmable(const String& characters = EMPTY_CHARACTERS) const {
		size_t pos = member.find_first_not_of(characters.member);
		return pos == std::string::npos ? length() : pos;
	}
	size_t rtrimmable(const String& characters = EMPTY_CHARACTERS) const {
		size_t pos = member.find_last_not_of(characters.member);
		return pos == std::string::npos ? length() : (member.length() - pos - 1);
	}
	String& ltrim(const String& characters = EMPTY_CHARACTERS) {
		size_t pos = member.find_first_not_of(characters.member);
		if (pos == std::string::npos)
			member.clear();
		else
			member.erase(0, pos);
		return *this;
	}
	String& rtrim(const String& characters = EMPTY_CHARACTERS) {
		size_t pos = member.find_last_not_of(characters.member);
		if (pos == std::string::npos)
			member.clear();
		else
			member.erase(pos + 1);
		return *this;
	}
	String& trim(const String& characters = EMPTY_CHARACTERS) {
		ltrim(characters);
		rtrim(characters);
		return *this;
	}
	String& lower() {
		std::transform(member.begin(), member.end(), member.begin(), Char::lower);
		return *this;
	}
	String& upper() {
		std::transform(member.begin(), member.end(), member.begin(), Char::upper);
		return *this;
	}
	size_t substringTo(char* out, size_t pos, size_t len = std::string::npos) const {
		size_t copied = member.copy(out, len, pos);
		out[copied] = '\0';
		return copied;
	}
	/**< out must be large enough to contains len characters + 1 null-terminal character. **/
	size_t charactersTo(char* out, size_t pos, size_t len = std::string::npos) const {
		return member.copy(out, len, pos);
	}
	/**< out must be large enough to contains just len characters. **/
	String substring(size_t pos, size_t len=std::string::npos) const {
		return String(*this, pos, len);
	}
	ArrayList<String> split(const String& delimiter) const {
		ArrayList<String> pieces;
		size_t sep_len = delimiter.member.length();
		size_t piece_start = 0;
		size_t next_start = 0;
		do {
			next_start = member.find(delimiter.member, piece_start);
			size_t piece_len = next_start - piece_start;
			pieces.add(std::move(String(*this, piece_start, piece_len)));
			piece_start = next_start + sep_len;
		} while (next_start != std::string::npos);
		return pieces;
	}
	ArrayList<String> split(const char* delimiter) const {
		ArrayList<String> pieces;
		size_t sep_len = strlen(delimiter);
		size_t piece_start = 0;
		size_t next_start = 0;
		do {
			next_start = member.find(delimiter, piece_start);
			pieces.add(std::move(String(*this, piece_start, next_start - piece_start)));
			piece_start = next_start + sep_len;
		} while (next_start != std::string::npos);
		return pieces;
	}
	bool contains(const String& s, size_t start = 0) const {
		return member.find(s.member, start) != std::string::npos;
	}
	bool contains(const char* s, size_t start = 0) const {
		return member.find(s, start) != std::string::npos;
	}
	bool contains(char c, size_t start = 0) const {
		return member.find(c, start) != std::string::npos;
	}
	bool startsWith(const String& s) const {
		return member.compare(0, s.member.length(), s.member) == 0;
	}
	bool startsWith(const char* s) const {
		return member.compare(0, strlen(s), s) == 0;
	}
	bool endsWith(const String& s) const {
		size_t len_m = member.length();
		size_t len_s = s.member.length();
		return len_m >= len_s && member.compare(len_m - len_s, len_s, s.member) == 0;
	}
	bool endsWith(const char* s) const {
		size_t len_m = member.length();
		size_t len_s = strlen(s);
		return len_m >= len_s && member.compare(len_m - len_s, len_s, s) == 0;
	}
	pos_t indexOf(const String& s, size_t start = 0) const {
		return pos_t::_stringpos(member.find(s.member, start));
	}
	pos_t indexOf(const char* s, size_t start = 0) const {
		return pos_t::_stringpos(member.find(s, start));
	}
	pos_t indexOf(char c, size_t start = 0) const {
		return pos_t::_stringpos(member.find(c, start));
	}
	pos_t lastIndexOf(const String& s, size_t start = std::string::npos) const {
		return pos_t::_stringpos(member.rfind(s.member, start));
	}
	pos_t lastIndexOf(const char* s, size_t start = std::string::npos) const {
		return pos_t::_stringpos(member.rfind(s, start));
	}
	pos_t lastIndexOf(char c, size_t start = std::string::npos) const {
		return pos_t::_stringpos(member.rfind(c, start));
	}

	void toStream(std::ostream& o) const override {
		o << member;
	}
	size_t hash() const override {
		return std::hash<std::string>()(member);
	}
	int compare(const String& other) const override {
		return member.compare(other.member);
	}
	int crossCompare(const char* other) const override {
		return member.compare(other);
	}
};

inline String operator "" _s(const char* s, size_t len) {
	return String(s);
}

}

#endif // CIGMAR_STRING
