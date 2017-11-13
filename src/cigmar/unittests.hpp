//
// Created by notoraptor on 08/11/2017.
//

#ifndef CIGMAR_UTT_HPP
#define CIGMAR_UTT_HPP

#include <functional>
#include <sstream>
#include <cigmar/numbers.hpp>

namespace cigmar::tests {
	// typedef std::function<void()> UttMethod;
	typedef void(*UttMethod)();

	class UttRecorder {
	public:
		UttRecorder(UttMethod method, const char* className, const char* methodName) noexcept ;
	};

	void run();

	inline void utt_assert_bool(bool condition, const char* else_message = nullptr) {
		if (!condition) {
			std::ostringstream s;
			s << "Error occurred (seed: " << numbers::random::rng.seed() << ')' << std::endl;
			if (else_message)
				s << "Assertion failed:" << std::endl << else_message;
			else
				s << "Assertion failed.";
			throw s.str();
		}
	}

	template<typename T>
	inline void utt_smart_assert_bool(bool condition, const char* message, T value) {
		if (!condition) {
			std::ostringstream s;
			if (message)
				s << message << std::endl << "Got: ";
			s << value << std::endl;
			utt_assert_bool(condition, s.str().c_str());
		}
	}

}

#define utt_begin(name) namespace cigmar::tests::name { static const char* classname = #name ;

#define utt(name) \
static void name(); \
static UttRecorder recorder_##name (&name, classname, #name ); \
static void name()

#define utt_end() }

#define utt_assert(condition_code) cigmar::tests::utt_assert_bool((bool)(condition_code), #condition_code)
#define utt_smart_assert(condition_code, print_value) cigmar::tests::utt_smart_assert_bool((bool)(condition_code), #condition_code , print_value)

#endif //CIGMAR_UTT_HPP
