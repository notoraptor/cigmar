//
// Created by notoraptor on 17-11-23.
//

#ifndef SRC_CIGMAR_REPORT_HPP
#define SRC_CIGMAR_REPORT_HPP


namespace cigmar::video::manager {
	struct Report {
		size_t total = 0;
		size_t added = 0;
		size_t removed = 0;
	};
	template <typename C>
	std::basic_ostream<C>& operator<<(std::basic_ostream<C>& o, const Report& r) {
		o << "Report(total: " << r.total << ", added: " << r.added << ", removed: " << r.removed << ")";
		return o;
	}
}

#endif //SRC_CIGMAR_REPORT_HPP
