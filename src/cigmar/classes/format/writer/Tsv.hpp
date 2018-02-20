//
// Created by notoraptor on 15/11/2017.
//

#ifndef SRC_CIGMAR_TSV_HPP
#define SRC_CIGMAR_TSV_HPP

#include <cigmar/classes/String.hpp>
#include <cigmar/classes/Queue.hpp>
#include <functional>
#include <cigmar/classes/exception/Exception.hpp>

namespace cigmar::format::writer {

	/** Helper class to convert Data objects to TSV format rows. **/
	template<typename Data> struct Tsv {
		typedef std::function<String(const Data&)> getter_t;
		typedef std::pair<String, getter_t> column_t;
		static column_t col(String name, getter_t getter) {
			return column_t(name, getter);
		}
	private:
		Queue<column_t> columns;
	public:
		Tsv(std::initializer_list<column_t> il): columns(il) {
			if (!columns)
				throw Exception("This TSV format hss no columns.");
		}
		String getHeader() const {
			auto it = columns.begin(), end = columns.end();
			String out = std::get<0>(*it);
			while (++it != end)
				out << '\t' << std::get<0>(*it);
			return out;
		}
		String getRow(const Data& data) const {
			auto it = columns.begin(), end = columns.end();
			String out = std::get<1>(*it)(data);
			while (++it != end)
				out << '\t' << std::get<1>(*it)(data);
			return out;
		}
	};
}

#endif //SRC_CIGMAR_TSV_HPP
