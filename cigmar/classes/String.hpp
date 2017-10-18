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
#include <cigmar/types/return_t.hpp>
#include <cigmar/classes/ArrayList.hpp>
#include <cigmar/symbols.hpp>

namespace cigmar {

// Motion fully-defined.

class String: public Streamable, public Hashable, public Comparable<String> {
private:
	std::string member;
public:
	typedef std::string::iterator iterator_t;
	typedef std::string::const_iterator const_iterator_t;
	static char to_lower(char c);
	static char to_upper(char c);
	static const char endl;
private:
	template<typename T, typename... Args>
	void concatenate(std::ostringstream& o, T variable, Args... args) {
		o << variable;
		concatenate(o, args...);
	}
	template<typename... Args>
	void concatenate(std::ostringstream& o, const char* s, Args... args) {
		o << s;
		concatenate(o, args...);
	}
	void concatenate(std::ostringstream& o) {}
protected:
public:
	String(): member() {}
	String(const char* s): member(s) {}
	String(const String& s): member(s.member) {}
	String(String&& s): member(std::move(s.member)) {}
	String(const String& s, size_t pos, size_t len = std::string::npos): member(s.member, pos, len) {}
	template<typename... Args>
	String(Args... args): member() {
		std::ostringstream o;
		o << std::boolalpha;
		concatenate(o, args...);
		member = o.str();
	}

	String& operator=(String&& s) {
		member = std::move(s.member);
		return *this;
	};
	String& operator=(const String& other) {
		member = other.member;
		return *this;
	}
	String& operator=(const char* s) {
		member = s;
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
	String& operator<<(bool val) {
		member += (val ? "true" : "false");
		return *this;
	}

	char& operator[](size_t pos) {return member[pos];}
	const char& operator[](size_t pos) const {return member[pos];}
	char& operator[](_LAST) {return member.back();}
	const char& operator[](_LAST) const {return member.back();}

	explicit operator bool() const {return !member.empty();}
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
	String& lower() {
		std::transform(member.begin(), member.end(), member.begin(), String::to_lower);
		return *this;
	}
	String& upper() {
		std::transform(member.begin(), member.end(), member.begin(), String::to_upper);
		return *this;
	}
	String& extractTo(String& out, size_t pos, size_t len = std::string::npos) {
		out.member.assign(member, pos, len);
		return *this;
	}
	const String& extractTo(String& out, size_t pos, size_t len = std::string::npos) const {
		return this->extractTo(out, pos, len);
	}
	size_t extractTo(char* out, size_t pos, size_t len = std::string::npos) const {
		size_t copied = member.copy(out, len, pos);
		out[copied] = '\0';
		return copied + 1;
	}
	size_t memout(char* out, size_t pos, size_t len = std::string::npos) const {
		return member.copy(out, len, pos);
	}

	String toLower() const {
		return String(*this).lower();
	}
	String toUpper() const {
		return String(*this).upper();
	}
	String substring(size_t pos, size_t len=std::string::npos) const {
		return String(*this, pos, len);
	}
	return_t<ArrayList<String>> split(const String& delimiter) const {
		ArrayList<String>* pieces = new ArrayList<String>();
		size_t sep_len = delimiter.member.length();
		size_t piece_start = 0;
		size_t next_start = 0;
		do {
			next_start = member.find(delimiter.member, piece_start);
			size_t piece_len = next_start - piece_start;
			pieces->add(std::move(String(*this, piece_start, piece_len)));
			piece_start = next_start + sep_len;
		} while (next_start != std::string::npos);
		return transfer(pieces);
	}
	return_t<ArrayList<String>> split(const char* delimiter) const {
		ArrayList<String>* pieces = new ArrayList<String>();
		size_t sep_len = strlen(delimiter);
		size_t piece_start = 0;
		size_t next_start = 0;
		do {
			next_start = member.find(delimiter, piece_start);
			pieces->add(std::move(String(*this, piece_start, next_start - piece_start)));
			piece_start = next_start + sep_len;
		} while (next_start != std::string::npos);
		return transfer(pieces);
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
	pos_t lastIndexOf(const char* s, size_t start) const {
		return pos_t::_stringpos(member.rfind(s, start));
	}
	pos_t lastIndexOf(char c, size_t start) const {
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
	int compare(const char* other) const {
		return member.compare(other);
	}
};

inline String operator "" _s(const char* s, size_t len) {
	return String(s);
}

COMPARABLE(String, const char*);

}

HASHABLE(cigmar::String);

#endif // CIGMAR_STRING
