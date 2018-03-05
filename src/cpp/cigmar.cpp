#include <locale>
#include <chrono>
#include <thread>
#include <libraries/base64/base64.hpp>
#include <libraries/whirlpool/nessie.h>
#include <cigmar/classes/file/Lines.hpp>
#include <cigmar/classes/TreeMap.hpp>
#include <cigmar/numbers.hpp>
#include <cigmar/unittests.hpp>

/* NB:
 * To have all C++11 thread functionalities available, compiler must be POSIX compliant.
 * Got it from mingw-w64: https://sourceforge.net/projects/mingw-w64/
 * (Default mingw32 is not POSIX for threads). To check POSIX compliance, run `gcc -v`.
 * If you read: "Thread model: Win32", then thread is not well supported, you must see "POSIX".
 * http://www.programering.com/q/MTM5UzNwATg.html
 * TODO: Optimization:  Virtual methods increase class size.
 * Let's try to reduce usage of virtual methods, for ex.  everywhere we could use templates instead of inheritance.
 * */

/* Notes: write a long multi-lines raw string in C++11+:
const char* jstring = R"(
{
"a": true,
"b": 1,
"salue": -2.9,
"hello": "merci"
}
)";
*/

namespace cigmar {
	/// Local definitons.
	#define CIGMAR_SYS_RUN_BUFFER_LENGTH 1025

	/// Global variables.
	/** Instantiation of library initialization class CigmarInit,
	to automatically initialize library before call to main() function. **/
	CigmarInit cigmar_init;

	/// Classes static methods.
	const char* const hex::digits = "0123456789ABCDEF";

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
	namespace characters {
		std::locale loc;
	}
	namespace crypto::hash {
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
			return hex::fromarray(digest, DIGESTBYTES);
		};
	}
	namespace file::text {
		template<typename T>
		String _read(const T& filename) {
			String text;
			file::Lines file(filename);
			for (const String& line : file) text << line;
			return text;
		}
		String read(const char* filename) {
			return _read(filename);
		};
		String read(const std::string& filename) {
			return _read(filename);
		};
		String read(const String& filename) {
			return _read(filename);
		};
	}
	namespace hex {
		static String fromarray(const unsigned char array[], size_t length) {
			String hexString(2 * length, '0');
			for (size_t i = 0; i < length; ++i) {
				hexString[2 * i] = hex::digit(hex::up(array[i]));
				hexString[2 * i + 1] = hex::digit(hex::down(array[i]));
			}
			return hexString;
		}
	}
	namespace numbers::random {
		RNG rng;
	}
	namespace sys {
		namespace path {
			const char* const windowsSeparator = "\\";
			const char* const unixSeparator = "/";
			String resolve(const char* pathname) {
				String normalized = norm(pathname);
				ArrayList<String> levels = normalized.split(separator);
				ArrayList<String> out;
				for(String& level: levels) {
					if (level == ".") {
						// pass
					} else if (level == "..") {
						if (!out || out[LAST] == level) {
							out.add(level);
						} else if (out.size() == 1) {
							if (isRooted(out[0].cstring())) {
								// ERROR (we are at root level, there is no parent).
								return "";
							} else {
								out.remove(out.size() - 1);
							}
						} else {
							out.remove(out.size() - 1);
						}
					} else {
						out.add(level);
					}
				}
				String resolved;
				if (!out) {
					resolved = ".";
				} else {
					resolved = out[0];
					for (size_t i = 1; i < out.size(); ++i)
						resolved << separator << out[i];
				}
				return resolved;
			}
			bool isAbsolute(const char* pathname) {
				if (!isRooted(pathname))
					return false;
				std::string path(pathname);
				std::vector<std::string> specialLevels = {".", ".."};
				for (std::string& specialLevel: specialLevels) {
					std::string unexpectedEnd = path::separator + specialLevel;
					std::string unexpectedIn = unexpectedEnd + path::separator;
					if (path.find(unexpectedIn) != std::string::npos ||
						path.find(unexpectedEnd) == path.length() - unexpectedEnd.length())
						return false;
				}
				return true;
			}
			bool isRelative(const char* pathname) {
				return !isAbsolute(pathname);
			}
			bool exists(const char* pathname) {
				return isFile(pathname) || isDirectory(pathname);
			};
			bool hasExtension(const char* pathname) {
				String p(pathname);
				pos_t posLastPoint = p.lastIndexOf('.');
				if (posLastPoint) {
					pos_t posLastSeparator = p.lastIndexOf(separator);
					return !posLastSeparator || (posLastSeparator < posLastPoint);
				}
				return false;
			};
			String dirname(const char* pathname) {
				String p(pathname);
				if (isRoot(pathname))
					throw Exception("Root path does not have parent directory.");
				if (p == "." || p == "..")
					throw Exception("Consider calling absolute() before dirname() for path ", p);
				pos_t posLastSeparator = p.lastIndexOf(separator);
				return posLastSeparator ? String(p, 0, (size_t)posLastSeparator) : ".";
			};
			String basename(const char* pathname) {
				String p(pathname);
				if (p == "." || p == "..")
					throw Exception("Consider calling absolute() before basename() for path ", p);
				if (isRoot(pathname))
					return pathname;
				pos_t posLastSeparator = p.lastIndexOf(separator);
				return posLastSeparator ? String(p, (size_t)posLastSeparator + 1) : p;
			};
			String filename(const char* pathname) {
				String bn = basename(pathname);
				pos_t posLastPoint = bn.lastIndexOf('.');
				return posLastPoint ? String(bn, 0, (size_t)posLastPoint) : bn;
			};
			String extension(const char* pathname) {
				return hasExtension(pathname) ? String(strchr(pathname, '.') + 1) : "";
			};
			String relative(const String& parent, const String& path) {
				String absoluteParent = sys::path::absolute((const char*)parent);
				String absolutePath = sys::path::absolute((const char*)path);
				if (!sys::path::isDirectory((const char*)absoluteParent))
					throw Exception("Parent must be a directory: ", absoluteParent);
				if (!absoluteParent.endsWith(sys::path::separator))
					absoluteParent << sys::path::separator;
				pos_t posParent = absolutePath.indexOf(absoluteParent);
				if (!posParent || posParent != 0)
					throw Exception("Unable to get relative path against parent path", absoluteParent);
				return absolutePath(absoluteParent.length(), absolutePath.length());
			}
		}

		class FileHandler {
		public:
			typedef int(*FileCleanerFunction)(FILE*);
		private:
			FILE* handle;
			FileCleanerFunction clean;
		public:
			FileHandler(FILE* stream, FileCleanerFunction cleanerFunction):
				handle(stream), clean(cleanerFunction) {}
			~FileHandler() {close();}
			FileHandler(const FileHandler&) = default;
			FileHandler(FileHandler&&) = default;
			FileHandler& operator=(const FileHandler&) = default;
			FileHandler& operator=(FileHandler&&) = default;
			explicit operator bool() const {return handle != NULL;}
			operator FILE*() {return handle;}
			int close() {
				int err = 0;
				if (handle) {
					err = clean(handle);
					handle = NULL;
				}
				return err;
			}
		};

		// Inspired from: https://stackoverflow.com/questions/478898/
		String run(const char* command) {
			String out;
			char buffer[CIGMAR_SYS_RUN_BUFFER_LENGTH];
			FileHandler pipe(popen(command, "r"), pclose);
			if (!pipe)
				throw Exception("cigmar::sys::run(): unable to open stdout. ", strerror(errno));
			while (!feof(pipe)) {
				if (fgets(buffer, CIGMAR_SYS_RUN_BUFFER_LENGTH, pipe))
					out << buffer;
			}
			errno = 0;
			if (pipe.close()) {
				if (errno) {
					throw Exception("cigmar::sys::run(): error closing process: ", strerror(errno));
				} else {
					throw Exception("cigmar::sys::run(): command error (", command ,")");
				}
			}
			return out;
		}
		bool isWindows() {
			String absolutePath = path::absolute(".");
			return absolutePath.length() > 2 && isalpha(absolutePath[0]) && absolutePath[1] == ':' && absolutePath[2] == '\\';
		}
		bool isUnix() {
			String absolutePath = path::absolute(".");
			return absolutePath && absolutePath[0] == '/';
		}
	}
	namespace tests {
		template<typename T> class Dynamic {
		public:
			T* pointer;
		public:
			~Dynamic() {
				delete pointer;
			}
		};
		typedef TreeMap<String, UttMethod> TestList;
		static Dynamic<TestList> tests;
		static void init() {
			static bool initialized = false;
			if (!initialized) {
				tests.pointer = new TestList();
				initialized = true;
			}
		}
		UttRecorder::UttRecorder(UttMethod method, const char *className, const char *methodName) noexcept {
			init();
			TestList& treemap = *tests.pointer;
			String s = String::write(className, ".", methodName);
			treemap[s] = method;
		}
		void run() {
			init();
			TestList& treemap = *tests.pointer;
			Timer timer;
			timer.start();
			for (TestList::iterator_t it = treemap.begin(); it != treemap.end(); ++it) {
				// catch
				try {
					// turn
					try {
						std::cerr << it->first << " ...";
						it->second();
						std::cerr << " ok" << std::endl;
					} catch (std::string &s) {
						std::ostringstream os;
						std::cerr << " ERROR" << std::endl;
						os << "ERROR: " << it->first << std::endl << s;
						throw os.str();
					} catch (...) {
						std::cerr << " ERROR" << std::endl << std::endl;
						std::cerr << "ERROR: " << it->first << std::endl;
						throw;
					}
				} catch (std::string& s) {
					std::cerr << std::endl << s << std::endl;
					exit(EXIT_FAILURE);
				} catch(std::exception& e) {
					std::cerr << std::endl << e.what() << std::endl;
					exit(EXIT_FAILURE);
				} catch (...) {
					std::cerr << std::endl << "(unknown exception)" << std::endl;
					exit(EXIT_FAILURE);
				}
			}
			double elapsedSeconds = timer.microseconds() * 1e-6;
			size_t testsCount = treemap.size();
			std::cerr << std::endl << "Ran " << testsCount << " test" << (testsCount == 1 ? "" : "s");
			std::cerr << " in " << elapsedSeconds << 's' << std::endl;
			std::cerr << std::endl << "OK" << std::endl;
		}
	}
	namespace time {
		namespace nanoseconds {
			void sleep(size_t count) {std::this_thread::sleep_for (std::chrono::nanoseconds(count));}
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
