#ifndef CIGMAR_HASHABLE
#define CIGMAR_HASHABLE

#include <cstddef>

namespace cigmar {
	struct Hashable {
		virtual size_t hash() const = 0;
	};
}

namespace std {
	template<>
	struct hash<cigmar::Hashable> {
		size_t operator()(const cigmar::Hashable &o) const {
			return o.hash();
		}
	};
}

#endif
