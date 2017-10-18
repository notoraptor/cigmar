#ifndef CIGMAR_TIME
#define CIGMAR_TIME

#include <cstddef>

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
}

#endif
