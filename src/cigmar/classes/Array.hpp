#ifndef CIGMAR_ARRAY_T
#define CIGMAR_ARRAY_T

#include <cstring>
#include <array>
#include <cigmar/symbols.hpp>
#include <cigmar/interfaces/Streamable.hpp>
#include <cigmar/interfaces/Collection.hpp>
#include <cigmar/exception/Exception.hpp>
#include <cigmar/classes/StaticCollection.hpp>

namespace cigmar {

// Motion fully-defined.

template<typename T, size_t N>
class Array: public Streamable, public Collection<std::array<T, N>> {
	T mem[N];
public:
	Array() {}
	explicit Array(T val) {
		if (val == 0) memset(mem, 0, N * sizeof(T));
		else for (size_t i = 0; i < N; ++i) mem[i] = val;
	}
	template<typename E>
	explicit Array(const Collection<E>& collection) {
		if (collection.size() > size())
			throw Exception("Cannot create an Array of size " , size(),
			                " from a collection with bigger size ", collection.size() ,".");
		size_t i = 0;
		auto cit = collection.begin();
		auto cend = collection.end();
		while (cit != cend) {
			mem[i] = *cit;
			++cit;
			++i;
		}
		if (i < size())
			memset(mem + i, 0, (size() - i) * sizeof(T));
	};
	Array(std::initializer_list<T> il): Array(StaticCollection<T>(il)) {}
	Array(const Array&) = delete;
	Array(Array&&) noexcept = default;
	void operator=(const Array&) = delete;
	Array& operator=(Array&&) noexcept = default;

	T& operator[](size_t pos) {return mem[pos];}
	const T& operator[](size_t pos) const {return mem[pos];}
	T& operator[](last_t) {return mem[N - 1];}
	const T& operator[](last_t) const {return mem[N - 1];}
	explicit operator T*() {return mem;}
	explicit operator const T*() const {return mem;}
	size_t size() const {return N;}

	T* begin() {return mem;}
	T* end() {return mem + N;}
	const T* begin() const {return mem;}
	const T* end() const {return mem + N;}
	void toStream(std::ostream& o) const override {
		o << '(';
		if (N) {
			o << mem[0];
			for (size_t i = 1; i < N; ++i)
				o << "; " << mem[i];
		}
		o << ')';
	}
};

}

#endif // CIGMAR_ARRAY_T
