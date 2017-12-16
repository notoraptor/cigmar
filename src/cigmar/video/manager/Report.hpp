//
// Created by notoraptor on 17-11-23.
//

#ifndef SRC_CIGMAR_REPORT_HPP
#define SRC_CIGMAR_REPORT_HPP

#include <cigmar/interfaces/Streamable.hpp>

namespace cigmar::video::manager {
	struct Report: public Streamable {
		size_t total = 0;
		size_t added = 0;
		size_t removed = 0;
		void toStream(std::ostream& o) const override {
			o << "Report(total: " << total << ", added: " << added << ", removed: " << removed << ")";
		}
	};
}

#endif //SRC_CIGMAR_REPORT_HPP
