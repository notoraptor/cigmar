#include <cigmar/time.hpp>
#include <cigmar/classes/Timer.hpp>
#include <cigmar/unittests.hpp>
using namespace cigmar;

utt_begin(TestTime);
utt(test) {
	Timer t;
	time::milliseconds::sleep(500);
	double elapsed = t.nanoseconds();
	utt_smart_assert(elapsed > 4e8, elapsed);
	utt_smart_assert(elapsed < 6e8, elapsed);
}
utt_end();
