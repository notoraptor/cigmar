//
// Created by HPPC on 12/11/2017.
//

#ifndef SRC_CIGMAR_STATICCOLLECTION_HPP
#define SRC_CIGMAR_STATICCOLLECTION_HPP

#include <initializer_list>
#include <cigmar/interfaces/Collection.hpp>

namespace cigmar {
	template <typename T>
	class StaticCollection: public Collection<T, const T*, const T*> {
	private:
		std::initializer_list<T> ril;
	public:
		StaticCollection(std::initializer_list<T> il): ril(il) {}
		size_t size() const override {return ril.size();}
		const T* begin() const override {return ril.begin();}
		const T* end() const override {return ril.end();}
		const T* begin() override {return ril.begin();}
		const T* end() override {return ril.end();}
	};
}


#endif //SRC_CIGMAR_STATICCOLLECTION_HPP
