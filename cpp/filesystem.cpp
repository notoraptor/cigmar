#include <string>
#include <vector>
#include <cigmar/classes/String.hpp>
#include <cigmar/classes/Exception.hpp>

#ifdef WIN32
#define popen _popen
#define pclose _pclose
#include <cpp/filesystem_windows.inc>
#else
#include <cstdio>
#include <cpp/filesystem_unix.inc>
#endif

#define BUFFER_LENGTH 1025

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
			char buffer[BUFFER_LENGTH];
			FileHandler pipe(popen(command, "r"), pclose);
			if (!pipe)
				throw Exception("cigmar::sys::run(): unable to open stdout. ", strerror(errno));
			while (!feof(pipe)) {
				if (fgets(buffer, BUFFER_LENGTH, pipe))
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
	}
}
