//
// Created by notoraptor on 17-12-21.
//
#include <cigmar/classes/String.hpp>
#include <cigmar/unittests.hpp>
#include <cigmar/print.hpp>

using namespace cigmar;
utt_begin(TestString);
	utt(test_empty_constructor) {
		String s;
		utt_assert(s.isEmpty());
		utt_assert(!s);
	}
	utt(test_basic_constructor) {
		String s = "Hello!";
		utt_assert(s);
		utt_assert(!s.isEmpty());
		utt_assert(s.length() == 6);
	}
	utt(test_constructors) {
		const char* ls = "Hello, world!";
		String s1(ls, 0);
		String s2(ls, 0, 5);
		String s3(ls, 7);
		String s4(ls, 7, 3);
		utt_assert(s1 == ls);
		utt_assert(ls == s1);
		utt_assert(s2 == "Hello");
		utt_assert("world!" == s3);
		utt_assert(s4 == "wor");
		utt_assert(s4 != "woR");
		utt_assert("Wor" != s4);
	}
utt_end();
