//
// Created by notoraptor on 09/11/2017.
//
#include <cigmar/unittests.hpp>
#include <cigmar/cigmar.hpp>

using namespace cigmar;

utt_begin(TestEncodings);
	utt(test_hex) {
		utt_assert(hex::encode(0) == "00");
		utt_assert(hex::encode(1) == "01");
		utt_assert(hex::encode(10) == "0A");
		utt_assert(hex::encode(15) == "0F");
		utt_assert(hex::encode(255) == "FF");
		utt_assert(hex::encode(256) == "0100");
		uint64_t value = ~uint64_t(0);
		utt_assert(hex::encode(value) == String(16, 'F'));
	}
	utt(test_base64_on_bytes) {
		numbers::random::rng.seed(1234);
		ArrayList<byte_t> data(37);
		pos_t zeroPos;
		do {
			numbers::random::uniform(data, 0, 5);
			zeroPos = data.indexOf(0);
		} while (!zeroPos || !(size_t)zeroPos);
		utt_assert(zeroPos > 0);
		utt_assert(data[(size_t)zeroPos] == 0);
		String encoded = base64::bytes::encode((byte_t*)data, data.size());
		ArrayList<byte_t> decoded = base64::bytes::decode(encoded);
		utt_assert(data == decoded);
	}
utt_end();
