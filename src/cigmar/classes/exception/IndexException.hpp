//
// Created by notoraptor on 17-12-20.
//

#ifndef SRC_CIGMAR_INDEXEXCEPTION_HPP
#define SRC_CIGMAR_INDEXEXCEPTION_HPP

#include <cigmar/classes/exception/Exception.hpp>

namespace cigmar {
	struct IndexException: public Exception {
		IndexException(size_t position, size_t size, const String& infos = String()):
			Exception("IndexException:", position, '/', size - 1, (infos ? String::write(" (", infos, ')') : infos)) {}
	};
}

#endif //SRC_CIGMAR_INDEXEXCEPTION_HPP
