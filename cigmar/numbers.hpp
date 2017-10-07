#ifndef CIGMAR_NUMBERS
#define CIGMAR_NUMBERS

#include <iostream>
#include <chrono>
#include <random>
#include <type_traits>
#include <cigmar/primitive/classes.hpp>
#include <cigmar/primitive/types/return_t.hpp>

namespace cigmar {
namespace numbers {

class RNG {
private:
	unsigned int _seed;
	std::default_random_engine generator;
public:
	RNG(): generator() {
		_seed = std::chrono::system_clock::now().time_since_epoch().count();
		generator.seed(_seed);
	}
	std::default_random_engine& get() {
		return generator;
	}
	unsigned int seed() const {return _seed;}
	RNG& seed(unsigned int s) {
		_seed = s;
		generator.seed(s);
		return *this;
	}
};

extern RNG rng;

template<typename C>
C& arange(C& arr) {
	size_t c = 0;
	for (auto& x: arr) {
		x = c;
		++c;
	}
	return arr;
}

template<typename C>
C& zeros(C& arr) {
	for (auto& x: arr) x = 0;
	return arr;
}

template<typename C>
C& ones(C& arr) {
	for (auto& x: arr) x = 1;
	return arr;
}

template<typename C, typename T>
C& fill(C& arr, T value) {
	for (auto& x: arr) x = value;
	return arr;
}

template<typename C, typename T>
C& add(C& arr, T value) {
	for (auto& x: arr) x += value;
	return arr;
}

template<typename C, typename T>
C& sub(C& arr, T value) {
	for (auto& x: arr) x -= value;
	return arr;
}

template<typename C, typename T>
C& mul(C& arr, T value) {
	for (auto& x: arr) x *= value;
	return arr;
}

template<typename C, typename T>
C& div(C& arr, T value) {
	for (auto& x: arr) x /= value;
	return arr;
}

template<typename C, typename T>
C& mod(C& arr, T value) {
	for (auto& x: arr) x %= value;
	return arr;
}

template<typename C, typename O = typename C::dtype>
void sum(C& arr, O& out, bool reset = true) {
	if (reset) out = 0;
	for(auto& x: arr) out += x;
}

template<typename C, typename O = typename C::dtype>
O sum(C& arr) {
	O out = 0;
	for (auto& x: arr) out += x;
	return out;
}

template<typename C, typename O = typename C::dtype>
O prod(C& arr) {
	O out = 1;
	for (auto& x: arr) out *= x;
	return out;
}

namespace random {

	template<typename C, typename A, typename B>
	C& uniform(C& arr, A a, B b) {
		typedef typename C::dtype dtype;
		typedef typename std::conditional<
			std::is_integral<dtype>::value,
			std::uniform_int_distribution<dtype>,
			std::uniform_real_distribution<dtype>
		>::type distribution_t;
		distribution_t distribution(a, b);
		for (dtype& x: arr) x = distribution(rng.get());
		return arr;
	}

	template<typename C, typename N>
	C& binomial(C& arr, N n, double p) {
		typedef typename C::dtype dtype;
		static_assert(
			std::is_integral<N>{},
			"binomial distribution: parameter 'n' must be an integer type."
		);
		std::binomial_distribution<N> distribution(n, p);
		for (dtype& x: arr) x = distribution(rng.get());
		return arr;
	}

	template<typename C>
	C& normal(C& arr, double mu, double sigma) {
		typedef typename C::dtype dtype;
		static_assert(
			std::is_signed<dtype>{},
			"normal distribution is forbidden for signed types, as it may generate negative values."
		);
		std::normal_distribution<double> distribution(mu, sigma);
		for (dtype& x: arr) x = (dtype)distribution(rng.get());
		return arr;
	}
}

template<typename C, typename dtype = typename C::dtype, typename F = std::function<bool(dtype)>>
bool all(const C& arr, F elemwiseChecker) {
	for (const dtype& x: arr) if (!elemwiseChecker(x)) return false;
	return true;
}

template<typename C, typename dtype = typename C::dtype, typename F = std::function<bool(dtype)>>
bool any(const C& arr, F elemwiseChecker) {
	for (const dtype& x: arr) if (elemwiseChecker(x)) return true;
	return false;
}

}
}

#endif // CIGMAR_INTEGER
