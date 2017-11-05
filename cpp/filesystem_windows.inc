/* Reference code for Windows: https://msdn.microsoft.com/en-us/library/windows/desktop/aa365200(v=vs.85).aspx */
#ifdef WIN32
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <cigmar/std.hpp>
#include <cigmar/filesystem.hpp>
#include <cigmar/classes/Exception.hpp>
#include <windows.h>
#include <direct.h>
#include <cassert>

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

		class PlatformDir {
		public:
			String internal;
		};

		class PlatformDirent {
		private:
			HANDLE handle;
			WIN32_FIND_DATA ffd;
			friend class Dirent;
			explicit PlatformDirent(const String& internalPath):
					handle(INVALID_HANDLE_VALUE) {
				handle = FindFirstFile(internalPath.cstring(), &ffd);
				assert(handle != INVALID_HANDLE_VALUE);
			};
		public:
			~PlatformDirent() {
				FindClose(handle);
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
				if (FindNextFile(handler->handle, &handler->ffd) == 0) {
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
			return handler ? handler->ffd.cFileName : nullptr;
		}

		Dir::Dir(const char *pathname): handler(nullptr) {
			String formatedFilename = sys::path::absolute(pathname);
			windows::formatPrefix(formatedFilename.cppstring());
			windows::formatSuffix(formatedFilename.cppstring());
			WIN32_FIND_DATA ffd;
			HANDLE handle = FindFirstFile(formatedFilename.cstring(), &ffd);
			if (handle == INVALID_HANDLE_VALUE)
				throw Exception("cigmar::sys::Dir: error opening directory: ", pathname);
			FindClose(handle);
			if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				throw Exception("cigmar::sys::Dir: not a directory: ", pathname);
			}
			handler = new PlatformDir();
			handler->internal = formatedFilename;
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
				std::string& normalizedPath = normalized.cppstring();
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
				try {
					Dir d(pathname);
					return true;
				} catch (...) {
					return false;
				}
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

		int makeDirectory(const char* pathname) {
			String formated = sys::path::absolute(pathname);
			windows::formatPrefix(formated.cppstring());
			return _mkdir(formated.cstring());
		};

		int removeDirectory(const char* pathname) {
			String formated = sys::path::absolute(pathname);
			windows::formatPrefix(formated.cppstring());
			return _rmdir(formated.cstring());
		};
	}

}
#endif
