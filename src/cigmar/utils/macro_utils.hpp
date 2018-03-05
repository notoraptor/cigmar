//
// Created by notoraptor on 04/03/2018.
//

#ifndef SRC_CIGMAR_MACROS_HPP
#define SRC_CIGMAR_MACROS_HPP

#define EMPTY_CHARACTERS {' ', '\f', '\n', '\r', '\t', '\v', '\0'}

#ifdef WIN32
#define ENDL {'\r', '\n', '\0'}
#elif defined(__APPLE__)
#define ENDL {'\r', '\0'}
#else
#define ENDL {'\n', '\0'}
#endif

// Thanks to https://stackoverflow.com/questions/24481810/how-to-remove-the-enclosing-parentheses-with-macro
#define ESC(...)  __VA_ARGS__

#define MAKE_AUTO_COMPARABLE(Class) \
	inline bool operator==(const Class& a, const Class& b) {return a.compare(b) == 0;} \
	inline bool operator!=(const Class& a, const Class& b) {return a.compare(b) != 0;} \
	inline bool operator< (const Class& a, const Class& b) {return a.compare(b) < 0; } \
	inline bool operator> (const Class& a, const Class& b) {return a.compare(b) > 0; } \
	inline bool operator<=(const Class& a, const Class& b) {return a.compare(b) <= 0;} \
	inline bool operator>=(const Class& a, const Class& b) {return a.compare(b) >= 0;}

#define MAKE_COMPARABLE(Class, Compared) \
	inline bool operator==(const Class& a, Compared b) {return a.compare(b) == 0;} \
	inline bool operator!=(const Class& a, Compared b) {return a.compare(b) != 0;} \
	inline bool operator< (const Class& a, Compared b) {return a.compare(b) < 0; } \
	inline bool operator> (const Class& a, Compared b) {return a.compare(b) > 0; } \
	inline bool operator<=(const Class& a, Compared b) {return a.compare(b) <= 0;} \
	inline bool operator>=(const Class& a, Compared b) {return a.compare(b) >= 0;} \
	inline bool operator==(Compared b, const Class& a) {return a.compare(b) == 0;} \
	inline bool operator!=(Compared b, const Class& a) {return a.compare(b) != 0;} \
	inline bool operator< (Compared b, const Class& a) {return a.compare(b) > 0; } \
	inline bool operator> (Compared b, const Class& a) {return a.compare(b) < 0; } \
	inline bool operator<=(Compared b, const Class& a) {return a.compare(b) >= 0;} \
	inline bool operator>=(Compared b, const Class& a) {return a.compare(b) <= 0;}

#define MAKE_TEMPLATE_AUTO_COMPARABLE(Template, Class) \
	ESC Template bool operator==(const Class& a, const Class& b) {return a.compare(b) == 0;} \
	ESC Template bool operator!=(const Class& a, const Class& b) {return a.compare(b) != 0;} \
	ESC Template bool operator< (const Class& a, const Class& b) {return a.compare(b) < 0; } \
	ESC Template bool operator> (const Class& a, const Class& b) {return a.compare(b) > 0; } \
	ESC Template bool operator<=(const Class& a, const Class& b) {return a.compare(b) <= 0;} \
	ESC Template bool operator>=(const Class& a, const Class& b) {return a.compare(b) >= 0;}

#define MAKE_TEMPLATE_COMPARABLE(Template, Class, Compared) \
	ESC Template bool operator==(const Class& a, Compared b) {return a.compare(b) == 0;} \
	ESC Template bool operator!=(const Class& a, Compared b) {return a.compare(b) != 0;} \
	ESC Template bool operator< (const Class& a, Compared b) {return a.compare(b) < 0; } \
	ESC Template bool operator> (const Class& a, Compared b) {return a.compare(b) > 0; } \
	ESC Template bool operator<=(const Class& a, Compared b) {return a.compare(b) <= 0;} \
	ESC Template bool operator>=(const Class& a, Compared b) {return a.compare(b) >= 0;} \
	ESC Template bool operator==(Compared b, const Class& a) {return a.compare(b) == 0;} \
	ESC Template bool operator!=(Compared b, const Class& a) {return a.compare(b) != 0;} \
	ESC Template bool operator< (Compared b, const Class& a) {return a.compare(b) > 0; } \
	ESC Template bool operator> (Compared b, const Class& a) {return a.compare(b) < 0; } \
	ESC Template bool operator<=(Compared b, const Class& a) {return a.compare(b) >= 0;} \
	ESC Template bool operator>=(Compared b, const Class& a) {return a.compare(b) <= 0;}

#endif //SRC_CIGMAR_MACROS_HPP
