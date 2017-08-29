#ifndef CIGMAR_POS_T
#define CIGMAR_POS_T
#include <string>
#include <ostream>
#include <limits>
#include "../interfaces.hpp"

class pos_t: public AutoComparable<pos_t>, public Streamable {
private:
	bool valid;
	size_t value;
public:
	pos_t(): valid(false), value(-1) {}
	pos_t(size_t v): valid(true), value(v) {}
	pos_t(bool ok, size_t v): valid(ok), value(ok ? v : -1) {}
	explicit operator bool() const {return valid;}
	explicit operator size_t() const {return value;}
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
	pos_t operator++(int) {
	    pos_t ret = *this;
        if (valid) {
            if (value == std::numeric_limits<size_t>::max()) {
                valid = false;
            } else {
                ++value;
            }
        }
        return ret;
	}
	pos_t& operator++() {
        if (valid) {
            if (value == std::numeric_limits<size_t>::max()) {
                valid = false;
            } else {
                ++value;
            }
        }
        return *this;
	}
	pos_t operator--(int) {
        pos_t ret = *this;
        if (valid) {
            if (value == 0) {
                valid = false;
            } else {
                --value;
            }
        }
        return ret;
	}
	pos_t& operator--() {
        if (valid) {
            if (value == 0) {
                valid = false;
            } else {
                --value;
            }
        }
        return *this;
	}
	pos_t& operator+=(size_t v) {
        if (value > std::numeric_limits<size_t>::max() - v) {
            valid = false;
        } else {
            value += v;
        }
        return *this;
	}
	pos_t& operator-=(size_t v) {
        if (value < v) {
            valid = false;
        } else {
            value -= v;
        }
        return *this;
	}
	pos_t& operator*=(size_t v) {
        if (value > std::numeric_limits<size_t>::max() / v) {
            valid = false;
        } else {
            value *= v;
        }
        return *this;
	}
	pos_t& operator/=(size_t v) {
        value /= v;
        return *this;
	}
	pos_t& operator%=(size_t v) {
        value %= v;
        return *this;
	}
	pos_t operator+(const pos_t& p) const {
	    if (!valid || !p.valid)
            return pos_t();
        pos_t ret = *this;
        ret += p.value;
        return ret;
	}
	pos_t operator-(const pos_t& p) const {
	    if (!valid || !p.valid)
            return pos_t();
        pos_t ret = *this;
        ret -= p.value;
        return ret;
	}
	pos_t operator*(const pos_t& p) const {
	    if (!valid || !p.valid)
            return pos_t();
        pos_t ret = *this;
        ret *= p.value;
        return ret;
	}
	pos_t operator/(const pos_t& p) const {
	    if (!valid || !p.valid)
            return pos_t();
        pos_t ret = *this;
        ret /= p.value;
        return ret;
	}
	pos_t operator%(const pos_t& p) const {
	    if (!valid || !p.valid)
            return pos_t();
        pos_t ret = *this;
        ret %= p.value;
        return ret;
	}
};

pos_t operator+(size_t a, const pos_t& b) {
    return b + a;
}
pos_t operator-(size_t a, const pos_t& b) {
    if (b && a > (size_t)b)
        return pos_t(a - (size_t)b);
    return pos_t();
}
pos_t operator*(size_t a, const pos_t& b) {
    return b * a;
}
pos_t operator/(size_t a, const pos_t& b) {
    if (b)
        return pos_t(a / (size_t)b);
    return pos_t();
}
pos_t operator%(size_t a, const pos_t& b) {
    if (b)
        return pos_t(a % (size_t)b);
    return pos_t();
}

#endif // CIGMAR_POS_T
