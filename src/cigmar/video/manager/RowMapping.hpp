//
// Created by notoraptor on 17-11-23.
//

#ifndef SRC_CIGMAR_ROWMAPPING_HPP
#define SRC_CIGMAR_ROWMAPPING_HPP

#include <cigmar/classes/HashSet.hpp>
#include <cigmar/video/manager/TableRow.hpp>
#include <cigmar/classes/HashMap.hpp>

namespace cigmar::video::manager {
	template<typename T>
	class RowMapping {
		static_assert(std::is_base_of<TableRow, T>{}, "We can only map a TableRow (or derived class) instance.");
	private:
		typedef HashMap<int64_t, T> store_type;
		store_type fromIndices;
		HashMap<String, T*> fromKeys;
	public:
		typedef typename store_type::value_iterator_t iterator_t;
		typedef typename store_type::const_value_iterator_t const_iterator_t;
		size_t size() const {return fromIndices.size();}
		bool contains(const T& value) const {return fromIndices.contains(value.id());}
		bool contains(int64_t id) const {return fromIndices.contains(id);}
		bool contains(const String& key) const {return fromKeys.contains(key);}
		T& add(const T& value) {
			if (!fromIndices.contains(value.id())) {
				fromIndices.put(value.id(), value);
				T& added = fromIndices.get(value.id());
				fromKeys.put(added.key(), &added);
			}
			return fromIndices.get(value.id());
		}
		T& add(T&& value) {
			if (!fromIndices.contains(value.id())) {
				int64_t id = value.id();
				fromIndices.put(id, std::move(value));
				T& added = fromIndices.get(id);
				fromKeys.put(added.key(), &added);
			}
			return fromIndices.get(value.id());
		}
		T& get(int64_t id) {return fromIndices.get(id);}
		T& get(const String& key) {return *fromKeys.get(key);}
		const T& get(int64_t id) const {return fromIndices.get(id);}
		const T& get(const String& key) const {return *fromKeys.get(key);}
		void remove(const String& key) {
			if (fromKeys.contains(key)) {
				T* ptr = fromKeys.get(key);
				fromKeys.remove(key);
				fromIndices.remove(ptr->id());
			}
		}
		void remove(int64_t id) {
			if (fromIndices.contains(id)) {
				T* ptr = &fromIndices.get(id);
				fromKeys.remove(ptr->key());
				fromIndices.remove(id);
			}
		}
		iterator_t begin() {return fromIndices.begin_values();}
		iterator_t end() {return fromIndices.end_values();}
		const_iterator_t begin() const {return fromIndices.begin_values();}
		const_iterator_t end() const {return fromIndices.end_values();}
	};
}

#endif //SRC_CIGMAR_ROWMAPPING_HPP
