//
// Created by notoraptor on 20/02/2018.
//

#ifndef SRC_CIGMAR_INTERNAL_DEFINITIONS_HPP
#define SRC_CIGMAR_INTERNAL_DEFINITIONS_HPP
#include <cstdint>
#include <type_traits>
#include <iterator>
#include <ostream>
#include <sstream>
#include <iostream>
#include <cigmar/classes/exception/Exception.hpp>
#include <cigmar/unicode.hpp>

namespace cigmar {
	/** Primitive types. **/
	using byte_t = int8_t;
	using short_t = int16_t;
	using int_t = int32_t;
	using long_t = int64_t;
	using ubyte_t = uint8_t;
	using ushort_t = uint16_t;
	using uint_t = uint32_t;
	using ulong_t = uint64_t;

	/** Advanced classes. **/
	class LinkedList; // Double linked list.
	class Tensor; // Sophisticated N-D array.
	// class AbstractList;
	// class AbstractMap;
	// class AbstractSet;

	/** Classes for unique special symbols. **/
	class last_t {};
	class max_t {};

	/** Global methods. **/
	std::ostream &operator<<(std::ostream &o, byte_t b);
	std::ostream &operator<<(std::ostream &o, ubyte_t b);

	/** Special symbols. **/
	extern last_t LAST;
	extern max_t MAX;

	#define EMPTY_CHARACTERS {' ', '\f', '\n', '\r', '\t', '\v', '\0'}
	#ifdef WIN32
		#define ENDL {'\r', '\n', '\0'}
	#elif defined(__APPLE__)
		#define ENDL {'\r', '\0'}
	#else
		#define ENDL {'\n', '\0'}
	#endif

	namespace {
		/* Definition of static trait `is_iterable<type>::value`
	 * https://stackoverflow.com/a/29634934
	 * https://ideone.com/ExTsEO
	 * (2017/09/24) */

		// To allow ADL with custom begin/end
		using std::begin;
		using std::end;

		template<typename T>
		auto is_iterable_impl(int)
		-> decltype(
		begin(std::declval<T &>()) != end(std::declval<T &>()), // begin/end and operator !=
				++std::declval<decltype(begin(std::declval<T &>())) & >(), // operator ++
				*begin(std::declval<T &>()), // operator*
				std::true_type{});

		template<typename T>
		std::false_type is_iterable_impl(...);

		//////////

		template<typename T>
		auto is_streamable_impl(int)
		-> decltype(std::declval<std::ostream&>() << std::declval<T&>(), std::true_type{});

		template<typename T>
		std::false_type is_streamable_impl(...);
	}

	template<typename T>
	using is_iterable = decltype(is_iterable_impl<T>(0));
	/**<
	~~~~
	cigmar::is_iterable<MyType>::value
	~~~~

	Type trait to statically check if a symbol T
	is iterable according to C++ standard library
	iteration model (ie. with `begin()` and `end()`
	methods/functions.

	Reference (2017/09/24):
	- https://stackoverflow.com/a/29634934
	- https://ideone.com/ExTsEO
	**/

	template<typename T>
	using is_streamable = decltype(is_streamable_impl<T>(0));

	template <typename T> struct is_char_type: std::false_type {};

	template <> struct is_char_type<char>: std::true_type {};
	template <> struct is_char_type<wchar_t>: std::true_type {};
	template <> struct is_char_type<char16_t>: std::true_type {};
	template <> struct is_char_type<char32_t>: std::true_type {};

	#define ASSERT_CHARTYPE(type) static_assert(is_char_type<type>{}, "A character type is required (see type trait is_char_tyepe<T>).")

	namespace {
		struct AutoStreamer {
			template<typename E>
			static void print(std::ostream& o, const E& value) {
				o << value;
			}
			static void print(std::ostream& o, bool value) {
				o << (value ? "true" : "false");
			}
			static void print(std::ostream& o, const wchar_t* s) {
				unicode::convert(s, o);
			}
			static void print(std::ostream& o, const char16_t* s) {
				unicode::convert(s, o);
			}
			static void print(std::ostream& o, const char32_t* s) {
				unicode::convert(s, o);
			}
		};
		struct DefaultStreamer {
			template<typename E>
			static void print(std::ostream& o, const E& value) {
				o << "{object&" << (void*)(&value) << '}';
			}
		};
	}

	namespace sys {
		/// Definitions for print(ln)/write(ln) functions.
		inline void writeElement(std::ostream& o) {}
		template<typename T, typename... Args> void writeElement(std::ostream& o, T variable, Args&&... args) {
			AutoStreamer::print(o, variable);
			writeElement(o, std::forward<Args>(args)...);
		}
		inline void printElement(std::ostream& o) {}
		template<typename T, typename... Args> void printElement(std::ostream& o, T variable, Args&&... args) {
			o << ' ';
			AutoStreamer::print(o, variable);
			printElement(o, std::forward<Args>(args)...);
		}
		inline void printFirstElement(std::ostream& o) {}
		template<typename T, typename... Args> void printFirstElement(std::ostream& o, T variable, Args&&... args) {
			AutoStreamer::print(o, variable);
			printElement(o, std::forward<Args>(args)...);
		}
		namespace path {
			extern const char* const separator;
			bool isRooted(const char* pathname);
			namespace {
				inline void concatenate(std::ostringstream &o) {}
				template<typename T, typename... Args>
				void concatenate(std::ostringstream &o, T variable, Args... args) {
					std::ostringstream temp;
					temp << variable;
					if (isRooted(temp.str().c_str()))
						throw Exception("Cannot build path with rooted element inside.");
					o << separator << temp.str();
					concatenate(o, args...);
				}
				template<typename T, typename... Args>
				void concatenateFirst(std::ostringstream &o, T variable, Args... args) {
					o << variable;
					concatenate(o, args...);
				}
			}
		}
	}
}

#endif //SRC_CIGMAR_INTERNAL_DEFINITIONS_HPP
