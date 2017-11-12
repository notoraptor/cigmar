#include <iostream>
#include <cigmar/unittests.hpp>
#include <cigmar/file/Lines.hpp>
#include <cigmar/print.hpp>
#include <video/utils.hpp>
#include <cigmar/file/writer/Binary.hpp>

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
	// tests::run();
	const char* foldername = "res/video";
	const char* dbname = "mydb.db.tsv";
	ArrayList<video::Video> videos = video::loadVideosFromDirectory(foldername);
	file::writer::Binary outfile(sys::path::join(foldername, dbname));
	sys::err::println(videos.size(), "videos.");
	outfile << video::TsvVideoRecorder::header() << ENDL;
	for(const video::Video& video: videos) {
		outfile << video::TsvVideoRecorder(video) << ENDL;
	}
	return 0;
}

