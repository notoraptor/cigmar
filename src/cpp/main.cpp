#include <iostream>
#include <cigmar/unittests.hpp>
#include <cigmar/video/manager/manager.hpp>
#include <cigmar/print.hpp>
#include <cigmar/classes/UnicodeString.hpp>
#include <cigmar/tree.hpp>

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
void testUnicode() {
	const char* s = "部首 国字 木, 神 !";
	std::cout << "Hello World!" << std::endl;
	std::cout << "部首 国字 木, 神 !" << std::endl;
	std::cout << s << std::endl;

	std::string utf8 =  u8"z\u00df\u6c34\U0001d10b"; // or u8"zß水𝄋"
	// or "\x7a\xc3\x9f\xe6\xb0\xb4\xf0\x9d\x84\x8b";
	std::cout << "original UTF-8 string size: " << utf8.size() << '\n';
	std::cout << utf8 << std::endl;

	// UTF-8 to UTF-32
	std::vector<uint32_t> utf32;
	unicode::convert(utf8, utf32);
	std::cout << "UTF-32 string size: " << utf32.size() << '\n';

	// UTF-32 to UTF-8
	std::string decoded;
	unicode::convert(utf32, decoded);
	std::cout << "new UTF-8 string size: " << decoded.size() << '\n';
	std::cout << utf8 << std::endl;
	std::cout << decoded << std::endl;
	std::cout << (utf8 == decoded) << std::endl;
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

int main() {
	 tests::run();
	 testUnicode();
	 testDatabase();
	 testNodes();
	 sys::err::println("Node count is", tree::nodes_count);
	return 0;
}
