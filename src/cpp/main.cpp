#include <iostream>
#include <cigmar/unittests.hpp>
#include <cigmar/video/manager/manager.hpp>
#include <cigmar/print.hpp>
#include <cigmar/tree.hpp>
#include <cigmar/file/writer/Binary.hpp>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using namespace cigmar;

namespace cigmar::tree {
	size_t nodes_count = 0;
}
struct MyContent: public tree::Content<MyContent> {
	int val = 11101;
	using tree::Content<MyContent>::Content;
};
using MyNode = tree::Node<MyContent>;
void testNodes() {
	MyNode a("a", nullptr);
	MyNode a1("a1", nullptr);
	MyNode b("b", a);
	MyNode c("c", b);
	MyNode d("d", b);
	sys::err::println(a->isRoot());
	sys::err::println(b->isInternal());
	sys::err::println(c->isLeaf());
	sys::err::println(b->size());
	sys::err::println(c->parent() == b);
	sys::err::println(a);
	sys::err::println(a1);
	sys::err::println();
	a1->add(c);
	sys::err::println(a);
	sys::err::println(a1);
	sys::err::println();
	b->add(a1);
	sys::err::println(a);
	sys::err::println(a1);
	sys::err::println();
	sys::err::println("a", a.refcount(), "a1", a1.refcount(), "b", b.refcount(), "c", c.refcount());
	sys::err::println(a->val, b->val);
	sys::err::println(a.typesize(), b->parent().typesize());
	sys::err::println("Node count is", tree::nodes_count);
}
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
void testImage() {
	// OUTPUT AN IMAGE IN .PAM FORMAT.
	file::writer::Binary out("test.pam");
	String header;
	header << "P7" << ENDL;
	header << "WIDTH 4" << ENDL;
	header << "HEIGHT 2" << ENDL;
	header << "DEPTH 4" << ENDL;
	header << "MAXVAL 255" << ENDL;
	header << "TUPLTYPE RGB_ALPHA" << ENDL;
	header << "ENDHDR" << ENDL;
	// FF00007F 00FF007F 0000FF7F FFFFFF7F FF0000FF 00FF00FF 0000FFFF FFFFFFFF
	unsigned char values[] = {0xFF, 0x00, 0x00, 0x7F, 0x00, 0xFF, 0x00, 0x7F, 0x00, 0x00, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0x7F,
							  0xFF, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	out << header;
	out.write((const char*)values, 32);
}

int main() {
//	tests::run();
//	testDatabase();
//	testNodes();
//	sys::err::println("Node count is", tree::nodes_count);
	return 0;
}
