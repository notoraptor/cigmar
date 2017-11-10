#include <cigmar/unittests.hpp>
#include <cigmar/classes/ArrayList.hpp>
#include <cigmar/classes/Stack.hpp>

using namespace cigmar;

utt_begin(TestStack);

utt(test_Stack) {
	Stack<int> s1;
	utt_assert(!s1);
	utt_assert(s1.size() == 0);
	ArrayList<double> arr = {1, 2, 3, 4};
	Stack<int> s2(arr);
	utt_assert(s2);
	utt_assert(s2.size() == 4);
	utt_assert(*s2 == 4);
	Stack<double> s3 = {1.7, -2.9, 3.36};
	utt_assert(s3);
	utt_assert(s3.size() == 3);
	utt_assert(*s3 == 3.36);
	--s3;
	utt_assert(s3);
	utt_assert(s3.size() == 2);
	utt_assert(*s3 == -2.9);
	--s3;
	utt_assert(s3);
	utt_assert(s3.size() == 1);
	utt_assert(1.7 == *s3);
	--s3;
	utt_assert(!s3);
	utt_assert(s3.size() == 0);
	Stack<size_t> stack;
	stack.push(12).push(27).push(61);
	utt_assert(stack.size() == 3);
	utt_assert(*stack == 61);
	stack.pop();
	utt_assert(stack.size() == 2);
	utt_assert(*stack == 27);
	stack.push(11).push(15).pop().pop().pop();
	utt_assert(*stack == 12);
	utt_assert(stack.front() == 12);
	Stack<size_t> other = {10, 11, 12, 13, 14, 15};
	stack.swap(other);
	utt_assert(stack.size() == 6);
	utt_assert(other.size() == 1);
	utt_assert(*other == 12);
	utt_assert(*stack == 15);
	utt_assert(*stack.begin() == 15);
	stack = arr;
	utt_assert(stack.size() == 4);
	utt_assert(*stack == 4);
	stack = {7, 8, 11, 24, 29};
	utt_assert(stack.size() == 5);
	utt_assert(*stack == 29);
	for (int i = 0; i < 5; ++i) --stack;
	utt_assert(!stack);
	utt_assert(stack.size() == 0);

	stack << 100 << 200 << 300 << 1111 << 400;
	utt_assert(stack.size() == 5);
	utt_assert(*stack == 400);
	--stack;
	utt_assert(*stack == 1111);
	--stack;
	utt_assert(*stack == 300);
	utt_assert(stack.size() == 3);
	stack << arr;
	utt_assert(stack.size() == 3 + 4);
	utt_assert(*stack == 4);
	stack.clear();
	utt_assert(!stack);
	Stack<std::vector<long>> vs;
	std::vector<long> l = {1, -2, 3}, l1 = {4, -5, 6}, l2 = {7, 8, 9, 10};
	vs << l;
	utt_assert(vs.size() == 1);
	vs << l1 << l2;
	utt_assert(vs.size() == 3);
	utt_assert((*vs)[3] == 10);
	vs.pop();
	--vs;
	utt_assert(vs.size() == 1);
	utt_assert((*vs)[1] == -2);
	vs << (std::vector<long>){1, 2, 3, 4};
	vs << (dtypeof(vs)){-1, -2, 3, -4};
}

utt_end();
