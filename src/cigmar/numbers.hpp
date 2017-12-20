#ifndef CIGMAR_NUMBERS
#define CIGMAR_NUMBERS

#include <iostream>
#include <chrono>
#include <random>
#include <functional>
#include <type_traits>
#include <cigmar/interfaces/Collection.hpp>

namespace cigmar::numbers {

template<typename T>
Collection<T>& arange(Collection<T>& arr) {
	size_t c = 0;
	for (auto& x: arr) {
		x = c;
		++c;
	}
	return arr;
}

template<typename T>
Collection<T>& zeros(Collection<T>& arr) {
	for (auto& x: arr) x = 0;
	return arr;
}

template<typename T>
Collection<T>& ones(Collection<T>& arr) {
	for (auto& x: arr) x = 1;
	return arr;
}

template<typename T, typename V>
Collection<T>& fill(Collection<T>& arr, V value) {
	for (auto& x: arr) x = value;
	return arr;
}

template<typename T, typename V>
Collection<T>& add(Collection<T>& arr, V value) {
	for (auto& x: arr) x += value;
	return arr;
}

template<typename T, typename V>
Collection<T>& sub(Collection<T>& arr, V value) {
	for (auto& x: arr) x -= value;
	return arr;
}

template<typename T, typename V>
Collection<T>& mul(Collection<T>& arr, V value) {
	for (auto& x: arr) x *= value;
	return arr;
}

template<typename T, typename V>
Collection<T>& div(Collection<T>& arr, V value) {
	for (auto& x: arr) x /= value;
	return arr;
}

template<typename T, typename V>
Collection<T>& mod(Collection<T>& arr, V value) {
	for (auto& x: arr) x %= value;
	return arr;
}

template<typename T, typename O = typename Collection<T>::dtype>
void sum(const Collection<T>& arr, O& out, bool reset = true) {
	if (reset) out = 0;
	for(auto& x: arr) out += x;
}

template<typename T, typename O = typename Collection<T>::dtype>
O sum(const Collection<T>& arr) {
	O out = 0;
	for (auto& x: arr) out += x;
	return out;
}

template<typename T, typename O = typename Collection<T>::dtype>
O prod(const Collection<T>& arr) {
	O out = 1;
	for (auto& x: arr) out *= x;
	return out;
}

namespace random {

	class RNG {
	private:
		unsigned int m_seed;
		std::default_random_engine generator;
	public:
		RNG() noexcept : generator() {
			m_seed = std::chrono::system_clock::now().time_since_epoch().count();
			generator.seed(m_seed);
		}
		std::default_random_engine& get() {
			return generator;
		}
		unsigned int seed() const {return m_seed;}
		RNG& seed(unsigned int s) {
			m_seed = s;
			generator.seed(s);
			return *this;
		}
	};

	extern RNG rng;

	template<typename T, typename A, typename B>
	Collection<T>& uniform(Collection<T>& arr, A a, B b) {
		typedef typename Collection<T>::dtype dtype;
		typedef typename std::conditional<
			std::is_integral<dtype>::value,
			std::uniform_int_distribution<dtype>,
			std::uniform_real_distribution<dtype>
		>::type distribution_t;
		distribution_t distribution(a, b);
		for (dtype& x: arr) x = distribution(rng.get());
		return arr;
	}

	template<typename T, typename N>
	Collection<T>& binomial(Collection<T>& arr, N n, double p) {
		typedef typename Collection<T>::dtype dtype;
		static_assert(
			std::is_integral<N>{},
			"binomial distribution: parameter 'n' must be an integer type."
		);
		std::binomial_distribution<N> distribution(n, p);
		for (dtype& x: arr) x = distribution(rng.get());
		return arr;
	}

	template<typename T>
	Collection<T>& normal(Collection<T>& arr, double mu, double sigma) {
		typedef typename Collection<T>::dtype dtype;
		static_assert(
			std::is_signed<dtype>{},
			"normal distribution is forbidden for non-signed types, as it may generate negative values."
		);
		std::normal_distribution<double> distribution(mu, sigma);
		for (dtype& x: arr) x = (dtype)distribution(rng.get());
		return arr;
	}

	template<typename A, typename B>
	A uniform(A a, B b) {
		typedef typename std::conditional<
			std::is_integral<A>::value,
			std::uniform_int_distribution<A>,
			std::uniform_real_distribution<A>
		>::type distribution_t;
		distribution_t distribution(a, b);
		return distribution(rng.get());
	};

	template<typename N>
	N binomial(N n, double p) {
		static_assert(
			std::is_integral<N>{},
			"binomial distribution: parameter 'n' must be an integer type."
		);
		std::binomial_distribution<N> distribution(n, p);
		return distribution(rng.get());
	}

	inline double normal(double mu, double sigma) {
		std::normal_distribution<double> distribution(mu, sigma);
		return distribution(rng.get());
	}
}

template<typename T, typename dtype = typename Collection<T>::dtype, typename F = std::function<bool(dtype)>>
bool all(const Collection<T>& arr, F elemwiseChecker) {
	for (const dtype& x: arr) if (!elemwiseChecker(x)) return false;
	return true;
}

template<typename T, typename dtype = typename Collection<T>::dtype, typename F = std::function<bool(dtype)>>
bool any(const Collection<T>& arr, F elemwiseChecker) {
	for (const dtype& x: arr) if (elemwiseChecker(x)) return true;
	return false;
}

}

#endif // CIGMAR_INTEGER
