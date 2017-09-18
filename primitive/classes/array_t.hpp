#ifndef CIGMAR_ARRAY_T
#define CIGMAR_ARRAY_T

#include <cstring>
#include "../symbols.hpp"
#include "../interfaces/Streamable.hpp"

// Motion fully-defined.

template<typename T, size_t N>
class array_t: public Streamable {
private:
	T mem[N];
public:
	typedef T dtype;
	const size_t length = N;
public:
	array_t() {}
	array_t(T val) {
		if (val == 0) memset(mem, 0, length * sizeof(T));
		else for (size_t i = 0; i < length; ++i) mem[i] = val;
	}
	array_t(const array_t&) = delete;
	array_t(array_t&&) = default;
	void operator=(const array_t&) = delete;
	array_t& operator=(array_t&&) = default;

	size_t size() const {return length;}

	T& operator[](size_t pos) {return mem[pos];}
	const T& operator[](size_t pos) const {return mem[pos];}

	T& operator[](_LAST) {return mem[length - 1];}
	const T& operator[](_LAST) const {return mem[length - 1];}

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

#endif // CIGMAR_ARRAY_T
