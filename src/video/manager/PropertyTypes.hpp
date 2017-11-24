//
// Created by notoraptor on 17-11-23.
//

#ifndef SRC_CIGMAR_PROPERTYTYPES_HPP
#define SRC_CIGMAR_PROPERTYTYPES_HPP

#include <video/manager/PropertyType.hpp>
#include <cigmar/classes/HashMap.hpp>
#include <cigmar/sqlite.hpp>
#include <cassert>

namespace cigmar::video::manager {
	class PropertyTypes {
		PropertyType bool_type;
		PropertyType int_type;
		PropertyType uint_type;
		PropertyType double_type;
		PropertyType string_type;
		HashMap<int64_t, PropertyType*> index;
	public:
		void init(sqlite::Database& db) {
			HashMap<String, int64_t> mapping;
			auto query = db.query("SELECT property_type_id, property_type_name FROM property_type");
			size_t count = 0;
			while (query) {
				int64_t id = query.getInt64(0);
				String name = query.getText(1);
				mapping[name] = id;
				++query;
				++count;
			}
			assert(count == 5);

			auto it = mapping.iterator("int");
			int_type = {it->second, it->first};
			index[int_type.property_type_id] = &int_type;

			it = mapping.iterator("uint");
			uint_type = {it->second, it->first};
			index[uint_type.property_type_id] = &uint_type;

			it = mapping.iterator("double");
			double_type = {it->second, it->first};
			index[double_type.property_type_id] = &double_type;

			it = mapping.iterator("string");
			string_type = {it->second, it->first};
			index[string_type.property_type_id] = &string_type;

			it = mapping.iterator("bool");
			bool_type = {it->second, it->first};
			index[bool_type.property_type_id] = &bool_type;
		}
		const PropertyType& get(int64_t id) const {
			return *index.get(id);
		}
		const PropertyType& integer() const {return int_type;}
		const PropertyType& unsignedInteger() const {return uint_type;}
		const PropertyType& floating() const {return double_type;}
		const PropertyType& text() const {return string_type;}
		const PropertyType& boolean() const {return bool_type;}
	};
}

#endif //SRC_CIGMAR_PROPERTYTYPES_HPP
