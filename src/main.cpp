#include <iostream>
#include <cigmar/unittests.hpp>
#include <cigmar/file/Lines.hpp>
#include <cigmar/print.hpp>
#include <cigmar/file/writer/Binary.hpp>
#include <video/database.hpp>
#include <cigmar/sqlite.hpp>
#include <cigmar/file/text/read.hpp>
#include <cigmar/utils/Hasher.hpp>

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
	video::database::Database database("res/work/video/model.db");
	String libraryName = "myTestLibrary";
	String folderName = "res/video";
	video::database::Library library;
	video::database::Folder folder;
	if (database.libraryExists(libraryName)) {
		sys::err::println("Library", libraryName, "already exists.");
		library = database.getLibrary(libraryName);
	} else {
		library = database.createLibrary(libraryName);
	}
	if (sys::path::isDirectory((const char*)folderName)) {
		if (library.folderExists(folderName)) {
			sys::err::println("Folder", folderName, "already exists.");
			folder = library.getFolder(folderName);
		} else {
			folder = library.createFolder(folderName);
		}
	}
	sys::err::println(database.countLibraries(), "librarie(s)");
	sys::err::println(library.countFolders(), "folder(s)");
	sys::err::println(library.getFolders().size(), "folder(s) loaded");
	sys::err::println("Library details:", library.getId(), library.getName(), library.getThumbnailExtension());
	sys::err::println("Folder details    :", folder.getId(), folder.getAbsolutePath());
	sys::err::println(folder.countVideos(), "videos.");

	if (database.uniqueProperties.has("notation"))
		sys::err::println("Unique property notation already exists.");
	else
		database.uniqueProperties.addInteger("notation", "0");

	if (database.multipleProperties.has("category"))
		sys::err::println("Multiple property category already exists.");
	else
		database.multipleProperties.addText("category");
}

int main() {
	 testDatabase();
	 tests::run();
	return 0;
}

