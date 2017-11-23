//
// Created by notoraptor on 17-11-23.
//

#ifndef SRC_CIGMAR_MULTIPLEPROPERTIES_HPP
#define SRC_CIGMAR_MULTIPLEPROPERTIES_HPP

#include <video/manager/RowMapping.hpp>
#include <video/manager/MultipleProperty.hpp>
#include <video/manager/PropertyTypes.hpp>
#include <cigmar/classes/Exception.hpp>

namespace cigmar::video::manager {
	class MultipleProperties {
		RowMapping<MultipleProperty> properties;
		sqlite::Database* db = nullptr;
		PropertyTypes* types = nullptr;
		void add(const String& multiple_property_name, const PropertyType& property_type) {
			int countUniqueProperties = 0;
			auto query = db->query(
				"SELECT COUNT(unique_property_id) FROM unique_property WHERE unique_property_name = ?",
				multiple_property_name);
			while (query) {
				countUniqueProperties = query.getInt(0);
				++query;
			}
			if (countUniqueProperties)
				throw Exception("Cannot create a (multiple) property with name \"",
								multiple_property_name, "\": an unique property already has this name.");
			db->run(
				"INSERT INTO multiple_property(multiple_property_name, property_type_id) VALUES(?, ?)",
				multiple_property_name, property_type.property_type_id);
			properties.add(MultipleProperty(db->lastId(), multiple_property_name, property_type));
		};
	public:
		void init(sqlite::Database& database, PropertyTypes& propertyTypes) {
			db = &database;
			types = &propertyTypes;
			auto query = db->query(
				"SELECT multiple_property_id, multiple_property_name, property_type_id FROM multiple_property");
			while (query) {
				properties.add(MultipleProperty(query.getInt64(0), query.getText(1), types->get(query.getInt64(2))));
				++query;
			}
		}
		void addInteger(const String& multiple_property_name) {
			add(multiple_property_name, types->integer());
		};
		void addUnsigned(const String& multiple_property_name) {
			add(multiple_property_name, types->unsignedInteger());
		};
		void addDouble(const String& multiple_property_name) {
			add(multiple_property_name, types->floating());
		};
		void addString(const String& multiple_property_name) {
			add(multiple_property_name, types->floating());
		};
		void remove(const String& multiple_property_name) {
			if (properties.contains(multiple_property_name)) {
				db->run("DELETE FROM multiple_property WHERE multiple_property_name = ?", multiple_property_name);
				properties.remove(multiple_property_name);
			}
		};
		void remove(int64_t multiple_property_id) {
			if (properties.contains(multiple_property_id)) {
				db->run("DELETE FROM multiple_property WHERE multiple_property_id = ?", multiple_property_id);
				properties.remove(multiple_property_id);
			}
		};
		const MultipleProperty* get(const String& multiple_property_name) const {
			return &properties.get(multiple_property_name);
		};
		const MultipleProperty* get(int64_t multiple_property_id) const {
			return &properties.get(multiple_property_id);
		};
	};
}

#endif //SRC_CIGMAR_MULTIPLEPROPERTIES_HPP
