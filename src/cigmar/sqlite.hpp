//
// Created by notoraptor on 16/11/2017.
//

#ifndef SRC_CIGMAR_SQLITE_HPP
#define SRC_CIGMAR_SQLITE_HPP

#include <cstddef>
#include <cstdint>
#include <modules/sqlite.hpp>
#include <cigmar/classes/String.hpp>
#include <cigmar/classes/ArrayList.hpp>
#include <cigmar/base64.hpp>
#include <cigmar/exception/Exception.hpp>

namespace cigmar::sqlite {
	typedef void(*Destructor)(void*);
	template<typename T> struct Binding {
		bool dynamic;
		const T* pointer;
		int size;
		Destructor destructor;
		Binding(bool isDynamic, const T* data, int dataSize, Destructor dataDestructor):
			dynamic(isDynamic), pointer(data), size(dataSize), destructor(dataDestructor) {}
	};
	Binding<void> inline zeros(int size) {
		return {false, nullptr, size, nullptr};
	}
	Binding<void> inline Static(const void* data, int size) {
		return {false, data, size, nullptr};
	};
	Binding<char> inline Static(const char* data, int size = -1) {
		return {false, data, size, nullptr};
	};
	Binding<void> inline Dynamic(const void* data, int size, Destructor destructor = nullptr) {
		return {true, data, size, destructor};
	};
	Binding<char> inline Dynamic(const char* data, int size = -1, Destructor destructor = nullptr) {
		return {true, data, size, destructor};
	};

	class Query {
	private:
		sqlite3* db;
		sqlite3_stmt* stmt;
		bool done;
		void next() {
			int step = sqlite3_step(stmt);
			if (step != SQLITE_ROW) {
				if (step != SQLITE_DONE)
					throw Exception(String::println("Error iterating SQL statement:", sqlite3_errmsg(db)));
				done = true;
			}
		}
		bool checkType(int col, int type) {
			if (col >= sqlite3_column_count(stmt))
				throw Exception("Column index out of bound for SQLite statement.");
			return sqlite3_column_type(stmt, col) == type;
		}
		friend class Database;
		explicit Query(sqlite3* database, sqlite3_stmt* statement): db(database), stmt(statement), done(false) {
			next();
		};
		void clear() {
			if (stmt) {
				sqlite3_finalize(stmt);
				stmt = nullptr;
				done = true;
			}
		}
	public:
		Query(const Query&) = delete;
		Query(Query&& other) noexcept {
			db = other.db;
			stmt = other.stmt;
			done = other.done;
			other.stmt = nullptr;
			other.done = true;
		};
		~Query() {
			clear();
		}
		Query& operator=(const Query&) = delete;
		Query& operator=(Query&& other) noexcept {
			clear();
			db = other.db;
			stmt = other.stmt;
			done = other.done;
			other.stmt = nullptr;
			other.done = true;
			return *this;
		}
		size_t run() {
			size_t count = 1;
			while (!done) {
				next();
				++count;
			}
			return count;
		}
		Query& operator++() {if (!done) next();};
		explicit operator bool() const {return !done;}
		int countColumns() {
			return sqlite3_column_count(stmt);
		}
		String columnName(int col) {
			if (col >= sqlite3_column_count(stmt))
				throw Exception("Column index out of bound for SQLite statement.");
			const char* name = sqlite3_column_name(stmt, col);
			if (!name)
				throw Exception("Unable to get name of column ", col, " from a SQL statement.");
			return String(name);
		}
		bool isInteger(int col) {return checkType(col, SQLITE_INTEGER);};
		bool isFloating(int col) {return checkType(col, SQLITE_FLOAT);};
		bool isText(int col) {return checkType(col, SQLITE_TEXT);};
		bool isBlob(int col) {return checkType(col, SQLITE_BLOB);};
		bool isNull(int col) {return checkType(col, SQLITE_NULL);};
		int getInt(int col) {
			if (col >= sqlite3_column_count(stmt))
				throw Exception("Column index out of bound for SQLite statement.");
			if (sqlite3_column_type(stmt, col) != SQLITE_INTEGER)
				throw Exception("Expected an integer type for column ", col, " in SQLite statement.");
			return sqlite3_column_int(stmt, col);
		};
		int64_t getInt64(int col) {
			if (col >= sqlite3_column_count(stmt))
				throw Exception("Column index out of bound for SQLite statement.");
			if (sqlite3_column_type(stmt, col) != SQLITE_INTEGER)
				throw Exception("Expected an integer type for column ", col, " in SQLite statement.");
			return sqlite3_column_int64(stmt, col);
		};
		double getDouble(int col) {
			if (col >= sqlite3_column_count(stmt))
				throw Exception("Column index out of bound for SQLite statement.");
			if (sqlite3_column_type(stmt, col) != SQLITE_FLOAT)
				throw Exception("Expected a float type for column ", col, " in SQLite statement.");
			return sqlite3_column_double(stmt, col);
		};
		String getText(int col) {
			if (col >= sqlite3_column_count(stmt))
				throw Exception("Column index out of bound for SQLite statement.");
			if (sqlite3_column_type(stmt, col) != SQLITE_TEXT)
				throw Exception("Expected text type for column ", col, " in SQLite statement.");
			return String((const char*)sqlite3_column_text(stmt, col));
		};
		ArrayList<byte_t> getBlob(int col) {
			if (col >= sqlite3_column_count(stmt))
				throw Exception("Column index out of bound for SQLite statement.");
			if (sqlite3_column_type(stmt, col) != SQLITE_BLOB)
				throw Exception("Expected blob type for column ", col, " in SQLite statement.");
			const byte_t* data = (const byte_t*)sqlite3_column_blob(stmt, col);
			int dataSize = sqlite3_column_bytes(stmt, col);
			ArrayList<byte_t> out(dataSize);
			memcpy((byte_t*)out, data, dataSize);
			return out;
		};
		String toString(int col) {
			if (col >= sqlite3_column_count(stmt))
				throw Exception("Column index out of bound for SQLite statement.");
			switch (sqlite3_column_type(stmt, col)) {
				case SQLITE_INTEGER:
					return String::write(sqlite3_column_int64(stmt, col));
					break;
				case SQLITE_FLOAT:
					return String::write(sqlite3_column_double(stmt, col));
					break;
				case SQLITE_TEXT:
					return String((const char*)sqlite3_column_text(stmt, col));
					break;
				case SQLITE_BLOB:
					{
						const byte_t *data = (const byte_t *) sqlite3_column_blob(stmt, col);
						int dataSize = sqlite3_column_bytes(stmt, col);
						return base64::bytes::encode(data, (size_t) dataSize);
					}
					break;
				case SQLITE_NULL:
					return String();
					break;
				default:
					throw Exception("Unknown type for SQL column ", col, ".");
					break;
			}
		};
	};

	class Database {
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
		}
		void bind(sqlite3_stmt* stmt, int pos) {}
		void bind(sqlite3_stmt* stmt, int pos, const char* str, int size, sqlite::Destructor destructor) {
			if (sqlite3_bind_text(stmt, pos, str, size, destructor) != SQLITE_OK)
				throw Exception("Unable to bind char pointer to SQLite statement.");
		}
		void bind(sqlite3_stmt* stmt, int pos, const void* data, int size, sqlite::Destructor destructor) {
			if (sqlite3_bind_blob(stmt, pos, data, size, destructor) != SQLITE_OK)
				throw Exception("Unable to bind blob value to SQLite statement.");
		}
		template<typename... Args>
		void bind(sqlite3_stmt* stmt, int pos, double value, Args&&... args) {
			if (sqlite3_bind_double(stmt, pos, value) != SQLITE_OK)
				throw Exception("Unable to bind double value to SQLite statement.");
			bind(stmt, pos + 1, std::forward<Args>(args)...);
		}
		template<typename... Args>
		void bind(sqlite3_stmt* stmt, int pos, int value, Args&&... args) {
			if (sqlite3_bind_int(stmt, pos, value) != SQLITE_OK)
				throw Exception("Unable to bind integer value to SQLite statement.");
			bind(stmt, pos + 1, std::forward<Args>(args)...);
		}
		template<typename... Args>
		void bind(sqlite3_stmt* stmt, int pos, int64_t value, Args&&... args) {
			if (sqlite3_bind_int64(stmt, pos, value) != SQLITE_OK)
				throw Exception("Unable to bind 64-bits integer value to SQLite statement.");
			bind(stmt, pos + 1, std::forward<Args>(args)...);
		}
		template<typename... Args>
		void bind(sqlite3_stmt* stmt, int pos, size_t value, Args&&... args) {
			if (sqlite3_bind_int64(stmt, pos, value) != SQLITE_OK)
				throw Exception("Unable to bind 64-bits integer value to SQLite statement.");
			bind(stmt, pos + 1, std::forward<Args>(args)...);
		}
		template<typename... Args>
		void bind(sqlite3_stmt* stmt, int pos, const void* null, Args&&... args) {
			if (null)
				throw Exception("Expected a null value to bind to SQLite statement.");
			if (sqlite3_bind_null(stmt, pos) != SQLITE_OK)
				throw Exception("Unable to bind null value to SQLite statement.");
			bind(stmt, pos + 1, std::forward<Args>(args)...);
		}
		template<typename... Args>
		void bind(sqlite3_stmt* stmt, int pos, const char* cstring, Args&&... args) {
			if (sqlite3_bind_text(stmt, pos, cstring, -1, SQLITE_STATIC) != SQLITE_OK)
				throw Exception("Unable to bind static C-string value to SQLite statement.");
			bind(stmt, pos + 1, std::forward<Args>(args)...);
		}
		template<typename... Args>
		void bind(sqlite3_stmt* stmt, int pos, const String& str, Args&&... args) {
			bind(stmt, pos, Dynamic((const char*)str), std::forward<Args>(args)...);
		}
		template<typename T, typename... Args>
		void bind(sqlite3_stmt* stmt, int pos, sqlite::Binding<T> binding, Args&&... args) {
			if (binding.pointer) {
				sqlite::Destructor destructor;
				if (binding.dynamic) {
					if (binding.destructor)
						destructor = binding.destructor;
					else {
						// Transient binding.
						destructor = SQLITE_TRANSIENT;
					}
				} else {
					// Static binding.
					destructor = SQLITE_STATIC;
				}
				bind(stmt, pos, binding.pointer, binding.size, destructor);
			} else if (binding.size) {
				// Bind a zeroblob.
				if (sqlite3_bind_zeroblob(stmt, pos, binding.size) != SQLITE_OK)
					throw Exception("Unable to bind zero-blob value to SQLite statement.");
			} else
				throw Exception("Expected a size for a zero-blob value to bind to SQLite statement.");
			bind(stmt, pos + 1, std::forward<Args>(args)...);
		}
	public:
		Database(): db(nullptr) {};
		explicit Database(const char* dbname): db(nullptr) {
			open(dbname);
		};
		~Database() {
			if (db) sqlite3_close(db);
		};
		template<typename... Args>
		Query query(const char* sql, Args&&... bindings) {
			sqlite3_stmt* statement = NULL;
			if (sqlite3_prepare_v2(db, sql, -1, &statement, NULL) != SQLITE_OK) {
				String message = String::println("Unable to prepare SQL query:", sqlite3_errmsg(db));
				message << "Query:" << ENDL << sql << ENDL;
				sqlite3_finalize(statement);
				throw Exception(message);
			}
			try {
				bind(statement, 1, std::forward<Args>(bindings)...);
			} catch (...) {
				sqlite3_finalize(statement);
				throw;
			}
			return Query(db, statement);
		}
		template<typename... Args>
		Query query(const String& sql, Args&&... bindings) {
			return query((const char*)sql, std::forward<Args>(bindings)...);
		}
		template<typename... Args>
		void run(const char* sql, Args&&... bindings) {
			query(sql, std::forward<Args>(bindings)...).run();
		}
		template<typename... Args>
		void run(const String& sql, Args&&... bindings) {
			query((const char*)sql, std::forward<Args>(bindings)...).run();
		}
		int64_t lastId() {
			return sqlite3_last_insert_rowid(db);
		}
	};
}

#endif //SRC_CIGMAR_SQLITE_HPP
