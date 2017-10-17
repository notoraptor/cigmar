#include <iostream>
#include <cigmar/utils.hpp>
using std::cout;
using std::endl;
using namespace cigmar;

utt_begin(TestCigmar);

utt(test_sets) {
	TreeSet<int> ts = {1, 2, 1, 1, 7, -1, 5, -1, 1};
	HashSet<int> hs = {1, 2, 1, 1, 7, -1, 5, -1, 1};
	utt_assert(ts && hs);
	utt_assert(ts.size() == hs.size());
	for (int i: ts) utt_assert(hs.contains(i));
	for (const int& i: hs) utt_assert(ts.contains(i));
}

utt_end();

int main() {
	runTests();
	return 0;
}
