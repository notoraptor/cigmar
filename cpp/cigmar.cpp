#include <locale>
#include <chrono>
#include <thread>
#include <cigmar/interfaces/Streamable.hpp>
#include <cigmar/classes/String.hpp>
#include <cigmar/classes/StringView.hpp>
#include <cigmar/init.hpp>
#include <cigmar/numbers.hpp>

/* NB:
 * To have all C++11 thread functionalities available, compiler must be POSIX compliant.
 * Got it from mingw-w64: https://sourceforge.net/projects/mingw-w64/
 * (Default mingw32 is not POSIX for threads).
 * To check POSIX compliance, run `gcc -v`.
 * If you read: "Thread model: Win32", then thread is not well supported, you must see "POSIX".
 * http://www.programering.com/q/MTM5UzNwATg.html
*/

/** Initialization of global symbols and variables. **/

namespace cigmar {
	_LAST LAST;
	CigmarInit cigmar_init;
	namespace numbers {
		RNG rng;
	}
	/** Others definitions. **/
	const char* StringView::empty = "";
	const char* StringView::empty_characters = " \f\n\r\t\v";

	/// Global variables.
	static std::locale loc;
	/// Global functions.
	std::ostream& operator<<(std::ostream& o, byte_t b) {
		return (o << (int)b);
	}
	std::ostream& operator<<(std::ostream& o, ubyte_t b) {
		return (o << (unsigned int)b);
	}
	std::ostream& operator<<(std::ostream& o, const Streamable& s) {
		s.toStream(o);
		return o;
	};
	char Char::lower(char c) {
		return std::tolower(c, loc);
	}
	char Char::upper(char c) {
		return std::toupper(c, loc);
	}
	namespace time {
		namespace nanoseconds {
			void sleep(size_t count) {std::this_thread::sleep_for (std::chrono::nanoseconds(count));};
		}
		namespace microseconds {
			void sleep(size_t count) {std::this_thread::sleep_for (std::chrono::microseconds(count));};
		}
		namespace milliseconds {
			void sleep(size_t count) {std::this_thread::sleep_for (std::chrono::milliseconds(count));};
		}
		namespace seconds {
			void sleep(size_t count) {std::this_thread::sleep_for (std::chrono::seconds(count));};
		}
		namespace minutes {
			void sleep(size_t count) {std::this_thread::sleep_for (std::chrono::minutes(count));};
		}
		namespace hours {
			void sleep(size_t count) {std::this_thread::sleep_for (std::chrono::hours(count));};
		}
	}
}
