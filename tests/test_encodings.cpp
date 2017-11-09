//
// Created by notoraptor on 09/11/2017.
//
#include <cigmar/unittests.hpp>
#include <cigmar/hex.hpp>
#include <cigmar/print.hpp>
using namespace cigmar;

utt_begin(TestEncodings);
	utt(test_hex) {
		utt_assert(hex::encode(0) == "00");
		utt_assert(hex::encode(1) == "01");
		utt_assert(hex::encode(10) == "0A");
		utt_assert(hex::encode(15) == "0F");
		utt_assert(hex::encode(255) == "FF");
		utt_assert(hex::encode(256) == "0100");
		uint64_t value = ~((uint64_t)0);
		utt_assert(hex::encode(value) == String(16, 'F'));
	}
utt_end();
