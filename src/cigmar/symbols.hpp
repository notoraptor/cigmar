#ifndef CIGMAR_PRIMITIVE_SYMBOLS
#define CIGMAR_PRIMITIVE_SYMBOLS

#include <cstdint>
#include <type_traits>
#include <iterator>
#include <ostream>

namespace cigmar {

	using byte_t = int8_t;
	using short_t = int16_t;
	using int_t = int32_t;
	using long_t = int64_t;
	using ubyte_t = uint8_t;
	using ushort_t = uint16_t;
	using uint_t = uint32_t;
	using ulong_t = uint64_t;

	std::ostream &operator<<(std::ostream &o, byte_t b);

	std::ostream &operator<<(std::ostream &o, ubyte_t b);

	// class AbstractList;
	// class AbstractMap;
	// class AbstractSet;
	class LinkedList; // Double linked list.
	class Tensor; // Sophisticated N-D array.

	#define dtypeof(variable) decltype(variable)::dtype

	class last_t {};

	extern last_t LAST;

	#define EMPTY_CHARACTERS " \f\n\r\t\v"
	#ifdef WIN32
		#define ENDL "\r\n"
		#define SHARED_LIBRARY_EXTENSION ".dll"
	#elif defined(__APPLE__)
		#define ENDL "\r"
		#define SHARED_LIBRARY_EXTENSION ".so"
	#else
		#define ENDL "\n"
		#define SHARED_LIBRARY_EXTENSION ".so"
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

}

#endif // CIGMAR_PRIMITIVE_SYMBOLS
