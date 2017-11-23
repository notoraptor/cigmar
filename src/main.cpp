#include <iostream>
#include <cigmar/unittests.hpp>
#include <video/manager/manager.hpp>
#include <cigmar/print.hpp>
#include <video/Video.hpp>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using namespace cigmar;

/* Notes: write a long multi-lines raw string in C++11+:

const char* jstring = R"(
{
"a": true,
"b": 1,
"salue": -2.9,
"hello": "merci"
}
)";

*/

void testDatabase() {
	String libraryName = "testLibrary";
	String folderName = "res/video";
	videomanager::Database db("res/work/video/model.db");
	videomanager::Library* library;
	videomanager::Folder* folder;
	sys::err::println(db.countLibraries(), "libraries.");
	if (db.hasLibrary(libraryName)) {
		library = &db.getLibrary(libraryName);
		sys::err::println("Library already exists:", libraryName);
	} else {
		library = &db.addLibrary(libraryName, video::Video::thumbnailExtension);
		sys::err::println("Library created:", libraryName);
	}
	sys::err::println(library->countFolders(), "folders.");
	if (library->hasFolder(folderName)) {
		folder = &library->getFolder(folderName);
		sys::err::println("Folder already registered:", folderName);
	} else {
		folder = &library->addFolder(folderName);
		sys::err::println("Folder added:", folderName);
	}
	sys::err::println("<a>");
	sys::err::println(folder->getAbsolutePath());
	sys::err::println("<b>");
	sys::err::println(folder->countVideos(), "videos.");
}

int main() {
	 testDatabase();
	 // tests::run();
	return 0;
}

