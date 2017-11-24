#include <iostream>
#include <cigmar/unittests.hpp>
#include <video/manager/manager.hpp>
#include <cigmar/print.hpp>
#include <video/Video.hpp>
#include <cigmar/numbers.hpp>

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
	sys::err::println("Folder path:", folder->getAbsolutePath());
	sys::err::println(folder->countVideos(), "videos.");
	String multipleProperties[] = {"category", "person", "place"};
	for (const String& mp: multipleProperties) {
		if (db.polyprops().contains(mp)) {
			sys::err::println("Multiple property", mp, "already registered.");
		} else {
			db.polyprops().addString(mp);
			sys::err::println("Multiple property", mp, "registered.");
		}
	}
	int count = numbers::random::uniform(3, 6);
	sys::err::println("Adding", count, "values for each property");
	for (videomanager::DbVideo& video: *folder) {
		for (const String& mp: multipleProperties) {
			auto& property = db.polyprops().get(mp);
			for (int i = 0; i < count; ++i) {
				String val = String::write(mp, ":value:", i + 1);
				if (video.hasMultiplePropertyValue(property, val)) {
					sys::err::println("Video already has value", val, "for property", mp, ":", video->getAbsolutePath());
				} else {
					video.addMultipleProperty(property, val);
					sys::err::println("Set value", val, "for property", mp, "for video", video->getAbsolutePath());
				}
			}
		}
	}
}

int main() {
	 testDatabase();
	 // tests::run();
	return 0;
}

