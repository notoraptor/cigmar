#ifndef CIGMAR_ARRAYLIST
#define CIGMAR_ARRAYLIST

#include <iostream>
#include <vector>
#include <initializer_list>
#include <cigmar/symbols.hpp>
#include <cigmar/interfaces/Streamable.hpp>
#include <cigmar/interfaces/Collection.hpp>
#include <cigmar/types/pos_t.hpp>

namespace cigmar {

// Motion fully-defined.

template<typename T>
class ArrayList: public Streamable, public Collection<std::vector<T>> {
	std::vector<T> vec;
	typedef typename std::add_const<
		typename std::conditional<
			std::is_pointer<T>::value,
			typename std::remove_pointer<T>::type,
			T
		>::type
	>::type const_val_type;
public:
	typedef typename std::conditional<
		std::is_pointer<T>::value,
		typename std::add_pointer<const_val_type>::type,
		const_val_type
	>::type const_t;
	typedef typename std::vector<T>::iterator iterator_t;
	typedef typename std::vector<T>::const_iterator const_iterator_t;

	ArrayList(): vec() {}
	explicit ArrayList(size_t n, T val = T()): vec(n, val) {}
	template<typename E>
	ArrayList(const Collection<E>& arr): vec(arr.begin(), arr.end()) {}
	ArrayList(std::initializer_list<T> il): vec(il) {}
	ArrayList(const ArrayList& copied): vec(copied.vec) {}
	ArrayList(ArrayList&&) noexcept = default;

	size_t size() const {return vec.size();}
	size_t capacity() const {return vec.capacity();}

	ArrayList& add(const_t& val) {
		vec.push_back(val);
		return *this;
	}
	ArrayList& add(T&& val) {
		vec.push_back(std::move(val));
		return *this;
	}
	ArrayList& insert(size_t pos, const_t & val) {
		vec.insert(vec.begin() + pos, val);
		return *this;
	}
	ArrayList& insert(size_t pos, size_t n, const_t& val) {
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
	bool remove(const_t& val) {
		pos_t position = indexOf(val);
		if (position) remove((size_t)position);
		return (bool)position;
	}
	ArrayList& swap(ArrayList& other) {
		vec.swap(other.vec);
		return *this;
	}
	ArrayList& moveUp(size_t pos, size_t offset) {
		if (offset > pos) offset = pos;
		size_t newPos = pos - offset;
		T value = vec[pos];
		for (size_t i = pos; i > newPos; --i)
			vec[i] = vec[i - 1];
		vec[newPos] = value;
		return *this;
	}
	ArrayList& moveDown(size_t pos, size_t offset) {
		size_t maxOffset = vec.size() - 1 - pos;
		if (offset > maxOffset) offset = maxOffset;
		size_t newPos = pos + offset;
		T value = vec[pos];
		for (size_t i = pos; i < newPos; ++i)
			vec[i] = vec[i + 1];
		vec[newPos] = value;
		return *this;
	}
	ArrayList& switchPosition(size_t pos1, size_t pos2) {
		std::swap(vec[pos1], vec[pos2]);
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

	pos_t indexOf(const_t& val) const {
		size_t i;
		for (i = 0; i < vec.size(); ++i) {
			if (vec[i] == val) {
				break;
			}
		}
		return pos_t(i != vec.size(), i);
	}
	bool contains(const_t& val) const {
		for (const_t& v: vec) if (v == val) return true;
		return false;
	}

	void operator=(const ArrayList&) = delete;
	ArrayList& operator=(ArrayList&&) noexcept = default;

	T& operator[](size_t pos) {return vec[pos];}
	const_t& operator[](size_t pos) const {return vec[pos];}

	T& operator[](last_t) {return vec.back();}
	const_t& operator[](last_t) const {return vec.back();}

	explicit operator T*() {return vec.data();}
	explicit operator const_t*() const {return vec.data();}
	explicit operator bool() const {return !vec.empty();}

	bool operator==(const ArrayList& other) const {
		return vec == other.vec;
	}
	bool operator!=(const ArrayList& other) const {
		return vec != other.vec;
	}

	iterator_t begin() {return vec.begin();}
	iterator_t end() {return vec.end();}
	const_iterator_t begin() const {return vec.begin();}
	const_iterator_t end() const {return vec.end();}

	void toStream(std::ostream& o) const override {
		o << '[';
		if (vec.size()) {
			o << Streamer<T>(vec[0]);
			for (size_t i = 1; i < size(); ++i) {
				o << "; " << Streamer<T>(vec[i]);
			}
		}
		o << ']';
	}
};
}

#endif // CIGMAR_ARRAYLIST
