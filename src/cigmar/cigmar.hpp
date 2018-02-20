//
// Created by notoraptor on 31/10/2017.
//

#ifndef CIGMAR_BASE64_HPP
#define CIGMAR_BASE64_HPP

#include <cstddef>
#include <chrono>
#include <cigmar/classes/String.hpp>
#include <cigmar/interfaces/Streamable.hpp>
#include <cigmar/classes/exception/Exception.hpp>
#include <cigmar/classes/String.hpp>
#include <cigmar/symbols.hpp>

#ifndef WIN32
#include <dirent.h>
#else
#define popen _popen
#define pclose _pclose
#endif

namespace cigmar {

	namespace base64 {
		String encode(const String& in);
		String decode(const String& in);
		namespace bytes {
			String encode(const byte_t* in, size_t len);
			ArrayList<byte_t> decode(const String& in);
		}
	}

	namespace crypto::hash {
		String whirlpool(const String& in);
	}

	struct CigmarInit {
		CigmarInit() noexcept {
			std::set_terminate(CigmarInit::terminate);
		}
		static void terminate() {
			std::exception_ptr current_exception = std::current_exception();
			if (current_exception) {
				try {
					std::rethrow_exception(current_exception);
				} catch (std::exception& ex) {
					std::cerr << "Exception not caught: " << ex.what() << std::endl;
				}
			}
			std::cerr << "Unexpected program end." << std::endl;
			exit(EXIT_FAILURE);
		}
	};

	extern CigmarInit cigmar_init;
	/**< Instantiation of library initialization class CigmarInit,
	to automatically initialize library before call to main() function.
	**/

	namespace file::text {
		String read(const char* filename);
		String read(const std::string& filename);
		String read(const String& filename);
	}

	struct hex {
		static const char* const digits;
		static unsigned int up(unsigned char c) {
			return c >> 4;
		}
		static unsigned int down(unsigned char c) {
			return c & (((unsigned int) 1 << 4) - 1);
		}
		static char digit(unsigned int hexval) {
			return digits[hexval];
		}
		template<typename T>
		static String encode(T value, bool trimFirstZeroPairs = true) {
			static_assert(std::is_integral<T>{}, "cigmar::hex::encode() expects an integral type.");
			size_t n_bytes = sizeof(value);
			String out(2 * n_bytes, '0');
			for (size_t i = 0; i < n_bytes; ++i) {
				unsigned char c = value & ((1 << 8) - 1);
				out[2 * n_bytes - 2 - 2 * i] = digit(up(c));
				out[2 * n_bytes - 1 - 2 * i] = digit(down(c));
				value = (value >> 8);
			}
			if (trimFirstZeroPairs) {
				size_t first_zeros = out.leftTrimmable("0");
				if (first_zeros) {
					if (first_zeros == out.length())
						out = "00";
					else if (first_zeros % 2 == 0)
						out.trimLeft("0");
					else
						out = String(out, first_zeros - 1);
				}
			}
			return out;
		}
	};

	namespace sys {
		template<typename... Args> void println(Args&&... args) {
			printFirstElement(std::cout, std::forward<Args>(args)...);
			std::cout << std::endl;
		};
		template<typename... Args> void print(Args&&... args) {
			printFirstElement(std::cout, std::forward<Args>(args)...);
		};
		template<typename... Args> void writeln(Args&&... args) {
			writeElement(std::cout, std::forward<Args>(args)...);
			std::cout << std::endl;
		};
		template<typename... Args> void write(Args&&... args) {
			writeElement(std::cout, std::forward<Args>(args)...);
		};
		namespace err {
			template<typename... Args> void println(Args&&... args) {
				printFirstElement(std::cerr, std::forward<Args>(args)...);
				std::cerr << std::endl;
			};
			template<typename... Args> void print(Args&&... args) {
				printFirstElement(std::cerr, std::forward<Args>(args)...);
			};
			template<typename... Args> void writeln(Args&&... args) {
				writeElement(std::cerr, std::forward<Args>(args)...);
				std::cerr << std::endl;
			};
			template<typename... Args> void write(Args&&... args) {
				writeElement(std::cerr, std::forward<Args>(args)...);
			};
		}

		class PlatformDirent;
		class PlatformDir;
		class Dir;

		class Dirent {
			PlatformDirent* handler;
		public:
			Dirent();
			explicit Dirent(Dir& dir);
			~Dirent();
			Dirent& operator++();
			bool operator==(const Dirent& other) const;
			bool operator!=(const Dirent& other) const;
			const char* operator*() const;
		};

		class Dir {
			PlatformDir* handler;
			friend class Dirent;
		public:
			explicit Dir(const char* pathname);
			~Dir();
			Dirent begin();
			Dirent end();
		};

		namespace path {
			extern const char* const windowsSeparator;
			extern const char* const unixSeparator;
			extern const char* const separator;
			/** Normalize separators and remove any useless trailing separator. **/
			String norm(const char* pathname);
			/** Normalize and try to remove relative directories "." and "..". **/
			String resolve(const char* pathname);
			String absolute(const char* pathname);
			String dirname(const char* pathname);
			String basename(const char* pathname);
			String filename(const char* pathname);
			String extension(const char* pathname);
			String relative(const String& parent, const String& path);
			bool isDirectory(const char* pathname);
			bool isFile(const char* pathname);
			bool isRoot(const char* pathname);
			bool isRooted(const char* pathname);
			bool isAbsolute(const char* pathname);
			bool isRelative(const char* pathname);
			bool exists(const char* pathname);
			bool hasExtension(const char* pathname);
			template<typename... Args> String join(Args... args) {
				std::ostringstream o;
				concatenateFirst(o, args...);
				return String(o.str());
			};
		}

		String run(const char* command);
		int makeDirectory(const char* pathname);
		int removeDirectory(const char* pathname);
		/**< Remove empty directory. **/
		bool isWindows();
		bool isUnix();
	}

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



#endif //CIGMAR_BASE64_HPP_HPP
