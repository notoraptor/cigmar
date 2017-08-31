#ifndef CIGMAR_ARRAY_T
#define CIGMAR_ARRAY_T

// Motion fully-defined.

template<typename T, size_t N>
class array_t {
private:
	T mem[N];
public:
	const size_t length = N;
public:
	array_t() {}
	array_t(const array_t&) = delete;
	array_t(array_t&&) = default;
	void operator=(const array_t&) = delete;
	array_t& operator=(array_t&&) = default;

	T& operator[](size_t pos) {return mem[pos];}
	const T& operator[](size_t pos) const {return mem[pos];}

	T& operator[](_LAST) {return mem[length - 1];}
	const T& operator[](_LAST) const {return mem[length - 1];}

	operator T*() {return mem;}
	operator const T*() const {return mem;}

	T* begin() {return mem;}
	T* end() {return mem + length;}
	const T* cbegin() const {return mem;}
	const T* cend() const {return mem + length;}
};

#endif // CIGMAR_ARRAY_T
