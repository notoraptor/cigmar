#ifndef CIGMAR_HASHMAP
#define CIGMAR_HASHMAP

#include <initializer_list>
#include <unordered_map>
#include <cigmar/interfaces/Streamable.hpp>
#include <cigmar/classes/Hasher.hpp>

namespace cigmar {

template<typename K, typename V>
class HashMap: public Streamable {
public:
	typedef std::unordered_map<K, V, Hasher> map_type;
	typedef typename map_type::iterator iterator_t;
	typedef typename map_type::const_iterator const_iterator_t;
	typedef typename map_type::value_type pair_type;
	/// iterator->first, iterator->second
private:
	map_type m;
public:
	HashMap(): m() {}
	HashMap(std::initializer_list<pair_type> il): m(il) {}
	HashMap(const HashMap& other): m(other.m) {}
	HashMap(HashMap&&) = default;

	size_t size() const {return m.size();}
	V& get(const K& key) {return m.at(key);}
	V& get(const K& key) const {return m.at(key);}
	bool contains(const K& key) const {return m.count(key) != 0;}
	HashMap& put(const K& key, const V& val) {
		m.insert(pair_type(key, val));
		return *this;
	}
	HashMap& remove(const K& key) {
		m.erase(key);
		return *this;
	}
	HashMap& swap(HashMap& other) {
		m.swap(other.m);
		return *this;
	}
	HashMap& clear() {
		m.clear();
		return *this;
	}
	void toStream(std::ostream& o) const override {
		o << '{';
		if (m.size()) {
			auto it = m.begin();
			o << it->first << ':' << it->second;
			++it;
			while(it != m.end()) {
				o << "; " << it->first << ':' << it->second;
				++it;
			}
		}
		o << '}';
	}

	iterator_t begin() {return m.begin();}
	iterator_t end() {return m.end();}
	const_iterator_t begin() const {return m.begin();}
	const_iterator_t end() const {return m.end();}

	HashMap& operator=(HashMap&&) = default;
	HashMap& operator=(std::initializer_list<pair_type> il) {
		m = il;
		return *this;
	}
	explicit operator bool() const {return !m.empty();}
	V& operator[](const K& key) {return m[key];}
	V& operator[](K&& key) {return m[std::move(key)];}
};

}

#endif // CIGMAR_HASHMAP
