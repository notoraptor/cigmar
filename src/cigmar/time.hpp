#ifndef CIGMAR_TIME
#define CIGMAR_TIME

#include <cstddef>
#include <chrono>

namespace cigmar {

	namespace time {
		namespace nanoseconds {
			void sleep(size_t count);
		}
		namespace microseconds {
			void sleep(size_t count);
		}
		namespace milliseconds {
			void sleep(size_t count);
		}
		namespace seconds {
			void sleep(size_t count);
		}
		namespace minutes {
			void sleep(size_t count);
		}
		namespace hours {
			void sleep(size_t count);
		}
	}

	struct Timer {
		std::chrono::steady_clock::time_point base;
		Timer() {start();}
		void start() {base = std::chrono::steady_clock::now();}
		double nanoseconds() {
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - base).count();
		}
		double microseconds() {
			return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - base).count();
		}
		double milliseconds() {
			return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - base).count();
		}
		double seconds() {
			return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - base).count();
		}
	};

}

#endif
