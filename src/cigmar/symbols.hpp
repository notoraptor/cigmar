//
// Created by notoraptor on 20/02/2018.
//

#ifndef SRC_CIGMAR_INTERNAL_DEFINITIONS_HPP
#define SRC_CIGMAR_INTERNAL_DEFINITIONS_HPP
#include <cstdint>
#include <type_traits>
#include <ostream>
#include <sstream>
#include <iostream>
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
	extern last_t LAST;
	extern max_t MAX;

	namespace special_traits {
		/* Definition of static traits. Inspired from:
		 * https://stackoverflow.com/a/29634934
		 * https://ideone.com/ExTsEO
		 * (2017/09/24) */

		template<typename T>
		auto is_iterable(int)
		-> decltype(std::begin(std::declval<T &>()) != std::end(std::declval<T &>()), // begin/end and operator !=
					++std::declval<decltype(std::begin(std::declval<T &>())) & >(), // operator ++
					*std::begin(std::declval<T &>()), // operator*
					std::true_type{});
		template<typename T>
		std::false_type is_iterable(...);

		template<typename T>
		auto has_method_toStream(int)
		-> decltype(static_cast<void(T::*)(std::ostream&)>(&T::toStream), std::true_type{});
		template<typename T>
		std::false_type has_method_toStream(...);

		template<typename T>
		auto is_directly_streamable(int)
		-> decltype(std::declval<std::ostream&>() << std::declval<T&>(), std::true_type{});
		template<typename T>
		std::false_type is_directly_streamable(...);
	}
	namespace special_classes {
		struct AutoStream {
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
		struct ToStream {
			template <typename T>
			static void print(std::ostream& o, const T& value) {
				value.toStream(o);
			}
		};
		struct DefaultStream {
			template<typename E>
			static void print(std::ostream& o, const E& value) {
				o << "{object&" << (void*)(&value) << '}';
			}
		};
		template<typename T>
		struct Streamer {
			const T& value;
			explicit Streamer(const T& v): value(v) {};
		};
	}
	namespace special_functions {
		template <typename C>
		void writeElement(std::basic_ostream<C>& o) {}
		template <typename C, typename T, typename... Args>
		void writeElement(std::basic_ostream<C>& o, const T& variable, Args&&... args) {
			o << special_classes::Streamer<T>(variable);
			writeElement(o, std::forward<Args>(args)...);
		};
		template <typename C>
		void printElement(std::basic_ostream<C>& o) {}
		template <typename C, typename T, typename... Args>
		void printElement(std::basic_ostream<C>& o, const T& variable, Args&&... args) {
			o << ' ';
			o << special_classes::Streamer<T>(variable);
			printElement(o, std::forward<Args>(args)...);
		};
		template <typename C>
		void printFirstElement(std::basic_ostream<C>& o) {}
		template <typename C, typename T, typename... Args>
		void printFirstElement(std::basic_ostream<C>& o, const T& variable, Args&&... args) {
			o << special_classes::Streamer<T>(variable);
			printElement(o, std::forward<Args>(args)...);
		};
	}

	/**
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
	template<typename T> using is_iterable = decltype(special_traits::is_iterable<T>(0));
	template<typename T> using has_method_toStream = decltype(special_traits::has_method_toStream<T>(0));
	template<typename T> using is_directly_streamable = decltype(special_traits::is_directly_streamable<T>(0));
	template<typename T> using is_streamable = typename std::conditional<
			is_directly_streamable<T>{},
			std::true_type, typename std::conditional<has_method_toStream<T>{}, std::true_type, std::false_type>::type
	>::type;

	template <typename A, typename B, typename... Types>
	struct isinstance: public std::conditional<std::is_same<A, B>{}, std::true_type, isinstance<A, Types...>>::type {};
	template <typename A, typename B>
	struct isinstance<A, B>: public std::conditional<std::is_same<A, B>{}, std::true_type, std::false_type>::type {};

	template <typename T> using is_char_type = isinstance<T, char, wchar_t, char16_t, char32_t>;

	#define ASSERT_CHARTYPE(type) static_assert(is_char_type<type>{}, "A character type is required.")

	#define EMPTY_CHARACTERS {' ', '\f', '\n', '\r', '\t', '\v', '\0'}

	#ifdef WIN32
		#define ENDL {'\r', '\n', '\0'}
	#elif defined(__APPLE__)
		#define ENDL {'\r', '\0'}
	#else
		#define ENDL {'\n', '\0'}
	#endif

	#define TO_STREAM(o, type, v) \
	template<typename C> std::basic_ostream<C>& operator<<(std::basic_ostream<C>& o, type v)

	/** Global methods. **/
	TO_STREAM(o, byte_t, b) {
		return (o << (int)b);
	}
	TO_STREAM(o, ubyte_t, b) {
		return (o << (unsigned int)b);
	}
	template <typename C, typename T>
	std::basic_ostream<C>& operator<<(std::basic_ostream<C>& o, const special_classes::Streamer<T>& streamer) {
		typedef typename std::conditional<
				is_directly_streamable<T>{},
				special_classes::AutoStream,
				typename std::conditional<
						has_method_toStream<T>{},
						special_classes::ToStream,
						special_classes::DefaultStream
				>::type
		>::type streamer_t;
		streamer_t::print(o, streamer.value);
		return o;
	}
	template <typename T>
	special_classes::Streamer<T> streamer(const T& v) {
		return special_classes::Streamer<T>(v);
	}
}

#endif //SRC_CIGMAR_INTERNAL_DEFINITIONS_HPP
