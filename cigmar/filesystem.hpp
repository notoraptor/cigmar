#ifndef CIGMAR_FILESYSTEM
#define CIGMAR_FILESYSTEM

#include <cigmar/classes/String.hpp>
#ifndef WIN32
#include <dirent.h>
#endif

namespace cigmar {
	namespace sys {
		class _PlatformDirent;
		class _PlatformDir;
		class _Dir;

		class _Dirent {
		private:
			_PlatformDirent* handler;
		public:
			_Dirent();
			explicit _Dirent(_Dir& dir);
			~_Dirent();
			_Dirent& operator++();
			bool operator==(const _Dirent& other) const;
			bool operator!=(const _Dirent& other) const;
			const char* operator*() const;
		};

		class _Dir {
		private:
			_PlatformDir* handler;
			friend class _Dirent;
		public:
			explicit _Dir(const char* pathname);
			~_Dir();
			_Dirent begin();
			_Dirent end();
		};

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
			/** Try to remove relative directories "." and "..". **/
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

		String run(const char* command);

		int makeDirectory(const char* pathname);
		int removeDirectory(const char* pathname);
		/**< Remove empty directory. **/

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
