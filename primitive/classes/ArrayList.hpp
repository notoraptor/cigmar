#ifndef CIGMAR_ARRAYLIST
#define CIGMAR_ARRAYLIST

#include <iostream>
#include <vector>

// Motion fully-defined.

template<typename T>
class ArrayList {
private:
	std::vector<T> vec;
public:
	// NB: "typename" here is mandatory.
	typedef typename std::vector<T>::iterator iterator_t;
	typedef typename std::vector<T>::const_iterator const_iterator_t;

	ArrayList(): vec() {}
	ArrayList(size_t n, T val = T()): vec(n, val) {}
	template<size_t N> ArrayList(const array_t<T, N>& arr): vec(arr.cbegin(), arr.cend()) {}
	ArrayList(const ArrayList& copied): vec(copied.vec) {}
	ArrayList(ArrayList&& moved) = default;

	void operator=(const ArrayList&) = delete;
	ArrayList& operator=(ArrayList&) = default;
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
	ArrayList& purge() {
		std::vector<T>().swap(vec);
		return *this;
	}

	size_t size() const {return vec.size();}
	explicit operator bool() const {return vec.empty();}

	size_t capacity() const {return vec.capacity();}
	ArrayList& reserve(size_t n) {vec.reserve(n); return *this;}

	T& operator[](size_t pos) {return vec[pos];}
	const T& operator[](size_t pos) const {return vec[pos];}

	T& operator[](_LAST) {return vec.back();}
	const T& operator[](_LAST) const {return vec.back();}

	explicit operator T*() {return vec.data();}
	explicit operator const T*() const {return vec.data();}

	iterator_t begin() {return vec.begin();}
	iterator_t end() {return vec.end();}
	const_iterator_t cbegin() const {return vec.cbegin();}
	const_iterator_t cend() const {return vec.cend();}
};

#endif // CIGMAR_ARRAYLIST
