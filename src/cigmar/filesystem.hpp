#ifndef CIGMAR_FILESYSTEM
#define CIGMAR_FILESYSTEM

#include <cigmar/classes/Exception.hpp>
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
			/** Normalize separators and remove any useless trailing separator. **/
			String norm(const char* pathname);
			/** Normalize and try to remove relative directories "." and "..". **/
			String resolve(const char* pathname);
			String absolute(const char* pathname);
			String dirname(const char* pathname);
			String basename(const char* pathname);
			String filename(const char* pathname);
			String extension(const char* pathname);
			String relativePath(const String& parent, const String& path);
			template<typename... Args> String join(Args... args);
			bool isDirectory(const char* pathname);
			bool isFile(const char* pathname);
			bool isRoot(const char* pathname);
			bool isRooted(const char* pathname);
			bool isAbsolute(const char* pathname);
			bool isRelative(const char* pathname);
			bool exists(const char* pathname);
			bool hasExtension(const char* pathname);
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
			namespace {
				inline void concatenate(std::ostringstream &o) {}

				template<typename T, typename... Args>
				void concatenate(std::ostringstream &o, T variable, Args... args) {
					std::ostringstream temp;
					temp << variable;
					if (isRooted(temp.str().c_str()))
						throw Exception("Cannot build path with rooted element inside.");
					o << separator << temp.str();
					concatenate(o, args...);
				}

				template<typename T, typename... Args>
				void concatenateFirst(std::ostringstream &o, T variable, Args... args) {
					o << variable;
					concatenate(o, args...);
				}
			}
			template<typename... Args>
			String join(Args... args) {
				std::ostringstream o;
				concatenateFirst(o, args...);
				return String(o.str());
			};
		}
	}
}

#endif
