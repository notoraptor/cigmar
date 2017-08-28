#ifndef CIGMAR_POS_T
#define CIGMAR_POS_T

#include <string>
#include <ostream>
#include "../interfaces.hpp"

class pos_t: public Comparable<pos_t>, public Streamable {
private:
	bool valid;
	size_t value;
public:
	pos_t(): valid(false), value(-1) {}
	pos_t(size_t v): valid(true), value(v) {}
	pos_t(bool ok, size_t v): valid(ok), value(v) {}
	explicit operator bool() const {return valid;}
	int compare(const pos_t& other) const override {
		return (!valid && !other.valid) ? 0 : ( value < other.value ? -1 : value - other.value );
	}
	void toStream(std::ostream& o) const override {
		if (valid)
			o << value;
		else
			o << -1;
	}
	static pos_t _stringpos(size_t pos) {
		return pos_t(pos != std::string::npos, pos);
	}
};

#endif // CIGMAR_POS_T
