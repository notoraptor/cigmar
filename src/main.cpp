#include <iostream>
#include <cigmar/unittests.hpp>
#include <cigmar/file/Lines.hpp>
#include <cigmar/print.hpp>
#include <cigmar/file/writer/Binary.hpp>
#include <video/database/Database.hpp>

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

int main() {
	const char* foldername = "res/video";
	const char* dbname = "mydb.db.tsv";
	ArrayList<video::Video> videos = video::database::loadFromDirectory(foldername);
	file::writer::Binary outfile(sys::path::join(foldername, dbname));
	sys::err::println(videos.size(), "videos.");

	auto tsv = video::database::tsv();
	outfile << tsv.getHeader() << ENDL;
	for (const video::Video& video: videos) {
		outfile << tsv.getRow(video) << ENDL;
	}
	tests::run();
	return 0;
}

