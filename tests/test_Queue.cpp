#include <cigmar/unittests.hpp>
#include <cigmar/primitive/classes/Queue.hpp>
using namespace cigmar;

utt_begin(TestQueue);

utt(test_Queue) {
	Queue<int> s1;
	utt_assert(!s1);
	utt_assert(s1.size() == 0);
	ArrayList<double> arr = {1, 2, 3, 4};
	Queue<int> s2(arr);
	utt_assert(s2);
	utt_assert(s2.size() == 4);
	utt_assert(*s2 == 1);
	Queue<double> s3 = {1.7, -2.9, 3.36};
	utt_assert(s3);
	utt_assert(s3.size() == 3);
	utt_assert(*s3 == 1.7);
	++s3;
	utt_assert(s3);
	utt_assert(s3.size() == 2);
	utt_assert(*s3 == -2.9);
	++s3;
	utt_assert(s3);
	utt_assert(s3.size() == 1);
	utt_assert(3.36 == *s3);
	++s3;
	utt_assert(!s3);
	utt_assert(s3.size() == 0);
	Queue<size_t> queue;
	queue.push(12).push(27).push(61);
	utt_assert(queue.size() == 3);
	utt_assert(*queue == 12);
	queue.pop();
	utt_assert(queue.size() == 2);
	utt_assert(*queue == 27);
	queue.push(11).push(15).pop().pop().pop();
	utt_assert(*queue == 15);
	utt_assert(queue.front() == 15);
	Queue<size_t> other = {10, 11, 12, 13, 14, 15};
	queue.swap(other);
	utt_assert(queue.size() == 6);
	utt_assert(other.size() == 1);
	utt_assert(*other == 15);
	utt_assert(*queue == 10);
	utt_assert(*queue.begin() == 10);
	queue = arr;
	utt_assert(queue.size() == 4);
	utt_assert(*queue == 1);
	queue = {7, 8, 11, 24, 29};
	utt_assert(queue.size() == 5);
	utt_assert(*queue == 7);
	for (int i = 0; i < 5; ++i) ++queue;
	utt_assert(!queue);
	utt_assert(queue.size() == 0);

	queue << 100 << 200 << 300 << 1111 << 400;
	utt_assert(queue.size() == 5);
	utt_assert(*queue == 100);
	++queue;
	utt_assert(*queue == 200);
	++queue;
	utt_assert(*queue == 300);
	utt_assert(queue.size() == 3);
	queue << arr;
	utt_assert(queue.size() == 3 + 4);
	utt_assert(*queue == 300);
	queue.clear();
	utt_assert(!queue);
	Queue<std::vector<long>> vs;
	std::vector<long> l = {1, -2, 3}, l1 = {4, -5, 6}, l2 = {7, 8, 9, 10};
	vs << l;
	utt_assert(vs.size() == 1);
	vs << l1 << l2;
	utt_assert(vs.size() == 3);
	utt_assert((*vs)[1] == -2);
	vs.pop();
	++vs;
	utt_assert(vs.size() == 1);
	utt_assert((*vs)[1] == 8);
	vs << (std::vector<long>){1, 2, 3, 4};
	vs << (dtypeof(vs)){-1, -2, 3, -4};
}

utt_end();
