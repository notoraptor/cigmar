#ifndef WIN32
#include <cstdio>
#include <string>
#include <iostream>
#include <cigmar/filesystem.hpp>
#include <dirent.h>
#include <unistd.h> // for rmdir()
#include <sys/stat.h> // for mkdir() and constants like S_IXOTH.
#include <cassert>

using std::cerr;
using std::endl;

namespace cigmar {

	namespace sys {

		class PlatformDir {
		public:
			String internal;
		};

		class PlatformDirent {
		private:
			DIR* directory;
			struct dirent* entry;
			friend class Dirent;
			explicit PlatformDirent(const String& internalPath):
					directory(NULL), entry(NULL) {
				directory = opendir((const char*)internalPath);
				assert(directory != NULL);
				entry = readdir(directory);
				assert(entry != NULL);
			};
		public:
			~PlatformDirent() {
				closedir(directory);
			}
		};

		Dirent::Dirent() {
			handler = nullptr;
		}
		Dirent::Dirent(Dir& dir) {
			handler = new PlatformDirent(dir.handler->internal);
		}
		Dirent::~Dirent() {
			delete handler;
		}
		Dirent& Dirent::operator++() {
			if (handler) {
				handler->entry = readdir(handler->directory);
				if (handler->entry == NULL) {
					delete handler;
					handler = nullptr;
				}
			}
			return *this;
		}
		bool Dirent::operator==(const Dirent& other) const {
			return handler == other.handler;
		}
		bool Dirent::operator!=(const Dirent& other) const {
			return handler != other.handler;
		}
		const char* Dirent::operator*() const {
			return handler ? handler->entry->d_name : nullptr;
		}

		Dir::Dir(const char *pathname): handler(nullptr) {
			DIR* directory = opendir(pathname);
			if (directory == NULL)
				throw Exception("cigmar::sys::Dir: error opening directory ", pathname);
			closedir(directory);
			handler = new PlatformDir();
			handler->internal = pathname;
		}
		Dir::~Dir() {
			delete handler;
		}
		Dirent Dir::begin() {
			return Dirent(*this);
		}
		Dirent Dir::end() {
			return Dirent();
		}

		namespace path {

			const char* const separator = unixSeparator;

			String norm(const char* pathname) {
				// Normalize separators.
				String p(pathname);
				String twoSeps = String::write(separator, separator);
				size_t sepLength = strlen(separator);
				p.replace(windowsSeparator, separator);
				// Remove trailing separators.
				while (p.endsWith(separator))
					p.remove(p.length() - sepLength, sepLength);
				// Remove consecutive separators.
				while (p.contains(twoSeps))
					p.replace(twoSeps, separator);
				return p;
			};

			String absolute(const char* pathname) {
				String normalized(norm(pathname));
				char out[PATH_MAX + 1];
				const char* result = realpath((const char*)normalized, out);
				return result == out ? String(out) : "";
			};

			bool isRooted(const char* pathname) {
				return pathname[0] == '/';
			}

			bool isRoot(const char* pathname) {
				return isRooted(pathname) && pathname[1] == '\0';
			}

			bool isDirectory(const char* pathname) {
				String normalized(norm(pathname));
				DIR* d = opendir((const char*)normalized);
				bool is = (d != NULL);
				if (is) closedir(d);
				return is;
			};

			bool isFile(const char* pathname) {
				/* TODO: This code may fail if pathname is already opened.
				 * (e.g. if pathname points to this actual program).
				 * This need to be tested on Linux. */
				/* fopen with mode "rb" seems to open directories on Ubuntu.
				 * Adding mode "+" seems to fix it:
				 * https://stackoverflow.com/questions/42876210/c-fopen-opening-directories */
				String normalized(norm(pathname));
				FILE* f = fopen((const char*)normalized, "rb+");
				bool is = (f != NULL);
				if (is) fclose(f);
				return is;
			};
		}

		int makeDirectory(const char* pathname) {
			// http://pubs.opengroup.org/onlinepubs/009695399/functions/mkdir.html
			/*
			The following line should create a directory with
			read/write/search permissions for owner and group, and with
			read/search permissions for others.
			*/
			return mkdir(pathname, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		};

		int removeDirectory(const char* pathname) {
			return rmdir(pathname);
		};

	}

}
#endif
