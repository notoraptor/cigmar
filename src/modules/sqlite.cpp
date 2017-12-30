//
// Created by notoraptor on 29/12/2017.
//
#include <modules/sqlite.hpp>
#include <cigmar/rll.hpp>

INIT_DYNAMIC_FUNCTION(sqlite3_open);
INIT_DYNAMIC_FUNCTION(sqlite3_close);
INIT_DYNAMIC_FUNCTION(sqlite3_last_insert_rowid);
INIT_DYNAMIC_FUNCTION(sqlite3_errmsg);
INIT_DYNAMIC_FUNCTION(sqlite3_prepare_v2);
INIT_DYNAMIC_FUNCTION(sqlite3_step);
INIT_DYNAMIC_FUNCTION(sqlite3_finalize);
INIT_DYNAMIC_FUNCTION(sqlite3_bind_zeroblob);
INIT_DYNAMIC_FUNCTION(sqlite3_bind_blob);
INIT_DYNAMIC_FUNCTION(sqlite3_bind_text);
INIT_DYNAMIC_FUNCTION(sqlite3_bind_double);
INIT_DYNAMIC_FUNCTION(sqlite3_bind_int);
INIT_DYNAMIC_FUNCTION(sqlite3_bind_int64);
INIT_DYNAMIC_FUNCTION(sqlite3_bind_null);
INIT_DYNAMIC_FUNCTION(sqlite3_column_count);
INIT_DYNAMIC_FUNCTION(sqlite3_column_name);
INIT_DYNAMIC_FUNCTION(sqlite3_column_type);
INIT_DYNAMIC_FUNCTION(sqlite3_column_bytes);
INIT_DYNAMIC_FUNCTION(sqlite3_column_blob);
INIT_DYNAMIC_FUNCTION(sqlite3_column_text);
INIT_DYNAMIC_FUNCTION(sqlite3_column_double);
INIT_DYNAMIC_FUNCTION(sqlite3_column_int);
INIT_DYNAMIC_FUNCTION(sqlite3_column_int64);

namespace module {
	struct sqlite_module_initializer_t {
		cigmar::RLL* rll;
		sqlite_module_initializer_t(): rll(nullptr) {
			const char* names[] = {"libsqlite", "libmodsqlite", nullptr};
			for (int i = 0; !rll && names[i]; ++i) {
				try {
					rll = new cigmar::RLL(cigmar::String::write(names[i], SHARED_LIBRARY_EXTENSION));
				} catch (cigmar::Exception& e) {
					// pass.
				}
			}
			if (!rll)
				throw cigmar::Exception("Unable to find sqlite dynamic library (looked for: libsqlite, libmodsqlite).");
			DEFINE_DYNAMIC_FUNCTION(sqlite3_open, rll);
			DEFINE_DYNAMIC_FUNCTION(sqlite3_close, rll);
			DEFINE_DYNAMIC_FUNCTION(sqlite3_last_insert_rowid, rll);
			DEFINE_DYNAMIC_FUNCTION(sqlite3_errmsg, rll);
			DEFINE_DYNAMIC_FUNCTION(sqlite3_prepare_v2, rll);
			DEFINE_DYNAMIC_FUNCTION(sqlite3_step, rll);
			DEFINE_DYNAMIC_FUNCTION(sqlite3_finalize, rll);
			DEFINE_DYNAMIC_FUNCTION(sqlite3_bind_zeroblob, rll);
			DEFINE_DYNAMIC_FUNCTION(sqlite3_bind_blob, rll);
			DEFINE_DYNAMIC_FUNCTION(sqlite3_bind_text, rll);
			DEFINE_DYNAMIC_FUNCTION(sqlite3_bind_double, rll);
			DEFINE_DYNAMIC_FUNCTION(sqlite3_bind_int, rll);
			DEFINE_DYNAMIC_FUNCTION(sqlite3_bind_int64, rll);
			DEFINE_DYNAMIC_FUNCTION(sqlite3_bind_null, rll);
			DEFINE_DYNAMIC_FUNCTION(sqlite3_column_count, rll);
			DEFINE_DYNAMIC_FUNCTION(sqlite3_column_name, rll);
			DEFINE_DYNAMIC_FUNCTION(sqlite3_column_type, rll);
			DEFINE_DYNAMIC_FUNCTION(sqlite3_column_bytes, rll);
			DEFINE_DYNAMIC_FUNCTION(sqlite3_column_blob, rll);
			DEFINE_DYNAMIC_FUNCTION(sqlite3_column_text, rll);
			DEFINE_DYNAMIC_FUNCTION(sqlite3_column_double, rll);
			DEFINE_DYNAMIC_FUNCTION(sqlite3_column_int, rll);
			DEFINE_DYNAMIC_FUNCTION(sqlite3_column_int64, rll);
		}
		~sqlite_module_initializer_t() {
			if (rll) delete rll;
		}
	} sqlite_module_initializer;
}
