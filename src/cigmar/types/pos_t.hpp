#ifndef CIGMAR_POS_T
#define CIGMAR_POS_T
#include <string>
#include <ostream>
#include <limits>
#include <cigmar/interfaces/Comparable.hpp>
#include <cigmar/interfaces/CrossComparable.hpp>
#include <cigmar/interfaces/Hashable.hpp>

namespace cigmar {
	class pos_t: public Comparable<pos_t>, public CrossComparable<size_t>, public Hashable {
	private:
		bool valid;
		size_t value;
	public:
		pos_t(): valid(false), value(-1) {}
		pos_t(size_t v): valid(true), value(v) {}
		pos_t(bool ok, size_t v): valid(ok), value(ok ? v : -1) {}
		explicit operator bool() const {return valid;}
		explicit operator size_t() const {return value;}
		size_t hash() const override {
			std::hash<size_t> h;
			return h(valid) ^ h(value);
		}
		int compare(const pos_t& other) const override {
			return (!valid && !other.valid) ? 0 : ( value < other.value ? -1 : value - other.value );
		}
		int crossCompare(size_t other) const override {
			return (!valid || value < other) ? -1 : value - other;
		}
		pos_t& operator=(size_t v) {
			valid = true;
			value = v;
			return *this;
		}
		pos_t& operator++() {
			if (value == std::numeric_limits<size_t>::max())
				valid = false;
			else
				++value;
			return *this;
		}
		pos_t operator++(int) {
			pos_t ret(*this);
			++(*this);
			return ret;
		}
		pos_t& operator--() {
			if (value == 0)
				valid = false;
			else
				--value;
			return *this;
		}
		pos_t operator--(int) {
			pos_t ret(*this);
			--(*this);
			return ret;
		}
		pos_t& operator+=(size_t v) {
			if (value > std::numeric_limits<size_t>::max() - v)
				valid = false;
			else
				value += v;
			return *this;
		}
		pos_t& operator-=(size_t v) {
			if (value < v)
				valid = false;
			else
				value -= v;
			return *this;
		}
		pos_t& operator*=(size_t v) {
			if (value > std::numeric_limits<size_t>::max() / v)
				valid = false;
			else
				value *= v;
			return *this;
		}
		pos_t& operator/=(size_t v) {
			if (v == 0)
				valid = false;
			else
				value /= v;
			return *this;
		}
		pos_t& operator%=(size_t v) {
			if (v == 0)
				valid = false;
			else
				value %= v;
			return *this;
		}
		pos_t& operator+=(const pos_t& p) {
			if (valid && p.valid)
				*this += p.value;
			else
				valid = false;
			return *this;
		}
		pos_t& operator-=(const pos_t& p) {
			if (valid && p.valid)
				*this -= p.value;
			else
				valid = false;
			return *this;
		}
		pos_t& operator*=(const pos_t& p) {
			if (valid && p.valid)
				*this *= p.value;
			else
				valid = false;
			return *this;
		}
		pos_t& operator/=(const pos_t& p) {
			if (valid && p.valid)
				*this /= p.value;
			else
				valid = false;
			return *this;
		}
		pos_t& operator%=(const pos_t& p) {
			if (valid && p.valid)
				*this %= p.value;
			else
				valid = false;
			return *this;
		}
		pos_t operator+(const pos_t& p) const {
			return (pos_t(*this) += p);
		}
		pos_t operator-(const pos_t& p) const {
			return (pos_t(*this) -= p);
		}
		pos_t operator*(const pos_t& p) const {
			return (pos_t(*this) *= p);
		}
		pos_t operator/(const pos_t& p) const {
			return (pos_t(*this) /= p);
		}
		pos_t operator%(const pos_t& p) const {
			return (pos_t(*this) %= p);
		}
		pos_t operator+(size_t v) const {
			return (pos_t(*this) += v);
		}
		pos_t operator-(size_t v) const {
			return (pos_t(*this) -= v);
		}
		pos_t operator*(size_t v) const {
			return (pos_t(*this) *= v);
		}
		pos_t operator/(size_t v) const {
			return (pos_t(*this) /= v);
		}
		pos_t operator%(size_t v) const {
			return (pos_t(*this) %= v);
		}
	};
	inline pos_t operator+(size_t a, const pos_t& b) {
		return b + a;
	}
	inline pos_t operator-(size_t a, const pos_t& b) {
		return (b && a >= (size_t)b) ? pos_t(a - (size_t)b) : b;
	}
	inline pos_t operator*(size_t a, const pos_t& b) {
		return b * a;
	}
	inline pos_t operator/(size_t a, const pos_t& b) {
		return b ? pos_t(a / (size_t)b) : b;
	}
	inline pos_t operator%(size_t a, const pos_t& b) {
		return b ? pos_t(a % (size_t)b) : b;
	}
	template <typename C>
	std::basic_ostream<C>& operator<<(std::basic_ostream<C>& o, const pos_t& p) {
		if (p)
			o << (size_t)p;
		else
			o << "-1";
		return o;
	}
}

#endif // CIGMAR_POS_T
