#include <locale>
#include <chrono>
#include <thread>
#include <cigmar/interfaces/Streamable.hpp>
#include <cigmar/classes/String.hpp>
#include <cigmar/classes/StringView.hpp>
#include <cigmar/init.hpp>
#include <cigmar/numbers.hpp>
#include <video/Video.hpp>
#include <cigmar/classes/HashSet.hpp>
#include <cigmar/file/Lines.hpp>

// #define STRING(a) const char* a = #a

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
	/// Local definitons.
	static std::locale loc;
	namespace video {
		static HashSet<String> supportedExtensions;
		static bool supportedExtensionsInitialized = false;
	}

	/// Global variables.
	last_t LAST;
	CigmarInit cigmar_init;

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

	/// Classes static attributes.
	const char* StringView::empty = "";
	const char* StringView::empty_characters = " \f\n\r\t\v";

	/// Classes static methods.
	char Char::lower(char c) {
		return std::tolower(c, loc);
	}
	char Char::upper(char c) {
		return std::toupper(c, loc);
	}

	/// Namespace definitions.
	namespace numbers::random {
		RNG rng;
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
	namespace video {
		const char* Video::thumbnailExtension = "jpg";
		bool Video::extensionIsSupported(const String& extension) {
			if (!supportedExtensionsInitialized) {
				file::Lines file("res/video/extensions.txt");
				for (String& line: file) {
					line.trim();
					if (line && line[0] != '#') supportedExtensions << line;
				}
				supportedExtensionsInitialized = true;
			}
			return supportedExtensions.contains(extension);
		}
		void Video::collect(const char *dirpath, const std::function<void(Video&&)>& collector) {
			sys::Dir dir(dirpath);
			for (const String pathname: dir) {
				if (pathname != "." && pathname != "..") {
					String path = sys::path::join(dirpath, pathname);
					if (sys::path::isDirectory((const char *) path)) {
						collect((const char *) path, collector);
					} else {
						String extension = sys::path::extension((const char *) pathname);
						if (extensionIsSupported(extension.lower()))
							collector(Video(path));
					}
				}
			}
		}
		void Video::collectPaths(const char *dirpath, const std::function<void(const String&)>& collector) {
			sys::Dir dir(dirpath);
			for (const String pathname: dir) {
				if (pathname != "." && pathname != "..") {
					const String path = sys::path::join(dirpath, pathname);
					if (sys::path::isDirectory((const char *) path)) {
						collectPaths((const char *) path, collector);
					} else {
						String extension = sys::path::extension((const char *) pathname);
						if (cigmar::video::Video::extensionIsSupported(extension.lower()))
							collector(path);
					}
				}
			}
		}
	}
}
