#include <iostream>
#include <cigmar/unittests.hpp>
#include <cigmar/file/Lines.hpp>
#include <cigmar/print.hpp>
#include <cigmar/file/writer/Binary.hpp>
#include <video/database/Database.hpp>
#include <cigmar/math/Fraction.hpp>
#include <libraries/sqlite/sqlite3.h>

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

class SQLiteQuery {
private:
	sqlite3_stmt* stmt;
	sqlite3* db;
	bool done;
	void next() {
		int step = sqlite3_step(stmt);
		if (step != SQLITE_ROW) {
			if (step != SQLITE_DONE) {
				String message = String::println("Error iterating SQL statement:", sqlite3_errmsg(db));
				throw Exception(message);
			}
			done = true;
		}
	}
	explicit SQLiteQuery(sqlite3* database, sqlite3_stmt* statement): db(database), stmt(statement), done(false) {
		next();
	};
	friend class SQLiteDatabase;
public:
	~SQLiteQuery() {
		if (stmt) sqlite3_finalize(stmt);
	}
	SQLiteQuery& operator++() {next();};
	explicit operator bool() const {return !done;}
	int getInt(int col);
	int64_t getInt64(int col);
	double getDouble(int col);
	String getText(int col);
	ArrayList<byte_t> getBlob(int col);
};

class SQLiteDatabase {
private:
	sqlite3* db;
	void open(const char* dbname) {
		if (db) throw Exception("This database object is already used.");
		if (sqlite3_open(dbname, &db) != SQLITE_OK) {
			String message = String::println("Cannot open database:", dbname, ":", sqlite3_errmsg(db));
			sqlite3_close(db);
			// TODO: Does sqlite3_close() really clear the db pointer ?
			db = nullptr;
			throw Exception(message);
		}
	};
public:
	SQLiteDatabase(): db(nullptr) {};
	explicit SQLiteDatabase(const char* dbname): db(nullptr) {
		open(dbname);
	};
	~SQLiteDatabase() {
		if (db) sqlite3_close(db);
	};
	template<typename... Args>
	SQLiteQuery run(const char* sql, Args&&... bindings) {

	}
};

int main() {
	sqlite3* db;
	sqlite3_stmt* res;
	int rc = sqlite3_open(":memory:", &db);
	if (rc != SQLITE_OK) {
		String s = String::print("Cannot open sqlite3:", sqlite3_errmsg(db));
		sqlite3_close(db);
		throw Exception(s);
	}
	rc = sqlite3_prepare_v2(db, "SELECT SQLITE_VERSION()", -1, &res, 0);
	if (rc != SQLITE_OK) {
		String s = String::print("Cannot select sqlite version:", sqlite3_errmsg(db));
		sqlite3_close(db);
		throw Exception(s);
	}

	/*
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
	math::Fraction f1, f2(0), f3(1), f4(-1), f5(32), f6(-64), f7(2, 3), f8(-15, 9), f9(-1, 12, 9), f10(1, 17, 69);
	sys::err::println(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10);
	sys::err::println(math::Fraction(-777, 7*11*3));
	tests::run();
	*/
	return 0;
}

