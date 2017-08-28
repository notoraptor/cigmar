#ifndef CIGMAR_NUMBERS
#define CIGMAR_NUMBERS

#include <iostream>
#include <chrono>
#include <random>
#include "primitive/classes.hpp"

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

namespace random {
	template<typename T, size_t N>
	array_t<T, N>& uniform(array_t<T, N>& arr, T a, T b) {
		std::uniform_int_distribution<T> distribution(a, b);
		for (T& x: arr) x = distribution(rng.get());
		return arr;
	}
	template<typename T, size_t N>
	array_t<T, N>& binomial(array_t<T, N>& arr, T n, double b) {
		std::binomial_distribution<T> distribution(n, b);
		for (T& x: arr) x = distribution(rng.get());
		return arr;
	}
	template<typename T, size_t N>
	array_t<T, N>& normal(array_t<T, N>& arr, double mu, double sigma) {
		std::normal_distribution<double> distribution(mu, sigma);
		for (T& x: arr) {
			double d = distribution(rng.get());
			std::cout << d << std::endl;
			x = (T)d;
		}
		return arr;
	}
}

}

#endif // CIGMAR_INTEGER
