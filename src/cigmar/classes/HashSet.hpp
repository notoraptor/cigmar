#ifndef CIGMAR_HASHSET
#define CIGMAR_HASHSET

#include <unordered_set>
#include <initializer_list>
#include <cigmar/utils/Hasher.hpp>

namespace cigmar {
	template<typename T>
	class HashSet {
	public:
		typedef std::unordered_set<T, Hasher> set_type;
		typedef typename set_type::iterator iterator_t;
		typedef typename set_type::const_iterator const_iterator_t;
	private:
		set_type s;
	public:
		HashSet() noexcept : s() {}
		template <typename Iterator>
		HashSet(Iterator firstIncluded, Iterator lastExcluded): s(firstIncluded, lastExcluded) {}
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

	template <typename C, typename T>
	std::basic_ostream<C>& operator<<(std::basic_ostream<C>& o, const HashSet<T>& s) {
		o << '{';
		if (s.size()) {
			auto it = s.begin();
			o << streamer(*it);
			while((++it) != s.end())
				o << "; " << streamer(*it);
		}
		o << '}';
		return o;
	};
}

#endif // CIGMAR_HASHSET
