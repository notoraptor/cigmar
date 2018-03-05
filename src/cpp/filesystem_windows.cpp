/* Reference code for Windows: https://msdn.microsoft.com/en-us/library/windows/desktop/aa365200(v=vs.85).aspx */
#ifdef WIN32
// #include <cstdio>
#include <cassert>
#include <string>
#include <direct.h>
#include <windows.h>
#include <cigmar/classes/String.hpp>
#include <cigmar/cigmar.hpp>

namespace cigmar {

	namespace windows {
		const String prefix = "\\\\?\\";
		const String suffix = "\\*";
		void formatPrefix(String& s, bool skipSpecialFolders = false) {
			if (skipSpecialFolders && (s == "." || s == ".."))
				return;
			if (!s.startsWith(prefix))
				s.insert(0, prefix);
		}
		void formatSuffix(String& s) {
			if (!s.endsWith(suffix))
				s << suffix;
		}
		void removePrefix(String& s) {
			if (s.startsWith(prefix))
				s.remove(0, prefix.length());
		}
		void removeSuffix(String& s) {
			if (s.endsWith(suffix)) {
				s.remove(s.length() - suffix.length(), suffix.length());
			}
		}
		void formatIfNeeded(String& path) {
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
			windows::formatPrefix(formatedFilename);
			windows::formatSuffix(formatedFilename);
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
				String p(pathname);
				String twoSeps = String::write(separator, separator);
				size_t sepLength = strlen(separator);
				p.replace(unixSeparator, separator);
				windows::removePrefix(p);
				windows::removeSuffix(p);
				// Remove separators at start (only on Windows).
				while (p.startsWith(separator))
					p.remove(0, sepLength);
				// Remove trailing separators.
				while (p.endsWith(separator))
					p.remove(p.length() - sepLength, sepLength);
				// Remove consecutive separators.
				while (p.contains(twoSeps))
					p.replace(twoSeps, separator);
				return p;
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

			bool isRoot(const char* pathname) {
				return isRooted(pathname) && pathname[3] == '\0';
			}

			bool isDirectory(const char* pathname) {
				bool is = true;
				try {
					Dir d(pathname);
				} catch (...) {
					is = false;
				}
				return is;
			};

			bool isFile(const char* pathname) {
				String formatedFilename = sys::path::absolute(pathname);
				windows::formatPrefix(formatedFilename);
				DWORD attributes = GetFileAttributes(formatedFilename.cstring());
				return attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
			}

		}

		int makeDirectory(const char* pathname) {
			String formated = sys::path::absolute(pathname);
			windows::formatPrefix(formated);
			return _mkdir(formated.cstring());
		};

		int removeDirectory(const char* pathname) {
			String formated = sys::path::absolute(pathname);
			windows::formatPrefix(formated);
			return _rmdir(formated.cstring());
		};
	}

}
#endif
