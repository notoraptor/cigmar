/* Reference code for Windows: https://msdn.microsoft.com/en-us/library/windows/desktop/aa365200(v=vs.85).aspx */
#ifdef WIN32
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <cigmar/std.hpp>
#include <cigmar/filesystem.hpp>
#include <windows.h>

using std::cerr;
using std::endl;

namespace cigmar {

	namespace windows {
		const std::string prefix = "\\\\?\\";
		const std::string suffix = "\\*";
		void formatPrefix(std::string& s, bool skipSpecialFolders = false) {
			if (skipSpecialFolders && (s == "." || s == ".."))
				return;
			if (s.find(prefix) != 0)
				s.insert(0, prefix);
		}
		void formatSuffix(std::string& s) {
			if (s.length() >= suffix.length() && s.rfind(suffix) != (s.length() - suffix.length()))
				s += suffix;
		}
		void removePrefix(std::string& s) {
			if (s.find(prefix) == 0)
				s = s.substr(prefix.length());
		}
		void removeSuffix(std::string& s) {
			if (s.length() > suffix.length()) {
				size_t expectedSuffixPos = s.length() - suffix.length();
				if (s.rfind(suffix) == expectedSuffixPos)
					s = s.substr(0, expectedSuffixPos);
			}
		}
		void formatIfNeeded(std::string& path) {
			if (path != "." && path != "..") {
				formatPrefix(path);
				formatSuffix(path);
			}
		}
	}

	namespace sys {

		class Dirent {
		private:
			WIN32_FIND_DATA ffd;
			friend class Dir;
			friend const char* nameof(Dirent* entry);
		public:
			bool isDirectory() const {return ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;}
			bool isFile() const {return !this->isDirectory();}
		};

		class Dir {
		private:
			Dirent entry;
			HANDLE handle;
			bool alreadyRead;
			bool valid;
		public:
			Dir(const char* filename) {
				std::string formatedFilename(sys::path::absolute(filename).cstring());
				windows::formatPrefix(formatedFilename);
				windows::formatSuffix(formatedFilename);
				handle = FindFirstFile(formatedFilename.c_str(), &entry.ffd);
				valid = (handle != INVALID_HANDLE_VALUE);
				alreadyRead = false;
			}
			~Dir() {
				if (handle != INVALID_HANDLE_VALUE)
					FindClose(handle);
			}
			operator bool() const {return valid;}
			Dirent* next() {
				if (valid) {
					if (alreadyRead) {
						valid = (FindNextFile(handle, &entry.ffd) != 0);
						/*
						dwError = GetLastError();
						if (dwError != ERROR_NO_MORE_FILES) {
							cout << "final error." << endl;
							// What to do ?
						}
						*/
					} else {
						alreadyRead = true;
					}
				}
				return valid ? &entry : NULL;
			}
		};

		namespace dir {

			Dir* open(const char* pathname) {
				Dir* dir = new Dir(pathname);
				if (!(*dir)) {
					delete dir;
					dir = NULL;
				}
				return dir;
			};

			int close(Dir* path) {
				delete path;
				return 0;
			};

			Dirent* read(Dir* path) {
				return path->next();
			};

		}

		namespace path {

			// const char* const separator;
			const char* const separator = windowsSeparator;

			String norm(const char* pathname) {
				// Normalize separators.
				std::string p(pathname);
				std::string twoSeps(separator);
				twoSeps += separator;
				size_t sepLength = strlen(separator);
				std_string_replace_inplace(p, unixSeparator, separator);
				windows::removePrefix(p);
				windows::removeSuffix(p);
				// Remove separators at start (only on Windows).
				while (p.find(separator) == 0)
					p = p.substr(sepLength);
				// Remove trailing separators.
				while (p.rfind(separator) == (p.length() - sepLength))
					p = p.substr(0, p.length() - sepLength);
				// Remove consecutive separators.
				while (p.find(twoSeps) != std::string::npos)
					std_string_replace_inplace(p, twoSeps.c_str(), separator);
				return String(p.c_str());
			};

			String absolute(const char* pathname) {
				if (isAbsolute(pathname))
					return String(pathname);
				String normalized = sys::path::resolve(pathname);
				if (isAbsolute(normalized.cstring()))
					return normalized;
				std::string normalizedPath(normalized.cstring());
				size_t posFirstSeparator = normalizedPath.find(separator);
				std::string parent;
				size_t fullPathLength;
				if(posFirstSeparator == std::string::npos) {
					fullPathLength = GetFullPathName(normalizedPath.c_str(), 0, NULL, NULL);
				} else {
					parent = normalizedPath.substr(0, posFirstSeparator);
					fullPathLength = GetFullPathName(parent.c_str(), 0, NULL, NULL);
				}
				if (fullPathLength > 0) {
					char* buffer = new char[fullPathLength];
					std::string* ptr = parent.empty() ? &normalizedPath : &parent;
					size_t writtenLength = GetFullPathName(ptr->c_str(), fullPathLength, buffer, NULL);
					if (writtenLength == fullPathLength - 1) {
						std::string s = buffer;
						if (!parent.empty()) {
							s += normalizedPath.substr(posFirstSeparator);
						}
						delete[] buffer;
						return String(s.c_str());
					}
				}
				return "";
			};

			bool isRooted(const char* pathname) {
				return isalpha(pathname[0]) && pathname[1] == ':' && pathname[2] == '\\';
			}

			bool isDirectory(const char* pathname) {
				Dir d(pathname);
				return d && d.next()->isDirectory();
			};

			bool isFile(const char* pathname) {
				std::string formatedFilename((const char*)sys::path::absolute(pathname));
				windows::formatPrefix(formatedFilename);
				DWORD attributes = GetFileAttributes(formatedFilename.c_str());
				return (attributes == INVALID_FILE_ATTRIBUTES) ?
				       false :
				       !(attributes & FILE_ATTRIBUTE_DIRECTORY);
			}

		}

		const char* nameof(Dirent* entry) {
			return entry->ffd.cFileName;
		}

	}

}
#endif
