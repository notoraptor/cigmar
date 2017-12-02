#include <locale>
#include <chrono>
#include <thread>
#include <cigmar/interfaces/Streamable.hpp>
#include <cigmar/classes/String.hpp>
#include <cigmar/classes/StringView.hpp>
#include <cigmar/init.hpp>
#include <cigmar/numbers.hpp>
#include <cigmar/classes/HashSet.hpp>
#include <cigmar/file/Lines.hpp>
#include <libraries/base64/base64.hpp>
#include <libraries/whirlpool/nessie.h>

/* NB:
 * To have all C++11 thread functionalities available, compiler must be POSIX compliant.
 * Got it from mingw-w64: https://sourceforge.net/projects/mingw-w64/
 * (Default mingw32 is not POSIX for threads).
 * To check POSIX compliance, run `gcc -v`.
 * If you read: "Thread model: Win32", then thread is not well supported, you must see "POSIX".
 * http://www.programering.com/q/MTM5UzNwATg.html
*/

namespace cigmar {
	/// Local definitons.
	static std::locale loc;
	static const char* const hexDigits = "0123456789ABCDEF";

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
	namespace base64 {
		String encode(const String& in) {
			String out;
			Base64::Encode(in.cppstring(), out.cppstring());
			return out;
		}
		String decode(const String& in) {
			String out;
			Base64::Decode(in.cppstring(), out.cppstring());
			return out;
		}
		namespace bytes {
			String encode(const byte_t* in, size_t len) {
				ArrayList<char> out(Base64::EncodedLength(len) + 1, '\0');
				Base64::Encode((const char*)in, len, (char*)out, out.size());
				return String((const char*)out);
			};
			ArrayList<byte_t> decode(const String& in) {
				ArrayList<byte_t> out((size_t)Base64::DecodedLength((const char*)in, in.length()));
				Base64::Decode((const char*)in, in.length(), (char*)(byte_t*)out, out.size());
				return out;
			};
		}
	}
	namespace crypto::hash {
		// TODO: Move it in cigmar hex module.
		static String toHex(const u8 array[], size_t length) {
			String hexString(2 * length, '0');
			for (size_t i = 0; i < length; ++i) {
				hexString[2 * i] = hexDigits[array[i] >> 4];
				hexString[2 * i + 1] = hexDigits[array[i] & ((1 << 4) - 1)];
			}
			return hexString;
		}
		String whirlpool(const String& in) {
			if (in.length() > std::numeric_limits<unsigned long>::max() / 8) {
				throw Exception("Current implementation of Whirlpool hashing algorithm supports maximum input size ",
				                std::numeric_limits<unsigned long>::max() / 8, ", got ", in.length(), '.');
			}
			u8 digest[DIGESTBYTES];
			struct NESSIEstruct w;
			NESSIEinit(&w);
			NESSIEadd((const unsigned char*)in.cstring(), 8 * (unsigned long)in.length(), &w);
			NESSIEfinalize(&w, digest);
			return toHex(digest, DIGESTBYTES);
		};
	}
	namespace file::text {
		String read(const char* filename) {
			String text;
			file::Lines file(filename);
			for (const String& line : file) {
				text << line;
			}
			return text;
		};
		String read(const std::string& filename){
			String text;
			file::Lines file(filename);
			for (const String& line : file) text << line;
			return text;
		};
		String read(const String& filename) {
			String text;
			file::Lines file(filename);
			for (const String& line : file) text << line;
			return text;
		};
	}
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
}
