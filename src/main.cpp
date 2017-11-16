#include <iostream>
#include <cigmar/unittests.hpp>
#include <cigmar/file/Lines.hpp>
#include <cigmar/print.hpp>
#include <cigmar/file/writer/Binary.hpp>
#include <video/database/Database.hpp>
#include <cigmar/sqlite.hpp>

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
	sqlite::Dataabase db("res/test.db");
	sqlite::Query query = db.query(
		"CREATE TABLE IF NOT EXISTS entries (ID INTEGER PRIMARY KEY AUTOINCREMENT, value VARCHAR(32))"
	);
	sys::err::println(query.run());
	query = db.query("INSERT INTO entries(value) VALUES(?)", sqlite::Static("Bonjour!"));
	sys::err::println(query.run());
	query = db.query("SELECT ID, value FROM entries WHERE ID > ?", 7);
	while (query) {
		int cc = query.countColumns();
		for (int i = 0; i < cc; ++i) {
			sys::err::write(query.columnName(i), " ");
			if (query.isInteger(i))
				sys::err::write(query.getInt64(i), " ");
			else if (query.isText(i))
				sys::err::write(query.getText(i), " ");
			else
				sys::err::print("Unexpected:", query.toString(i), " ");
		}
		sys::err::println();
		++query;
	}
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

