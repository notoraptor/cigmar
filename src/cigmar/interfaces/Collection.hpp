//
// Created by HPPC on 12/11/2017.
//

#ifndef SRC_CIGMAR_COLLECTION_HPP
#define SRC_CIGMAR_COLLECTION_HPP

#include <cstddef>

namespace cigmar {
	template<typename Container>
	class Collection {
	public:
		typedef Collection<Container> collection_t;
		typedef typename Container::value_type dtype;
		typedef typename Container::iterator iterator_t;
		typedef typename Container::const_iterator const_iterator_t;
		virtual iterator_t begin() = 0;
		virtual iterator_t end() = 0;
		virtual const_iterator_t begin() const = 0;
		virtual const_iterator_t end() const = 0;
		virtual size_t size() const = 0;
	};
}


#endif //SRC_CIGMAR_COLLECTION_HPP
