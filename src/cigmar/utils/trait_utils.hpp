//
// Created by notoraptor on 04/03/2018.
//

#ifndef SRC_CIGMAR_TRAITS_HPP
#define SRC_CIGMAR_TRAITS_HPP

#include <type_traits>
#include <iostream>

namespace cigmar {
	namespace trait_utils {
		/* Definition of static traits. Inspired from:
		 * https://stackoverflow.com/a/29634934
		 * https://ideone.com/ExTsEO
		 * (2017/09/24) */

		template<typename T>
		auto is_iterable(int)
		-> decltype(std::begin(std::declval<T &>()) != std::end(std::declval<T &>()), // begin/end and operator !=
				++std::declval<decltype(std::begin(std::declval<T &>())) &>(), // operator ++
				*std::begin(std::declval<T &>()), // operator*
				std::true_type{});
		template<typename T>
		std::false_type is_iterable(...);

		template<typename T>
		auto has_method_toStream(int)
		-> decltype(static_cast<void (T::*)(std::ostream &)>(&T::toStream), std::true_type{});
		template<typename T>
		std::false_type has_method_toStream(...);

		template<typename T>
		auto is_directly_streamable(int)
		-> decltype(std::declval<std::ostream &>() << std::declval<T &>(), std::true_type{});
		template<typename T>
		std::false_type is_directly_streamable(...);

		template<typename T>
		auto has_method_hash(int)
		-> decltype(static_cast<size_t(T::*)() const>(&T::hash), std::true_type{});
		template<typename T>
		std::false_type has_method_hash(...);

		template<typename A, typename B>
		struct isinstance<A, B>: public std::conditional<std::is_same<A, B>{}, std::true_type, std::false_type>::type {};
		template<typename A, typename B, typename... Types>
		struct isinstance: public std::conditional<std::is_same<A, B>{}, std::true_type, isinstance<A, Types...>>::type {};
	}
	/**
	~~~~
	cigmar::is_iterable<MyType>::value
	~~~~

	Type trait to statically check if a symbol T is iterable according to C++ standard library
	iteration model (ie. with `begin()` and `end()` methods/functions.

	Reference (2017/09/24):
	- https://stackoverflow.com/a/29634934
	- https://ideone.com/ExTsEO
	**/
	template<typename T> using is_iterable = decltype(trait_utils::is_iterable<T>(0));
	template<typename T> using is_directly_streamable = decltype(trait_utils::is_directly_streamable<T>(0));
	template<typename T> using has_method_toStream = decltype(trait_utils::has_method_toStream<T>(0));
	template<typename T> using has_method_hash = decltype(trait_utils::has_method_hash<T>(0));
	template<typename T, typename... Types> using isinstance = trait_utils::isinstance<T, Types...>;
	template<typename T> using is_char_type = isinstance<T, char, wchar_t, char16_t, char32_t>;
	#define ASSERT_CHARTYPE(type) static_assert(is_char_type<type>{}, "A character type is required.")
}

#endif //SRC_CIGMAR_TRAITS_HPP
