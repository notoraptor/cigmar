#ifndef CIGMAR_NUMBERS
#define CIGMAR_NUMBERS

#include <iostream>
#include <chrono>
#include <random>
#include <functional>
#include <type_traits>
#include <cigmar/symbols.hpp>

namespace cigmar::numbers {

	template<typename Iterable>
	Iterable& arange(Iterable& arr) {
		static_assert(is_iterable<Iterable>{});
		size_t c = 0;
		for (auto& x: arr) {
			x = c;
			++c;
		}
		return arr;
	}

	template<typename Iterable>
	Iterable& zeros(Iterable& arr) {
		static_assert(is_iterable<Iterable>{});
		for (auto& x: arr) x = 0;
		return arr;
	}

	template<typename Iterable>
	Iterable& ones(Iterable& arr) {
		static_assert(is_iterable<Iterable>{});
		for (auto& x: arr) x = 1;
		return arr;
	}

	template<typename Iterable, typename V>
	Iterable& fill(Iterable& arr, V value) {
		static_assert(is_iterable<Iterable>{});
		for (auto& x: arr) x = value;
		return arr;
	}

	template<typename Iterable, typename V>
	Iterable& add(Iterable& arr, V value) {
		static_assert(is_iterable<Iterable>{});
		for (auto& x: arr) x += value;
		return arr;
	}

	template<typename Iterable, typename V>
	Iterable& sub(Iterable& arr, V value) {
		static_assert(is_iterable<Iterable>{});
		for (auto& x: arr) x -= value;
		return arr;
	}

	template<typename Iterable, typename V>
	Iterable& mul(Iterable& arr, V value) {
		static_assert(is_iterable<Iterable>{});
		for (auto& x: arr) x *= value;
		return arr;
	}

	template<typename Iterable, typename V>
	Iterable& div(Iterable& arr, V value) {
		static_assert(is_iterable<Iterable>{});
		for (auto& x: arr) x /= value;
		return arr;
	}

	template<typename Iterable, typename V>
	Iterable& mod(Iterable& arr, V value) {
		static_assert(is_iterable<Iterable>{});
		for (auto& x: arr) x %= value;
		return arr;
	}

	template<typename Iterable, typename O>
	void sum(const Iterable& arr, O& out, bool reset = true) {
		static_assert(is_iterable<Iterable>{});
		if (reset) out = 0;
		for(auto& x: arr) out += x;
	}

	template<typename Iterable, typename O = decltype(*std::begin(std::declval<Iterable&>()))>
	O sum(const Iterable& arr) {
		static_assert(is_iterable<Iterable>{});
		O out = 0;
		for (auto& x: arr) out += x;
		return out;
	}

	template<typename Iterable, typename O = decltype(*std::begin(std::declval<Iterable&>()))>
	O prod(const Iterable& arr) {
		static_assert(is_iterable<Iterable>{});
		O out = 1;
		for (auto& x: arr) out *= x;
		return out;
	}

	namespace random {

		class RNG {
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

		template<typename Iterable, typename A, typename B>
		Iterable& uniform(Iterable& arr, A a, B b) {
			static_assert(is_iterable<Iterable>{});
			typedef typename decltype(*std::begin(std::declval<Iterable&>())) dtype;
			typedef typename std::conditional<
				std::is_integral<dtype>::value,
				std::uniform_int_distribution<dtype>,
				std::uniform_real_distribution<dtype>
			>::type distribution_t;
			distribution_t distribution(a, b);
			for (dtype& x: arr) x = distribution(rng.get());
			return arr;
		}

		template<typename Iterable, typename N>
		Iterable& binomial(Iterable& arr, N n, double p) {
			static_assert(is_iterable<Iterable>{});
			static_assert(std::is_integral<N>{}, "binomial distribution: parameter 'n' must be an integer type.");
			typedef typename decltype(*std::begin(std::declval<Iterable&>())) dtype;
			std::binomial_distribution<N> distribution(n, p);
			for (dtype& x: arr) x = distribution(rng.get());
			return arr;
		}

		template<typename Iterable>
		Iterable& normal(Iterable& arr, double mu, double sigma) {
			static_assert(is_iterable<Iterable>{});
			typedef typename decltype(*std::begin(std::declval<Iterable&>())) dtype;
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

	template<
			typename Iterable,
			typename dtype = typename decltype(*std::begin(std::declval<Iterable&>())),
			typename F = std::function<bool(dtype)>
	>
	bool all(const Iterable& arr, F elemwiseChecker) {
		static_assert(is_iterable<Iterable>{});
		for (const dtype& x: arr) if (!elemwiseChecker(x)) return false;
		return true;
	}

	template<
			typename Iterable,
			typename dtype = typename decltype(*std::begin(std::declval<Iterable&>())),
			typename F = std::function<bool(dtype)>
	>
	bool any(const Iterable& arr, F elemwiseChecker) {
		static_assert(is_iterable<Iterable>{});
		for (const dtype& x: arr) if (elemwiseChecker(x)) return true;
		return false;
	}

}

#endif // CIGMAR_INTEGER
