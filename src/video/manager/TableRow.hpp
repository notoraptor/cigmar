//
// Created by notoraptor on 17-11-23.
//

#ifndef SRC_CIGMAR_TABLEROW_HPP
#define SRC_CIGMAR_TABLEROW_HPP

#include <cigmar/classes/String.hpp>

namespace cigmar::video::manager {
	class TableRow: public Hashable {
	private:
		int64_t m_id;
		String m_key;
	protected:
		void setId(int64_t value) {
			m_id = value;
		}
		void setKey(const String& value) {
			m_key = value;
		}
		void setKey(String&& moved) {
			m_key = std::move(moved);
		}
	public:
		int64_t id() const {return m_id;}
		const String& key() const {return m_key;}
		size_t hash() const override {
			return (size_t)m_id;
		}
	};
}

#endif //SRC_CIGMAR_TABLEROW_HPP
