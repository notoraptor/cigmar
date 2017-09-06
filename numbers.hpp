#ifndef CIGMAR_NUMBERS
#define CIGMAR_NUMBERS

#include <iostream>
#include <chrono>
#include <random>
#include <type_traits>
#include "primitive/classes.hpp"
#include "primitive/types/return_t.hpp"

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

	namespace integers {

		template<typename C, typename A, typename B>
		C& uniform(C& arr, A a, B b) {
			typedef typename C::dtype dtype;
			std::uniform_int_distribution<dtype> distribution(a, b);
			for (dtype& x : arr) x = distribution(rng.get());
			return arr;
		}

	}

	namespace reals {

		template<typename C, typename A, typename B>
		C& uniform(C& arr, A a, B b) {
			typedef typename C::dtype dtype;
			std::uniform_real_distribution<dtype> distribution(a, b);
			for (dtype& x: arr) x = distribution(rng.get());
			return arr;
		}

	}

	template<typename T, size_t N>
	array_t<T, N>& binomial(array_t<T, N>& arr, T n, double p) {
		std::binomial_distribution<T> distribution(n, p);
		for (T& x: arr) x = distribution(rng.get());
		return arr;
	}

	template<typename T, size_t N>
	array_t<T, N>& normal(array_t<T, N>& arr, double mu, double sigma) {
		static_assert(std::is_signed<T>{}, "normal distribution is forbidden for signed types, as it may generate negative values.");
		std::normal_distribution<double> distribution(mu, sigma);
		for (T& x: arr) x = (T)distribution(rng.get());
		return arr;
	}
}

}

#endif // CIGMAR_INTEGER
