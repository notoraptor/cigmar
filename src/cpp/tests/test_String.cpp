//
// Created by HPPC on 07/11/2017.
//
#include <cigmar/unittests.hpp>
#include <cigmar/classes/String.hpp>
using namespace cigmar;

utt_begin(TestString);

utt(test_comparisons) {
	String a1 = "assert";
	String a2 = "assert";
	String b = "b";
	String c = "c";
	utt_assert(a1 == a1);
	utt_assert(!(a1 != a1));
	utt_assert(a1 == a2);
	utt_assert(!(a1 != a2));
	utt_assert(a1 == "assert");
	utt_assert(!(a1 != "assert"));
	utt_assert("assert" == a2);
	utt_assert(!("assert" != a2));
	utt_assert(b != c);
	utt_assert(!(b == c));
	utt_assert(b < c);
	utt_assert(!(b > c));
	utt_assert(!(b >= c));
	utt_assert(b > a1);
	utt_assert(!(b < a1));
	utt_assert(!(b <= a1));
	utt_assert(a1 <= a2);
	utt_assert(!(a1 > a2));
	utt_assert(a1 <= b);
	utt_assert(!(a1 > b));
	utt_assert(a2 >= a1);
	utt_assert(!(a2 < a1));
	utt_assert(c >= a1);
	utt_assert(b != "c");
	utt_assert(b <= "b");
	utt_assert(b <= "c");
	utt_assert(b < "c");
	utt_assert(b > "a");
	utt_assert(b >= "b");
	utt_assert(b >= "a");
}

utt_end();
