#include <iostream>
#include <cigmar/unittests.hpp>
#include <cigmar/file/Lines.hpp>
#include <cigmar/print.hpp>
#include <cigmar/file/writer/Binary.hpp>
#include <video/database.hpp>
#include <cigmar/sqlite.hpp>
#include <cigmar/file/text/read.hpp>

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
	String collectionName = "myTestCollection";
	String folderName = "res/video";
	video::database::VideoCollection collection;
	video::database::Folder folder;
	if (database.collectionExists(collectionName)) {
		sys::err::println("Collection", collectionName, "already exists.");
		collection = database.getCollection(collectionName);
	} else {
		collection = database.createCollection(collectionName);
	}
	if (sys::path::isDirectory((const char*)folderName)) {
		if (collection.folderExists(folderName)) {
			sys::err::println("Folder", folderName, "already exists.");
			folder = collection.getFolder(folderName);
		} else {
			folder = collection.createFolder(folderName);
		}
	}
	sys::err::println(database.countCollections(), "collection(s)");
	sys::err::println(collection.countFolders(), "folder(s)");
	sys::err::println(collection.getFolders().size(), "folder(s) loaded");
	sys::err::println("Collection details:", collection.getId(), collection.getName(), collection.getThumbnailExtension());
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

class A{};

int main() {

	// testDatabase();

	// tests::run();
	sys::err::println(is_streamable<int>::value);
	sys::err::println(is_streamable<A>::value);

	return 0;
}

