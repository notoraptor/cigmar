//
// Created by notoraptor on 29/12/2017.
//
#ifndef SRC_CIGMAR_MODULES_SQLITE_HPP
#define SRC_CIGMAR_MODULES_SQLITE_HPP
#include <cstdint>

#define DECLARE_DYNAMIC_FUNCTION(name, return_type, parameters) \
typedef return_type (*fn_##name) parameters; \
extern fn_##name name;

#define INIT_DYNAMIC_FUNCTION(name) fn_##name name = nullptr;
#define DEFINE_DYNAMIC_FUNCTION(name, dynamic_loader_ptr) name = (fn_##name)((dynamic_loader_ptr)->get( #name ));

typedef int64_t sqlite3_int64;
struct sqlite3;
struct sqlite3_stmt;
DECLARE_DYNAMIC_FUNCTION(sqlite3_open, int, (const char*, sqlite3**));
DECLARE_DYNAMIC_FUNCTION(sqlite3_close, int, (sqlite3*));
DECLARE_DYNAMIC_FUNCTION(sqlite3_last_insert_rowid, sqlite3_int64, (sqlite3*));
DECLARE_DYNAMIC_FUNCTION(sqlite3_errmsg, const char*, (sqlite3*));
DECLARE_DYNAMIC_FUNCTION(sqlite3_prepare_v2, int, (sqlite3*, const char*, int, sqlite3_stmt**, const char**));
DECLARE_DYNAMIC_FUNCTION(sqlite3_step, int, (sqlite3_stmt*));
DECLARE_DYNAMIC_FUNCTION(sqlite3_finalize, int, (sqlite3_stmt*));
DECLARE_DYNAMIC_FUNCTION(sqlite3_bind_zeroblob, int, (sqlite3_stmt*, int, int));
DECLARE_DYNAMIC_FUNCTION(sqlite3_bind_blob, int, (sqlite3_stmt*, int, const void*, int, void(*)(void*)));
DECLARE_DYNAMIC_FUNCTION(sqlite3_bind_text, int, (sqlite3_stmt*,int,const char*,int,void(*)(void*)));
DECLARE_DYNAMIC_FUNCTION(sqlite3_bind_double, int, (sqlite3_stmt*, int, double));
DECLARE_DYNAMIC_FUNCTION(sqlite3_bind_int, int, (sqlite3_stmt*, int, int));
DECLARE_DYNAMIC_FUNCTION(sqlite3_bind_int64, int, (sqlite3_stmt*, int, sqlite3_int64));
DECLARE_DYNAMIC_FUNCTION(sqlite3_bind_null, int, (sqlite3_stmt*, int));
DECLARE_DYNAMIC_FUNCTION(sqlite3_column_count, int, (sqlite3_stmt*));
DECLARE_DYNAMIC_FUNCTION(sqlite3_column_name, const char*, (sqlite3_stmt*, int));
DECLARE_DYNAMIC_FUNCTION(sqlite3_column_type, int, (sqlite3_stmt*, int));
DECLARE_DYNAMIC_FUNCTION(sqlite3_column_bytes, int, (sqlite3_stmt*, int));
DECLARE_DYNAMIC_FUNCTION(sqlite3_column_blob, const void*, (sqlite3_stmt*, int));
DECLARE_DYNAMIC_FUNCTION(sqlite3_column_text, const unsigned char*, (sqlite3_stmt*, int));
DECLARE_DYNAMIC_FUNCTION(sqlite3_column_double, double, (sqlite3_stmt*, int));
DECLARE_DYNAMIC_FUNCTION(sqlite3_column_int, int, (sqlite3_stmt*, int));
DECLARE_DYNAMIC_FUNCTION(sqlite3_column_int64, sqlite3_int64, (sqlite3_stmt*, int));

typedef void (*sqlite3_destructor_type)(void*);
#define SQLITE_STATIC      ((sqlite3_destructor_type)0)
#define SQLITE_TRANSIENT   ((sqlite3_destructor_type)-1)

#define SQLITE_INTEGER  1
#define SQLITE_FLOAT    2
#define SQLITE_BLOB     4
#define SQLITE_NULL     5
# define SQLITE_TEXT     3
#define SQLITE_OK           0
#define SQLITE_ROW         100
#define SQLITE_DONE        101

#endif //SRC_CIGMAR_MODULES_SQLITE_HPP
