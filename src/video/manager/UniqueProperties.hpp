//
// Created by notoraptor on 17-11-23.
//

#ifndef SRC_CIGMAR_UNIQUEPROPERTIES_HPP
#define SRC_CIGMAR_UNIQUEPROPERTIES_HPP

#include <cigmar/sqlite.hpp>
#include <video/manager/RowMapping.hpp>
#include <video/manager/UniqueProperty.hpp>
#include <video/manager/PropertyTypes.hpp>
#include <cigmar/classes/Exception.hpp>

namespace cigmar::video::manager {
	class UniqueProperties {
		RowMapping<UniqueProperty> properties;
		sqlite::Database* db = nullptr;
		PropertyTypes* types = nullptr;
		void add(const String& unique_property_name, const String& default_value, const PropertyType& property_type) {
			int countMultipleProperties = 0;
			auto query = db->query(
				"SELECT COUNT(multiple_property_id) FROM multiple_property WHERE multiple_property_name = ?",
				unique_property_name);
			while (query) {
				countMultipleProperties = query.getInt(0);
				++query;
			}
			if (countMultipleProperties)
				throw Exception("Cannot create a (unique) property with name \"",
								unique_property_name, "\": a multiple property already has this name.");
			db->run("INSERT INTO unique_property(unique_property_name, default_value, property_type_id) VALUES(?, ?, ?)",
					unique_property_name, default_value, property_type.property_type_id);
			properties.add(UniqueProperty(db->lastId(), unique_property_name, default_value, property_type));
		};
	public:
		void init(sqlite::Database& database, PropertyTypes& propertyTypes) {
			db = &database;
			types = &propertyTypes;
			auto query = db->query(
				"SELECT unique_property_id, unique_property_name, default_value, property_type_id FROM unique_property");
			while (query) {
				properties.add(UniqueProperty(query.getInt64(0), query.getText(1),
											  query.isNull(2) ? "" : query.getText(2),
											  types->get(query.getInt64(3))));
				++query;
			}
		}
		bool contains(const String& name) const {
			return properties.contains(name);
		}
		void addInteger(const String& unique_property_name, const String& default_value = "") {
			add(unique_property_name, default_value, types->integer());
		};
		void addUnsigned(const String& unique_property_name, const String& default_value = "") {
			add(unique_property_name, default_value, types->unsignedInteger());
		};
		void addDouble(const String& unique_property_name, const String& default_value = "") {
			add(unique_property_name, default_value, types->floating());
		};
		void addString(const String& unique_property_name, const String& default_value = "") {
			add(unique_property_name, default_value, types->text());
		};
		void addBool(const String& unique_property_name, bool default_value = false) {
			add(unique_property_name, default_value ? "1" : "0", types->text());
		}
		void remove(const String& unique_property_name) {
			if (properties.contains(unique_property_name)) {
				db->run("DELETE FROM unique_property WHERE unique_property_name = ?", unique_property_name);
				properties.remove(unique_property_name);
			}
		};
		void remove(int64_t unique_property_id) {
			if (properties.contains(unique_property_id)) {
				db->run("DELETE FROM unique_property WHERE unique_property_id = ?", unique_property_id);
				properties.remove(unique_property_id);
			}
		};
		const UniqueProperty* get(const String& unique_property_name) const {
			return &properties.get(unique_property_name);
		};
		const UniqueProperty* get(int64_t unique_property_id) const {
			return &properties.get(unique_property_id);
		};
	};
}

#endif //SRC_CIGMAR_UNIQUEPROPERTIES_HPP
