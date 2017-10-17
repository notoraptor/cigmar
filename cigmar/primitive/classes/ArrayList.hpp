#ifndef CIGMAR_ARRAYLIST
#define CIGMAR_ARRAYLIST

#include <iostream>
#include <vector>
#include <initializer_list>
#include <cigmar/primitive/symbols.hpp>
#include <cigmar/primitive/utils.hpp>
#include <cigmar/primitive/interfaces/Streamable.hpp>

namespace cigmar {

// Motion fully-defined.

template<typename T>
class ArrayList: public Streamable {
private:
	std::vector<T> vec;
public:
	typedef T dtype;
	// NB: "typename" here is mandatory.
	typedef typename std::vector<T>::iterator iterator_t;
	typedef typename std::vector<T>::const_iterator const_iterator_t;

	ArrayList(): vec() {}
	ArrayList(size_t n, T val = T()): vec(n, val) {}
	template<typename C>
	ArrayList(const C& arr): vec() {
		static_assert(is_iterable<C>::value, "Iterable type required to instantiate an ArrayList.");
		vec.assign(arr.begin(), arr.end());
	}
	ArrayList(std::initializer_list<T> il): vec(il) {}
	ArrayList(const ArrayList& copied): vec(copied.vec) {}
	ArrayList(ArrayList&&) = default;

	size_t size() const {return vec.size();}
	size_t capacity() const {return vec.capacity();}

	ArrayList& add(const T& val) {
		vec.push_back(val);
		return *this;
	}
	ArrayList& add(T&& val) {
		vec.push_back(std::move(val));
		return *this;
	}
	ArrayList& insert(size_t pos, const T& val) {
		vec.insert(vec.begin() + pos, val);
		return *this;
	}
	ArrayList& insert(size_t pos, size_t n, const T& val) {
		vec.insert(vec.begin() + pos, n, val);
		return *this;
	}
	ArrayList& resize(size_t n, T val = T()) {
		vec.resize(n, val);
		return *this;
	}
	ArrayList& remove(size_t pos) {
		vec.erase(vec.begin() + pos);
		return *this;
	}
	ArrayList& remove(size_t from, size_t to_excluded) {
		vec.erase(vec.begin() + from, vec.begin() + to_excluded);
		return *this;
	}
	ArrayList& swap(ArrayList& other) {
		vec.swap(other.vec);
		return *this;
	}
	ArrayList& clear() {
		vec.clear();
		return *this;
	}
	ArrayList& free() {
		std::vector<T>().swap(vec);
		return *this;
	}

	ArrayList& reserve(size_t n) {vec.reserve(n); return *this;}

	void operator=(const ArrayList&) = delete;
	ArrayList& operator=(ArrayList&&) = default;

	T& operator[](size_t pos) {return vec[pos];}
	const T& operator[](size_t pos) const {return vec[pos];}

	T& operator[](_LAST) {return vec.back();}
	const T& operator[](_LAST) const {return vec.back();}

	explicit operator T*() {return vec.data();}
	explicit operator const T*() const {return vec.data();}
	explicit operator bool() const {return vec.empty();}

	iterator_t begin() {return vec.begin();}
	iterator_t end() {return vec.end();}
	const_iterator_t begin() const {return vec.begin();}
	const_iterator_t end() const {return vec.end();}

	void toStream(std::ostream& o) const override {
		o << '[';
		if (vec.size()) {
			o << vec[0];
			for (size_t i = 1; i < size(); ++i)
				o << "; " << vec[i];
		}
		o << ']';
	}
};

}

#endif // CIGMAR_ARRAYLIST
