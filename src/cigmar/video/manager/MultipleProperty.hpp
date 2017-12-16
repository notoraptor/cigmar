//
// Created by notoraptor on 17-11-23.
//

#ifndef SRC_CIGMAR_MULTIPLEPROPERTY_HPP
#define SRC_CIGMAR_MULTIPLEPROPERTY_HPP

#include <cigmar/video/manager/TableRow.hpp>

namespace cigmar::video::manager {
	struct MultipleProperty: public TableRow {
		int64_t multiple_property_id;
		String multiple_property_name;
		const PropertyType* const property_type;
	public:
		MultipleProperty(int64_t propertyId, const String& propertyName, const PropertyType& type):
			multiple_property_id(propertyId), multiple_property_name(propertyName), property_type(&type) {
			setId(multiple_property_id);
			setKey(multiple_property_name);
		}
		bool operator==(const MultipleProperty& other) const {
			return multiple_property_id == other.multiple_property_id;
		}
	};
}

#endif //SRC_CIGMAR_MULTIPLEPROPERTY_HPP
