//
// Created by notoraptor on 16/11/2017.
//
#include <cigmar/math/Fraction.hpp>
#include <cigmar/unittests.hpp>
#include <cigmar/classes/String.hpp>

using namespace cigmar;
utt_begin(TestMath);
	utt(test_fraction) {
		math::Fraction f1, f2(0), f3(1), f4(-1), f5(32), f6(-64), f7(2, 3), f8(-15, 9), f9(-1, 12, 9), f10(1, 17, 69);
		utt_assert(String::write(f1) == "0");
		utt_assert(String::write(f2) == "0");
		utt_assert(String::write(f3) == "1");
		utt_assert(String::write(f4) == "-1");
		utt_assert(String::write(f5) == "32");
		utt_assert(String::write(f6) == "-64");
		utt_assert(String::write(f7) == "2/3");
		utt_assert(String::write(f8) == "-5/3");
		utt_assert(String::write(f9) == "-4/3");
		utt_assert(String::write(f10) == "17/69");
	}
utt_end();
