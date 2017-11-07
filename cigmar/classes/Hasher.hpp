//
// Created by notoraptor on 07/11/2017.
//

#ifndef CIGMAR_HASHER_HPP
#define CIGMAR_HASHER_HPP

#include <cigmar/interfaces/Hashable.hpp>
#include <functional>
#include <type_traits>

namespace cigmar {
	class Hasher {
	public:
		template<typename T>
		size_t operator()(const T& object) const {
			typedef typename std::conditional<std::is_base_of<Hashable, T>::value, Hashable, T>::type type_to_hash;
			return std::hash<type_to_hash>()(object);
		}
		/*
		size_t operator()(const Hashable& hashable) const {
			return hashable.hash();
		}
		*/
	};
}

#endif //CIGMAR_HASHER_HPP
