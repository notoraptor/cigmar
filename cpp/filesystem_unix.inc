#ifndef WIN32
#include <cstdio>
#include <string>
#include <iostream>
#include <cigmar/std.hpp>
#include <cigmar/filesystem.hpp>
#include <dirent.h>

using std::cerr;
using std::endl;

namespace cigmar {

	namespace sys {

		namespace dir {

			Dir* open(const char* pathname) {
				return opendir(pathname);
			};

			int close(Dir* path) {
				return closedir(path);
			};

			Dirent* read(Dir* path) {
				return readdir(path);
			};

		}

		namespace path {

			const char* const separator = unixSeparator;

			String norm(const char* pathname) {
				// Normalize separators.
				std::string p(pathname);
				std::string twoSeps(separator);
				twoSeps += separator;
				size_t sepLength = strlen(separator);
				std_string_replace_inplace(p, windowsSeparator, separator);
				// Remove trailing separators.
				while (p.rfind(separator) == (p.length() - sepLength))
					p = p.substr(0, p.length() - sepLength);
				// Remove consecutive separators.
				while (p.find(twoSeps) != std::string::npos)
					std_string_replace_inplace(p, twoSeps.c_str(), separator);
				return String(p.c_str());
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

			bool isDirectory(const char* pathname) {
				String normalized(norm(pathname));
				DIR* d = opendir((const char*)normalized);
				bool is = (d != NULL);
				if (is) closedir(d);
				return is;
			};

			bool isFile(const char* pathname) {
				/* TODO: This code may fail if pathname is already opened
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

		const char* nameof(Dirent* entry) {
			return entry->d_name;
		}

		int mkdir(const char* pathname) {
			// http://pubs.opengroup.org/onlinepubs/009695399/functions/mkdir.html
			/*
			The following line should create a directory with
			read/write/search permissions for owner and group, and with
			read/search permissions for others.
			*/
			return mkdir(pathname, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		};

		int rmdir(const char* pathname) {
			return ::rmdir(pathname);
		};

	}

}
#endif
