#ifndef CIGMAR_ARRAYLIST
#define CIGMAR_ARRAYLIST

#include <iostream>
#include <vector>
#include <initializer_list>
#include <cigmar/symbols.hpp>
#include <cigmar/types/pos_t.hpp>

namespace cigmar {
	template <typename T>
	struct Pointer {
		static_assert(std::is_pointer<T>{}, "Pointer class expects a pointer type.");
		T ptr;
		Pointer() = default;
		Pointer(T pointer): ptr(pointer) {}
		bool operator==(const Pointer& other) const {return ptr == other.ptr;}
		bool operator!=(const Pointer& other) const {return ptr != other.ptr;}
		T operator->() {return ptr;}
		const T operator->() const {return ptr;}
		operator T() {return ptr;}
		operator const T() const {return ptr;}
		explicit operator bool() const {return (bool)ptr;}
	};

	template <typename T>
	inline std::ostream& operator<<(std::ostream& o, const Pointer<T>& pointer) {
		o << pointer.ptr;
		return o;
	}

	// Motion fully-defined.
	template<typename T, typename Type = typename std::conditional<std::is_pointer<T>{}, Pointer<T>, T>::type>
	class ArrayList {
		typedef std::vector<Type> vector_t;
		vector_t vec;
	public:
		typedef typename vector_t::iterator iterator_t;
		typedef typename vector_t::const_iterator const_iterator_t;

		ArrayList(): vec() {}
		explicit ArrayList(size_t n, Type val = Type()): vec(n, val) {}
		template <typename Iterator>
		ArrayList(Iterator firstIncluded, Iterator lastExcluded): vec(firstIncluded, lastExcluded) {}
		ArrayList(std::initializer_list<Type> il): vec(il) {}
		ArrayList(const ArrayList& copied): vec(copied.vec) {}
		ArrayList(ArrayList&&) noexcept = default;

		size_t size() const {return vec.size();}
		size_t capacity() const {return vec.capacity();}

		ArrayList& add(const Type& val) {
			vec.push_back(val);
			return *this;
		}
		ArrayList& add(Type&& val) {
			vec.push_back(std::move(val));
			return *this;
		}
		ArrayList& insert(size_t pos, const Type& val) {
			vec.insert(vec.begin() + pos, val);
			return *this;
		}
		ArrayList& insert(size_t pos, size_t n, const Type& val) {
			vec.insert(vec.begin() + pos, n, val);
			return *this;
		}
		ArrayList& resize(size_t n, Type val = Type()) {
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
		pos_t remove(const Type& val) {
			pos_t position = indexOf(val);
			if (position) remove((size_t)position);
			return position;
		}
		ArrayList& swap(ArrayList& other) {
			vec.swap(other.vec);
			return *this;
		}
		ArrayList& moveUp(size_t pos, size_t offset) {
			if (offset > pos) offset = pos;
			size_t newPos = pos - offset;
			Type value = vec[pos];
			for (size_t i = pos; i > newPos; --i)
				vec[i] = vec[i - 1];
			vec[newPos] = value;
			return *this;
		}
		ArrayList& moveDown(size_t pos, size_t offset) {
			size_t maxOffset = vec.size() - 1 - pos;
			if (offset > maxOffset) offset = maxOffset;
			size_t newPos = pos + offset;
			Type value = vec[pos];
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
			vector_t().swap(vec);
			return *this;
		}

		ArrayList& reserve(size_t n) {vec.reserve(n); return *this;}

		pos_t indexOf(const Type& val) const {
			size_t i;
			for (i = 0; i < vec.size(); ++i) {
				if (vec[i] == val) {
					break;
				}
			}
			return pos_t(i != vec.size(), i);
		}
		bool contains(const Type& val) const {
			for (const Type& v: vec) if (v == val) return true;
			return false;
		}

		void operator=(const ArrayList&) = delete;
		ArrayList& operator=(ArrayList&&) noexcept = default;

		Type& operator[](size_t pos) {return vec[pos];}
		const Type& operator[](size_t pos) const {return vec[pos];}

		Type& operator[](last_t) {return vec.back();}
		const Type& operator[](last_t) const {return vec.back();}

		explicit operator Type*() {return vec.data();}
		explicit operator const Type*() const {return vec.data();}
		explicit operator bool() const {return (bool)vec.size();}

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
	};

	template <typename C, typename T>
	std::basic_ostream<C>& operator<<(std::basic_ostream<C>& o, const ArrayList<T>& vec) {
		o << '[';
		if (vec.size()) {
			o << streamer(vec[0]);
			for (size_t i = 1; i < vec.size(); ++i)
				o << "; " << streamer(vec[i]);
		}
		o << ']';
		return o;
	};
}

#endif // CIGMAR_ARRAYLIST
