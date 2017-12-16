//
// Created by notoraptor on 17-11-23.
//

#ifndef SRC_CIGMAR_PROPERTYTYPE_HPP
#define SRC_CIGMAR_PROPERTYTYPE_HPP

#include <cigmar/classes/String.hpp>

namespace cigmar::video::manager {
	struct PropertyType {
		int64_t property_type_id;
		String property_type_name;
	};
}

#endif //SRC_CIGMAR_PROPERTYTYPE_HPP
