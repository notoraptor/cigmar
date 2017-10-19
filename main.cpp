#include <cstdio>
#include <iostream>
#include <string>
#include <cigmar/unittests.hpp>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using namespace cigmar;

#ifdef WIN32
/* Windows OS. */

#include <windows.h>
class dirent {
private:
	WIN32_FIND_DATA ffd;
	friend class DIR;
public:
	const char* d_name;
};

class DIR {
private:
	dirent entry;
	HANDLE handle;
	bool alreadyRead;
	bool valid;
public:
	DIR(const char* filename) {
		const char* prefix = "\\\\?\\";
		string suffix = "\\*";
		string formatedFilename;
		if (strstr(filename, prefix) != filename)
			formatedFilename += prefix;
		formatedFilename += filename;
		if (formatedFilename.rfind(suffix) != (formatedFilename.length() - suffix.length()))
			formatedFilename += suffix;
		handle = FindFirstFile(formatedFilename.c_str(), &entry.ffd);
		valid = handle != INVALID_HANDLE_VALUE;
		alreadyRead = false;
	}
	~DIR() {
		if (handle != INVALID_HANDLE_VALUE) {
			FindClose(handle);
		}
	}
	operator bool() const {return valid;}
	dirent* next() {
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
		if (valid) {
			entry.d_name = entry.ffd.cFileName;;
			return &entry;
		}
		return NULL;
	}
};

DIR* openDir(const char* filename) {
	DIR* dir = new DIR(filename);
	if (!dir) {
		delete dir;
		dir = NULL;
	}
	return dir;
}
int closeDir(DIR* directory) {
	delete directory;
	return 0;
}
dirent* readDir(DIR* directory) {
	return directory->next();
}

#else
/* Non-Windows OS. */

#include <dirent.h>
typedef struct dirent dirent;
DIR* openDir(const char* filename) {
	return opendir(filename);
}
int closeDir(DIR* directory) {
	return closedir(directory);
}
dirent* readDir(DIR* directory) {
	return readdir(directory);
}

#endif

int open_directory(const char* filename) {
	DIR* dir = openDir(filename);
	if (!dir) {
		cerr << "unable to open dir." << endl;
		return -1;
	}
	dirent* entry = NULL;
	while ((entry = readDir(dir)) != NULL) {
		cerr << entry->d_name << endl;
	}
	closeDir(dir);
	return 0;
}

int main() {
	string p("tests"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa1"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa2"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa3"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa4"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa5"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa0"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa2"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa3"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa0"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa1"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa1"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa2"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa3"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa4"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa5"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa6"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa7"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa8"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa9"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa0"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa1"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa2"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa3"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa4"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa5"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa6"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa7"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa8"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa9");
#ifndef WIN32
	for (char& c: p) {
		if (c == '\\')
			c = '/';
	}
#endif
	cerr << open_directory(p.c_str()) << endl;
	tests::run();
	return 0;
}
