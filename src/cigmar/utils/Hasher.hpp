//
// Created by notoraptor on 07/11/2017.
//

#ifndef CIGMAR_HASHER_HPP
#define CIGMAR_HASHER_HPP

#include <functional>
#include <type_traits>

namespace cigmar {
	template <typename T> struct StandardHash {
		const T& val;
		explicit StandardHash(const T& v): val(v) {}
		size_t hash() const {
			return std::hash<T>()(val);
		}
	};
	template <typename T> struct MethodHash {
		const T& val;
		explicit MethodHash(const T& v): val(v) {}
		size_t hash() const {
			return val.hash();
		}
	};
	struct Hasher {
		template<typename T> size_t operator()(const T& object) const {
			typedef typename std::conditional<has_method_hash<T>{}, MethodHash<T>, StandardHash<T>>::type hasher_class;
			return hasher_class(object).hash();
		}
	};
}

#endif //CIGMAR_HASHER_HPP
