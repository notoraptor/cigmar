#ifndef CIGMAR_TREESET
#define CIGMAR_TREESET

#include <functional>
#include <set>

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

#endif // CIGMAR_TREESET
