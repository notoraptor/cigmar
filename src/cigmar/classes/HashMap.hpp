#ifndef CIGMAR_HASHMAP
#define CIGMAR_HASHMAP

#include <initializer_list>
#include <unordered_map>
#include <cigmar/internal/Hasher.hpp>

namespace cigmar {
	template<typename K, typename V>
	class HashMap {
		template<typename Iterator, typename Key>
		class key_iterator {
			Iterator it;
		public:
			key_iterator(Iterator iterator): it(iterator) {}
			key_iterator& operator++() {++it; return *this;}
			bool operator==(const key_iterator& other) const {return it == other.it;}
			bool operator!=(const key_iterator& other) const {return it != other.it;}
			Key& operator*() {
				return it->first;
			}
		};
		template<typename Iterator, typename Value>
		class value_iterator {
			Iterator it;
		public:
			value_iterator(Iterator iterator): it(iterator) {}
			value_iterator& operator++() {++it; return *this;}
			bool operator==(const value_iterator& other) const {return it == other.it;}
			bool operator!=(const value_iterator& other) const {return it != other.it;}
			Value& operator*() {
				return it->second;
			}
		};
	public:
		typedef std::unordered_map<K, V, Hasher> map_type;
		typedef typename map_type::iterator iterator_t;
		typedef typename map_type::const_iterator const_iterator_t;
		typedef typename map_type::value_type pair_type;
		/// iterator->first, iterator->second
		typedef value_iterator<iterator_t, V> value_iterator_t;
		typedef value_iterator<const_iterator_t, const V> const_value_iterator_t;
		typedef key_iterator<iterator_t, K> key_iterator_t;
		typedef key_iterator<const_iterator_t, K> const_key_iterator_t;
	private:
		map_type m;
	public:
		HashMap(): m() {}
		HashMap(std::initializer_list<pair_type> il): m(il) {}
		HashMap(const HashMap& other): m(other.m) {}
		HashMap(HashMap&&) = default;

		size_t size() const {return m.size();}
		V& get(const K& key) {return m.at(key);}
		const V& get(const K& key) const {return m.at(key);}
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

		iterator_t begin() {return m.begin();}
		iterator_t end() {return m.end();}
		const_iterator_t begin() const {return m.begin();}
		const_iterator_t end() const {return m.end();}
		iterator_t iterator(const K& key) {return m.find(key);}
		const_iterator_t iterator(const K& key) const {return m.find(key);}
		key_iterator_t begin_keys() {return key_iterator_t(m.begin());}
		key_iterator_t end_keys() {return key_iterator_t(m.end());}
		const_key_iterator_t begin_keys() const {return const_key_iterator_t(m.begin());}
		const_key_iterator_t end_keys() const {return const_key_iterator_t(m.end());}
		value_iterator_t begin_values() {return value_iterator_t(m.begin());}
		value_iterator_t end_values() {return value_iterator_t(m.end());}
		const_value_iterator_t begin_values() const {return const_value_iterator_t(m.begin());}
		const_value_iterator_t end_values() const {return const_value_iterator_t(m.end());}

		HashMap& operator=(HashMap&&) noexcept = default;
		HashMap& operator=(std::initializer_list<pair_type> il) {
			m = il;
			return *this;
		}
		explicit operator bool() const {return !m.empty();}
		V& operator[](const K& key) {return m[key];}
		V& operator[](K&& key) {return m[std::move(key)];}
	};

	template <typename C, typename K, typename V>
	std::basic_ostream<C>& operator<<(std::basic_ostream<C>& o, const HashMap<K, V>& m) {
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

#endif // CIGMAR_HASHMAP
