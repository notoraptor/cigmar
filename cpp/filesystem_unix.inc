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

		}

		const char* nameof(Dirent* entry) {
			return entry->d_name;
		}



	}

}
