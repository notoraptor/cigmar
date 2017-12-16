//
// Created by notoraptor on 09/11/2017.
//
#include <cigmar/classes/ArrayList.hpp>
#include <cigmar/classes/array_t.hpp>
#include <cigmar/unittests.hpp>
using namespace cigmar;

utt_begin(TestArrays);

	utt(test_array_t) {
		array_t<long long int, 8> r;
		numbers::zeros(r);
		utt_assert(numbers::sum(r) == 0);
		numbers::ones(r);
		utt_assert(numbers::sum(r) == r.size());
		numbers::add(r, 12);
		numbers::add(r, -1);
		numbers::mul(r, -3.5);
		utt_assert(r[0] == r[LAST]);
		utt_assert(r[r.size() / 2] == -42);
		numbers::random::uniform(r, 5, 34);
		long long int x = numbers::sum(r);
		long long int y = -37;
		numbers::sum(r, y, false);
		utt_assert(y == x - 37);
		numbers::sum(r, y);
		utt_assert(x == y);
		array_t<double, 44> u(0);
		array_t<double, 44> v(-1);
		utt_assert(numbers::sum(u) == 0);
		utt_assert(numbers::sum(v) == -(double)v.size());
	}

	utt(test_ArrayList) {
		ArrayList<int> r((size_t)8);
		numbers::zeros(r);
		utt_assert(numbers::sum(r) == 0);
		numbers::ones(r);
		utt_assert(numbers::sum(r) == (int)r.size());
		numbers::add(r, 12);
		numbers::add(r, -1);
		numbers::mul(r, -3.5);
		utt_assert(r[0] == r[LAST]);
		utt_assert(r[r.size() / 2] == -42);
		numbers::random::uniform(r, 5, 34);
		long long int x = numbers::sum(r);
		long long int y = -37;
		numbers::sum(r, y, false);
		utt_assert(y == x - 37);
		numbers::sum(r, y);
		utt_assert(x == y);
		ArrayList<double> u(44, 0);
		ArrayList<double> v(44, -1);
		utt_assert(numbers::sum(u) == 0);
		utt_assert(numbers::sum(v) == -(double)v.size());
	}

utt_end();
