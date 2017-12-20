#ifndef CIGMAR_HASHSET
#define CIGMAR_HASHSET

#include <unordered_set>
#include <initializer_list>
#include <cigmar/interfaces/Streamable.hpp>
#include <cigmar/utils/Hasher.hpp>
#include <cigmar/interfaces/Collection.hpp>

namespace cigmar {

template<typename T>
class HashSet: public Streamable, public Collection<std::unordered_set<T, Hasher>> {
public:
	typedef T dtype;
public:
	typedef std::unordered_set<T, Hasher> set_type;
	typedef typename set_type::iterator iterator_t;
	typedef typename set_type::const_iterator const_iterator_t;
private:
	set_type s;
public:
	HashSet() noexcept : s() {}
	template<typename E>
	HashSet(const Collection<E>& container): s(container.begin(), container.end()) {}
	HashSet(std::initializer_list<T> il): s(il) {}
	HashSet(const HashSet& other): s(other.s) {}
	HashSet(HashSet&&) noexcept = default;

	size_t size() const {return s.size();}
	bool contains(const T& val) const {return s.count(val) != 0;}
	iterator_t add(const T& val) {
		auto details = s.insert(val);
		return std::get<0>(details);
	}
	iterator_t add(T&& val) {
		std::pair<iterator_t, bool> details = s.insert(std::move(val));
		return std::get<0>(details);
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
			o << Streamer<T>(*it);
			++it;
			while(it != s.end()) {
				o << "; " << Streamer<T>(*it);
				++it;
			}
		}
		o << '}';
	}
	iterator_t begin() {return s.begin();}
	iterator_t end() {return s.end();}
	const_iterator_t begin() const {return s.begin();}
	const_iterator_t end() const {return s.end();}
	iterator_t iterator(const T& val) {return s.find(val);}
	const_iterator_t iterator(const T& val) const {return s.find(val);}

	HashSet& operator=(HashSet&&) noexcept = default;
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
