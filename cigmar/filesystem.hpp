#ifndef CIGMAR_FILESYSTEM
#define CIGMAR_FILESYSTEM

#include <cigmar/classes/String.hpp>
#ifndef WIN32
#include <dirent.h>
#endif

namespace cigmar {
	namespace sys {
		#ifdef WIN32
		class Dir;
		class Dirent;
		#else
		typedef DIR Dir;
		typedef struct dirent Dirent;
		#endif

		namespace dir {
			Dir* open(const char* pathname);
			int close(Dir* path);
			Dirent* read(Dir* path);
		}

		namespace path {
			extern const char* const windowsSeparator;
			extern const char* const unixSeparator;
			extern const char* const separator;
			String norm(const char* pathname);
			String resolve(const char* pathname);
			String absolute(const char* pathname);
			template<typename... Args> String join(Args... args);
			bool isDirectory(const char* pathname);
			bool isFile(const char* pathname);
			bool isRooted(const char* pathname);
			bool isAbsolute(const char* pathname);
			bool isRelative(const char* pathname);
			bool exists(const char* pathname);
		}
		const char* nameof(Dirent* entry);

	}
}

namespace cigmar {
	namespace sys {
		namespace path {
			inline void concatenate(std::ostringstream& o) {}
			template<typename T, typename... Args>
			void concatenate(std::ostringstream& o, T variable, Args... args) {
				if (!o.str().empty())
					o << separator;
				o << variable;
				concatenate(o, args...);
			}
			template<typename... Args>
			String join(Args... args) {
				std::ostringstream o;
				concatenate(o, args...);
				return String(o.str());
			};
		}
	}
}

#endif
