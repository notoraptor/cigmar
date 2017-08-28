#ifndef CIGMAR_PRIMITIVE_CLASSES
#define CIGMAR_PRIMITIVE_CLASSES

#include <functional>
#include <vector>
#include <set>

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

template<typename T>
class ArrayList {
private:
	std::vector<T> vec;
public:
	// NB: "typename" here is mandatory.
	typedef typename std::vector<T>::iterator iterator_t;
	typedef typename std::vector<T>::const_iterator const_iterator_t;

	ArrayList(): vec() {}
	ArrayList(const ArrayList& copied): vec(copied.vec) {}
	ArrayList(size_t n, T val = T()): vec(n, val) {}
	template<size_t N> ArrayList(const array_t<T, N>& arr): vec(arr.cbegin(), arr.cend()) {}
	ArrayList(ArrayList&& moved) = default;

	ArrayList& add(const T& val) {
		vec.push_back(val);
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
	operator bool() const {return vec.empty();}

	size_t capacity() const {return vec.capacity();}
	ArrayList& reserve(size_t n) {vec.reserve(n); return *this;}

	T& operator[](size_t pos) {return vec[pos];}
	const T& operator[](size_t pos) const {return vec[pos];}

	T& operator[](_LAST) {return vec.back();}
	const T& operator[](_LAST) const {return vec.back();}

	operator T*() {return vec.data();}
	operator const T*() const {return vec.data();}

	iterator_t begin() {return vec.begin();}
	iterator_t end() {return vec.end();}
	const_iterator_t cbegin() const {return vec.cbegin();}
	const_iterator_t cend() const {return vec.cend();}
};

template<typename T>
class TreeSet {
public:
	typedef std::function<bool(const T&, const T&)> less_type;
	typedef std::set<T, less_type> set_type;
	typedef typename std::set<T, less_type>::iterator iterator_t;
	typedef typename std::set<T, less_type>::const_iterator const_iterator_t;
private:
	set_type s;
	less_type comparator;
	static bool less_than(const T& a, const T& b) {return a < b;}
public:
	TreeSet(less_type c = less_than): s(c) {}
	TreeSet(const ArrayList<T>& arr, less_type c = less_than): s(arr.cbegin(), arr.cend(), c) {}
	template<size_t N>
	TreeSet(const array_t<T, N>& arr, less_type c = less_than): s(arr.cbegin(), arr.cend(), c) {}
	TreeSet(const TreeSet& other): s(other.s) {}
	TreeSet(TreeSet&&) = default;

	operator bool() const {return s.empty();}
	size_t size() const {return s.size();}

	bool contains(const T& val) const {
		return s.find(val) != s.end();
	}

	TreeSet<T>& add(const T& val) {
		s.insert(val);
		return *this;
	}
	TreeSet<T>& remove(const T& val) {
		s.erase(val);
		return *this;
	}
	TreeSet<T>& swap(TreeSet<T>& other) {
		s.swap(other.s);
		return *this;
	}
	TreeSet<T>& clear() {
		s.clear();
		return *this;
	}

	iterator_t begin() {return s.begin();}
	iterator_t end() {return s.end();}
	const_iterator_t cbegin() const {return s.cbegin();}
	const_iterator_t cend() const {return s.cend();}
};

// class AbstractList;
// class AbstractMap;
// class AbstractSet;
class LinkedList; // Double linked list.
class Stack;
class Queue;
class HashMap;
class TreeMap;
class HashSet;

class Tensor; // Sophisticated N-D array.

#endif // CIGMAR_PRIMITIVE_CLASSES
