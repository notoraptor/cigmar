#include <locale>
#include <chrono>
#include <thread>
#include <cigmar/interfaces/Streamable.hpp>
#include <cigmar/classes/String.hpp>
#include <cigmar/classes/StringView.hpp>
#include <cigmar/init.hpp>
#include <cigmar/numbers.hpp>
#include <video/Video.hpp>
#include <video/utils.hpp>

#define STRING(a) const char* a = #a

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
	namespace numbers {
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

		namespace strings {
			STRING(id);
			STRING(path);
			STRING(dateAddedMicroseconds);
			STRING(format);
			STRING(frameRate);
			STRING(sampleRate);
			STRING(size);
			STRING(duration);
			STRING(audioCodec);
			STRING(videoCodec);
			STRING(width);
			STRING(height);
			STRING(notation);
			STRING(thumbnail);
		}
		const char* TsvVideoRecorder::fields[] = {
				strings::id,
				strings::format,
				strings::frameRate,
				strings::sampleRate,
				strings::audioCodec,
				strings::videoCodec,
				strings::size,
				strings::duration,
				strings::width,
				strings::height,
				strings::notation,
				strings::thumbnail,
				strings::dateAddedMicroseconds,
				strings::path
		};
		const size_t TsvVideoRecorder::fieldsCount = 14;
		void TsvVideoRecorder::header(std::ostream &o) {
			o << strings::id;
			o << '\t' << strings::format;
			o << '\t' << strings::frameRate;
			o << '\t' << strings::sampleRate;
			o << '\t' << strings::audioCodec;
			o << '\t' << strings::videoCodec;
			o << '\t' << strings::size;
			o << '\t' << strings::duration;
			o << '\t' << strings::width;
			o << '\t' << strings::height;
			o << '\t' << strings::notation;
			o << '\t' << strings::thumbnail;
			o << '\t' << strings::dateAddedMicroseconds;
			o << '\t' << strings::path;
		}
	}
}
