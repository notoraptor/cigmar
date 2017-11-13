#ifndef CIGMAR_NUMBERS
#define CIGMAR_NUMBERS

#include <iostream>
#include <chrono>
#include <random>
#include <functional>
#include <type_traits>
#include <cigmar/interfaces/Collection.hpp>

#define COLLECTION_TEMPLATE typename T, typename I, typename C
#define COLLECTION Collection<T, I, C>

namespace cigmar::numbers {

template<COLLECTION_TEMPLATE>
COLLECTION& arange(COLLECTION& arr) {
	size_t c = 0;
	for (auto& x: arr) {
		x = c;
		++c;
	}
	return arr;
}

template<COLLECTION_TEMPLATE>
COLLECTION& zeros(COLLECTION& arr) {
	for (auto& x: arr) x = 0;
	return arr;
}

template<COLLECTION_TEMPLATE>
COLLECTION& ones(COLLECTION& arr) {
	for (auto& x: arr) x = 1;
	return arr;
}

template<COLLECTION_TEMPLATE, typename V>
COLLECTION& fill(COLLECTION& arr, V value) {
	for (auto& x: arr) x = value;
	return arr;
}

template<COLLECTION_TEMPLATE, typename V>
COLLECTION& add(COLLECTION& arr, V value) {
	for (auto& x: arr) x += value;
	return arr;
}

template<COLLECTION_TEMPLATE, typename V>
COLLECTION& sub(COLLECTION& arr, V value) {
	for (auto& x: arr) x -= value;
	return arr;
}

template<COLLECTION_TEMPLATE, typename V>
COLLECTION& mul(COLLECTION& arr, V value) {
	for (auto& x: arr) x *= value;
	return arr;
}

template<COLLECTION_TEMPLATE, typename V>
COLLECTION& div(COLLECTION& arr, V value) {
	for (auto& x: arr) x /= value;
	return arr;
}

template<COLLECTION_TEMPLATE, typename V>
COLLECTION& mod(COLLECTION& arr, V value) {
	for (auto& x: arr) x %= value;
	return arr;
}

template<COLLECTION_TEMPLATE, typename O = typename COLLECTION::dtype>
void sum(const COLLECTION& arr, O& out, bool reset = true) {
	if (reset) out = 0;
	for(auto& x: arr) out += x;
}

template<COLLECTION_TEMPLATE>
T sum(const COLLECTION& arr) {
	T out = 0;
	for (auto& x: arr) out += x;
	return out;
}

template<COLLECTION_TEMPLATE, typename O = typename COLLECTION::dtype>
O prod(const COLLECTION& arr) {
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

	template<COLLECTION_TEMPLATE, typename A, typename B>
	COLLECTION& uniform(COLLECTION& arr, A a, B b) {
		typedef typename COLLECTION::dtype dtype;
		typedef typename std::conditional<
			std::is_integral<dtype>::value,
			std::uniform_int_distribution<dtype>,
			std::uniform_real_distribution<dtype>
		>::type distribution_t;
		distribution_t distribution(a, b);
		for (dtype& x: arr) x = distribution(rng.get());
		return arr;
	}

	template<COLLECTION_TEMPLATE, typename N>
	COLLECTION& binomial(COLLECTION& arr, N n, double p) {
		typedef typename COLLECTION::dtype dtype;
		static_assert(
			std::is_integral<N>{},
			"binomial distribution: parameter 'n' must be an integer type."
		);
		std::binomial_distribution<N> distribution(n, p);
		for (dtype& x: arr) x = distribution(rng.get());
		return arr;
	}

	template<COLLECTION_TEMPLATE>
	COLLECTION& normal(COLLECTION& arr, double mu, double sigma) {
		typedef typename COLLECTION::dtype dtype;
		static_assert(
			std::is_signed<dtype>{},
			"normal distribution is forbidden for signed types, as it may generate negative values."
		);
		std::normal_distribution<double> distribution(mu, sigma);
		for (dtype& x: arr) x = (dtype)distribution(rng.get());
		return arr;
	}
}

template<COLLECTION_TEMPLATE, typename dtype = typename COLLECTION::dtype, typename F = std::function<bool(dtype)>>
bool all(const C& arr, F elemwiseChecker) {
	for (const dtype& x: arr) if (!elemwiseChecker(x)) return false;
	return true;
}

template<COLLECTION_TEMPLATE, typename dtype = typename COLLECTION::dtype, typename F = std::function<bool(dtype)>>
bool any(const C& arr, F elemwiseChecker) {
	for (const dtype& x: arr) if (elemwiseChecker(x)) return true;
	return false;
}

}

#undef COLLECTION_TEMPLATE
#undef COLLECTION

#endif // CIGMAR_INTEGER
