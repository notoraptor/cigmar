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
class array_t: public Streamable, public Collection<std::array<T, N>> {
private:
	T mem[N];
public:
	const size_t length = N;
public:
	array_t() {}
	explicit array_t(T val) {
		if (val == 0) memset(mem, 0, length * sizeof(T));
		else for (size_t i = 0; i < length; ++i) mem[i] = val;
	}
	template<typename E>
	explicit array_t(const Collection<E>& collection) {
		if (collection.size() > size())
			throw Exception("Cannot create an array_t of size " , size(),
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
	array_t(std::initializer_list<T> il): array_t(StaticCollection<T>(il)) {}
	array_t(const array_t&) = delete;
	array_t(array_t&&) noexcept = default;
	void operator=(const array_t&) = delete;
	array_t& operator=(array_t&&) noexcept = default;

	size_t size() const {return length;}

	T& operator[](size_t pos) {return mem[pos];}
	const T& operator[](size_t pos) const {return mem[pos];}

	T& operator[](last_t) {return mem[length - 1];}
	const T& operator[](last_t) const {return mem[length - 1];}

	explicit operator T*() {return mem;}
	explicit operator const T*() const {return mem;}

	T* begin() {return mem;}
	T* end() {return mem + length;}
	const T* begin() const {return mem;}
	const T* end() const {return mem + length;}
	void toStream(std::ostream& o) const override {
		o << '(';
		if (length) {
			o << mem[0];
			for (size_t i = 1; i < length; ++i)
				o << "; " << mem[i];
		}
		o << ')';
	}
};

}

#endif // CIGMAR_ARRAY_T
