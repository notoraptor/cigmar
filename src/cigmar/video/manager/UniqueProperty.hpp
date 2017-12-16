//
// Created by notoraptor on 17-11-23.
//

#ifndef SRC_CIGMAR_UNIQUEPROPERTY_HPP
#define SRC_CIGMAR_UNIQUEPROPERTY_HPP

#include <cigmar/video/manager/TableRow.hpp>
#include <cigmar/video/manager/PropertyType.hpp>

namespace cigmar::video::manager {
	struct UniqueProperty: public TableRow {
		int64_t unique_property_id;
		String unique_property_name;
		String default_value;
		const PropertyType& property_type;
		UniqueProperty(int64_t propertyId, const String& propertyName,
					   const String& defaultValue, const PropertyType& type):
			unique_property_id(propertyId), unique_property_name(propertyName),
			default_value(defaultValue), property_type(type) {
			setId(unique_property_id);
			setKey(unique_property_name);
		}
		bool operator==(const UniqueProperty& other) const {
			return unique_property_id == other.unique_property_id;
		}
	};
}

#endif //SRC_CIGMAR_UNIQUEPROPERTY_HPP
