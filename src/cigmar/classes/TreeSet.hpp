#ifndef CIGMAR_TREESET
#define CIGMAR_TREESET

#include <functional>
#include <set>
#include <initializer_list>
#include <cigmar/interfaces/Streamable.hpp>
#include <cigmar/interfaces/Collection.hpp>

namespace cigmar {

template<typename T>
class TreeSet:
		public Streamable,
		public Collection<T,
				typename std::set<T, std::function<bool(const T&, const T&)>>::iterator,
				typename std::set<T, std::function<bool(const T&, const T&)>>::const_iterator> {
public:
	typedef T dtype;
public:
	typedef std::function<bool(const T&, const T&)> less_type;
	typedef std::set<T, less_type> set_type;
	typedef typename std::set<T, less_type>::iterator iterator_t;
	typedef typename std::set<T, less_type>::const_iterator const_iterator_t;
private:
	set_type s;
	static bool less_than(const T& a, const T& b) {return a < b;}
public:
	explicit TreeSet(less_type c = less_than): s(c) {}
	template<typename A, typename I, typename C>
	explicit TreeSet(const Collection<A, I, C>& arr, less_type c = less_than): s(arr.begin(), arr.end(), c) {}
	TreeSet(std::initializer_list<T> il, less_type c = less_than): s(il, c) {}
	TreeSet(const TreeSet& other): s(other.s) {}
	TreeSet(TreeSet&&) noexcept = default;

	TreeSet& operator=(TreeSet&&) noexcept = default;
	TreeSet& operator=(std::initializer_list<T> il) {
		s = il;
		return *this;
	}
	explicit operator bool() const {return !s.empty();}
	TreeSet& operator<<(const T& val) {
		s.insert(val);
		return *this;
	}
	size_t size() const {return s.size();}
	bool contains(const T& val) const {
		return s.count(val) != 0;
	}
	TreeSet& add(const T& val) {
		s.insert(val);
		return *this;
	}
	TreeSet& remove(const T& val) {
		s.erase(val);
		return *this;
	}
	TreeSet& swap(TreeSet& other) {
		s.swap(other.s);
		return *this;
	}
	TreeSet& clear() {
		s.clear();
		return *this;
	}

	const T* safeMin() const {return s.empty() ? nullptr: &(*s.begin());}
	const T* safeMax() const {return s.empty() ? nullptr: &(*(--s.end()));}
	const T* safeBefore(const T& val) const {
		auto it = s.lower_bound(val);
		return it == s.begin() ? nullptr : &(*(--it));
	}
	const T* safeAfter(const T& val) const {
		auto it = s.upper_bound(val);
		return it == s.end() ? nullptr : &(*it);
	}

	const T& min() const {return *s.begin();}
	const T& max() const {return *(--s.end());}
	const T& before(const T& val) const {return *(--s.lower_bound(val));}
	const T& after(const T& val) const {return *s.upper_bound(val);}

	iterator_t begin() {return s.begin();}
	iterator_t end() {return s.end();}
	const_iterator_t begin() const {return s.begin();}
	const_iterator_t end() const {return s.end();}

	void toStream(std::ostream& o) const override {
		o << '{';
		if (s.size()) {
			auto it = s.begin();
			o << *it;
			++it;
			while(it != s.end()) {
				o << "; " << *it;
				++it;
			}
		}
		o << '}';
	}
};

}

#endif // CIGMAR_TREESET
