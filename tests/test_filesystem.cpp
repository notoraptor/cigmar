#include <iostream>
#include <string>
#include <cigmar/filesystem.hpp>
#include <cigmar/unittests.hpp>

using std::cerr;
using std::endl;
using std::string;
using namespace cigmar;

utt_begin(TestFileSystem);

utt(test_separator) {
	string separator(sys::path::separator);
	#ifdef WIN32
	utt_assert(separator == "\\");
	#else
	utt_assert(separator == "/");
	#endif
}

utt(test_current_dir) {
	sys::Dir* dir = sys::dir::open(".");
	sys::Dirent* entry = NULL;
	utt_assert(dir != NULL);
	while ((entry = sys::dir::read(dir)) != NULL) {
		String s = sys::path::join(".", sys::nameof(entry));
		bool isDirectory = sys::path::isDirectory(s.cstring());
		bool isFile = sys::path::isFile(s.cstring());
		utt_assert((isDirectory && !isFile) || (isFile && !isDirectory));
		if (s.endsWith(String(sys::path::separator, "..")))
			utt_assert(isDirectory);
		if (s.endsWith(String(sys::path::separator, ".")))
			utt_assert(isDirectory);
	}
	sys::dir::close(dir);
}

utt(test_norm) {
	utt_assert(sys::path::norm(".") == ".");
	utt_assert(sys::path::norm("..") == "..");
	#ifdef WIN32
	utt_assert(sys::path::norm("///////.///////////////////////////////") == ".");
	utt_assert(sys::path::norm("\\\\\\\\\\..\\\\\\") == "..");
	#else
	utt_assert(sys::path::norm("///////.///////////////////////////////") == "/.");
	utt_assert(sys::path::norm("\\\\\\\\\\..\\\\\\") == "/..");
	#endif
}

utt(test_resolve) {
	utt_assert(sys::path::resolve("./././././///././././///.//./") == ".");
	utt_assert(sys::path::resolve("a/b/./././../c/d/e/f/../../..//////../") == "a");
	utt_assert(sys::path::resolve("a/b/./././../c/d/e/f/../../..//////../..") == ".");
	utt_assert(sys::path::resolve("a/b/./././../c/d/e/f/../../..//////../../..") == "..");
}

utt(test_join) {
	string a = "a/b/c/d\\e\\f\\g";
	string b = "h/i/j";
	string c = "k\\something\\l/m";
	string sep(sys::path::separator);
	utt_assert(sys::path::join(a, b, c) == (a + sep + b + sep + c));
	utt_assert(sys::path::join(c, b, c, a) == (c + sep + b + sep + c + sep + a));
}

utt(test_absolute) {
	String current = sys::path::absolute(".");
	String parent = sys::path::absolute("..");
	utt_assert(sys::path::isRooted((const char*)current));
	utt_assert(sys::path::isAbsolute((const char*)current));
	utt_assert(sys::path::isRooted((const char*)parent));
	utt_assert(sys::path::isAbsolute((const char*)parent));
}

utt(test_very_long_path) {
	const char *p ="tests"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa1"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa2"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa3"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa4"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa5"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa6"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa7"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa8"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa9"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa10"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa11"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa12"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa13"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa14"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa15"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa16"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa17"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa18"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa19"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa20"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa21"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa22"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa23"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa24"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa25"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa26"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa27"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa28"
			"\\aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa29";
	utt_assert(sys::path::isRelative(p));
	utt_assert(!sys::path::isFile(p));
	utt_assert(sys::path::isDirectory(p));
	utt_assert(sys::path::exists(p));
	String abs = sys::path::absolute(p);
	utt_assert(abs);
}


utt_end();
