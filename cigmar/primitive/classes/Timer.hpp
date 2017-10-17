#ifndef CIGMAR_TIMER
#define CIGMAR_TIMER

#include <chrono>

namespace cigmar {

struct Timer {
    std::chrono::steady_clock::time_point base;
    void start() {base = std::chrono::steady_clock::now();}
    double milliseconds() {
    	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - base).count();
    }
    double nanoseconds() {
    	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - base).count();
    }
};

}

#endif
