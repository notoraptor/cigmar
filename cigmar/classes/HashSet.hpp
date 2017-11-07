#ifndef CIGMAR_HASHSET
#define CIGMAR_HASHSET

#include <unordered_set>
#include <initializer_list>
#include <cigmar/interfaces/Streamable.hpp>
#include <cigmar/classes/Hasher.hpp>

namespace cigmar {

template<typename T>
class HashSet: public Streamable {
public:
	typedef T dtype;
public:
	typedef std::unordered_set<T, Hasher> set_type;
	typedef typename set_type::iterator iterator_t;
	typedef typename set_type::const_iterator const_iterator_t;
private:
	set_type s;
public:
	HashSet(): s() {}
	template<typename C>
	HashSet(C& container): s(container.begin(), container.end()) {}
	HashSet(std::initializer_list<T> il): s(il) {}
	HashSet(const HashSet& other): s(other.s) {}
	HashSet(HashSet&&) = default;

	size_t size() const {return s.size();}
	bool contains(const T& val) const {return s.count(val) != 0;}
	HashSet& add(const T& val) {
		s.insert(val);
		return *this;
	}
	HashSet& remove(const T& val) {
		s.erase(val);
		return *this;
	}
	HashSet& swap(HashSet& other) {
		s.swap(other.s);
		return *this;
	}
	HashSet& clear() {
		s.clear();
		return *this;
	}
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
	iterator_t begin() {return s.begin();}
	iterator_t end() {return s.end();}
	const_iterator_t begin() const {return s.begin();}
	const_iterator_t end() const {return s.end();}

	HashSet& operator=(HashSet&&) = default;
	HashSet& operator=(std::initializer_list<T> il) {
		s = il;
		return *this;
	}
	explicit operator bool() const {return !s.empty();}
	HashSet& operator<<(const T& val) {
		s.insert(val);
		return *this;
	}
};

}

#endif // CIGMAR_HASHSET
