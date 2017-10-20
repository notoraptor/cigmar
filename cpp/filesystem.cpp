#include <string>
#include <vector>
#include <cigmar/classes/String.hpp>

#ifdef WIN32
#include <cpp/filesystem_windows.inc>
#else
#include <cpp/filesystem_unix.inc>
#endif

/* Common code. */
namespace cigmar {
	namespace sys {
		namespace path {
			const char* const windowsSeparator = "\\";
			const char* const unixSeparator = "/";
			String resolve(const char* pathname) {
				std::string normalized = norm(pathname).cstring();
				std::vector<std::string> levels;
				std::vector<std::string> out;
				std_string_split(normalized, separator, levels);
				for(std::string& level: levels) {
					if (level == ".") {
						// pass
					} else if (level == "..") {
						if (out.empty() || out.back() == level) {
							out.push_back(level);
						} else if (out.size() == 1) {
							if (isRooted(out[0].c_str())) {
								// ERROR (we are at root level, there is no parent).
								return "";
							} else {
								out.pop_back();
							}
						} else {
							out.pop_back();
						}
					} else {
						out.push_back(level);
					}
				}
				std::string resolved;
				if (out.empty()) {
					resolved = ".";
				} else {
					resolved = out[0];
					for (size_t i = 1; i < out.size(); ++i)
						(resolved += separator) += out[i];
				}
				return String(resolved.c_str());
			}
			bool isFile(const char* pathname) {
				/* fopen with mode "rb" seems to open directories on Ubuntu.
				 * Adding mode "+" seems to fix it: https://stackoverflow.com/questions/42876210/c-fopen-opening-directories */
				String normalized(norm(pathname));
				FILE* f = fopen((const char*)normalized, "rb+");
				bool is = (f != NULL);
				if (is) fclose(f);
				return is;
			};
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
		}
	}
}
