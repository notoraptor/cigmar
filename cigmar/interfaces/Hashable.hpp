#ifndef CIGMAR_HASHABLE
#define CIGMAR_HASHABLE

#include <cstddef>

namespace cigmar {
	class Hashable {
	public:
		virtual size_t hash() const = 0;
	};
}

template<> struct std::hash<cigmar::Hashable> {
	size_t operator()(const cigmar::Hashable& o) const {
		return o.hash();
	}
};

#endif
