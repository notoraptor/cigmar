#ifndef CIGMAR_TREEMAP
#define CIGMAR_TREEMAP

#include <functional>
#include <map>
#include <initializer_list>

namespace cigmar {
	template<typename K, typename V>
	class TreeMap {
	public:
		typedef std::function<bool(const K&, const K&)> less_type;
		typedef std::map<K, V, less_type> map_type;
		typedef typename std::map<K, V, less_type>::iterator iterator_t;
		typedef typename std::map<K, V, less_type>::const_iterator const_iterator_t;
		typedef typename std::map<K, V, less_type>::value_type pair_type;
		/// K& iterator->first, V& iterator->second
	private:
		map_type m;
		static bool less_than(const K& a, const K& b) {return a < b;}
	public:
		TreeMap(less_type c = less_than): m(c) {}
		TreeMap(std::initializer_list<pair_type> il, less_type c = less_than): m(il, c) {}
		TreeMap(const TreeMap& other): m(other.m) {}
		TreeMap(TreeMap&&) noexcept = default;

		size_t size() const {return m.size();}
		bool contains(const K& key) const {return m.count(key) != 0;}
		TreeMap& put(const K& key, const V& val) {
			/// NB: Added only if key not already in map.
			m.insert(pair_type(key, val));
			return *this;
		}
		V& get(const K& key) {return m.at(key);}
		V& get(const K& key) const {return m.at(key);}
		TreeMap& remove(const K& key) {
			m.erase(key);
			return *this;
		}
		TreeMap& swap(TreeMap& other) {
			m.swap(other.m);
			return *this;
		}
		TreeMap& clear() {
			m.clear();
			return *this;
		}

		iterator_t begin() {return m.begin();}
		iterator_t end() {return m.end();}
		const_iterator_t begin() const {return m.begin();}
		const_iterator_t end() const {return m.end();}

		TreeMap& operator=(TreeMap&&) noexcept = default;
		TreeMap& operator=(std::initializer_list<pair_type> il) {
			m = il;
			return *this;
		}
		explicit operator bool() const {return !m.empty();}
		V& operator[](const K& key) {return m[key];}
		V& operator[](K&& key) {return m[std::move(key)];}
	};
	template <typename C, typename K, typename V>
	std::basic_ostream<C>& operator<<(std::basic_ostream<C>& o, const TreeMap<K, V>& m) {
		o << '{';
		if (m.size()) {
			auto it = m.begin();
			o << streamer(it->first) << ':' << streamer(it->second);
			while((++it) != m.end())
				o << "; " << streamer << ':' << streamer(it->second);
		}
		o << '}';
		return o;
	};
}

#endif // CIGMAR_TREEMAP
