#ifndef CIGMAR_STRING
#define CIGMAR_STRING

#include <algorithm>
#include <locale>
#include <string>
#include <sstream>
#include <cstring>
#include "primitive/interfaces.hpp"

char to_lower(char c);
char to_upper(char c);

class String: public Streamable, public Hashable, public AutoComparable<String>, public Comparable<const char*> {
private:
	std::string member;
public:
	static const size_t UNKNOWN;
private:
	template<typename T, typename... Args>
	void vlist(std::ostringstream& o, T variable, Args... args) {
		o << variable;
		vlist(o, args...);
	}
	template<typename... Args>
	void vlist(std::ostringstream& o, const char* s, Args... args) {
		o << s;
		vlist(o, args...);
	}
	void vlist(std::ostringstream& o) {}
protected:
public:
	String(): member() {}
	String(const char* s): member(s) {}
	String(const String& s): member(s.member) {}
	String(const String& s, size_t pos, size_t len = std::string::npos): member() {
		member.assign(s.member, pos, len);
	}
	template<typename... Args>
	String(Args... args): member() {
		std::ostringstream o;
		vlist(o, args...);
		member = o.str();
	}

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

	char& operator[](size_t pos) {return member[pos];}
	const char& operator[](size_t pos) const {return member[pos];}
	char& operator[](_LAST) {return member.back();}
	const char& operator[](_LAST) const {return member.back();}

	explicit operator bool() const {return member.empty();}
	size_t length() const {return member.length();}

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
		std::transform(member.begin(), member.end(), member.begin(), to_lower);
		return *this;
	}
	String& upper() {
		std::transform(member.begin(), member.end(), member.begin(), to_upper);
		return *this;
	}
	String toLower() const {
		String s(*this);
		s.lower();
		return s;
	}
	String toUpper() const {
		return String(*this).upper();
	}
	String substring(size_t pos, size_t len=std::string::npos) const {
		return String(*this, pos, len);
	}
	String& extractTo(String& out, size_t pos, size_t len = std::string::npos) {
		out.member.assign(member, pos, len);
		return *this;
	}
	const String& extractTo(String& out, size_t pos, size_t len = std::string::npos) const {
		out.member.assign(member, pos, len);
		return *this;
	}
	bool contains(const String& s, size_t start = 0) const {
		return member.find(s.member, start) != std::string::npos;
	}
	bool contains(const char* s, size_t start = 0) const {
		return member.find(s, start) != std::string::npos;
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
	pos_t lastIndexOf(const String& s, size_t start = std::string::npos) const {
		return pos_t::_stringpos(member.rfind(s.member, start));
	}
	pos_t lastIndexOf(const char* s, size_t start) const {
		return pos_t::_stringpos(member.rfind(s, start));
	}
	int compare(const String& other) const override {
		return member.compare(other.member);
	}
	int compare(const char* other) const override {
		return member.compare(other);
	}
	void toStream(std::ostream& o) const override {
		o << member;
	}
	size_t hash() const override {
		return std::hash<std::string>()(member);
	}
};

HASHABLE(String);

#endif // CIGMAR_STRING
