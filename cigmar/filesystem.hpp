#ifndef CIGMAR_FILESYSTEM
#define CIGMAR_FILESYSTEM

#include <cigmar/classes/String.hpp>
#ifndef WIN32
#include <dirent.h>
#endif

namespace cigmar {
	namespace sys {
		class PlatformDirent;
		class PlatformDir;
		class Dir;

		class Dirent {
		private:
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
		private:
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
			String norm(const char* pathname);
			/** Normalize and try to remove relative directories "." and "..". **/
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
