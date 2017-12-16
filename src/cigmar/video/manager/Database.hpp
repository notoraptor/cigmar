//
// Created by notoraptor on 17-11-23.
//

#ifndef SRC_CIGMAR_DATABASE_HPP
#define SRC_CIGMAR_DATABASE_HPP

#include <cigmar/sqlite.hpp>
#include <cigmar/classes/HashSet.hpp>
#include <cigmar/video/manager/PropertyTypes.hpp>
#include <cigmar/video/manager/UniqueProperties.hpp>
#include <cigmar/video/manager/MultipleProperties.hpp>
#include <cigmar/video/manager/Library.hpp>
#include <cigmar/file/Lines.hpp>

namespace cigmar::video::manager {
	class Database {
	public:
		sqlite::Database database;
	private:
		HashSet<String> library_names;
		RowMapping<Library> libraries;
		PropertyTypes propertyTypes;
		static void loadDatabaseStructureQueries(const char* filename, ArrayList<String>& out) {
			file::Lines file(filename);
			String current;
			for (const String& line: file) {
				if (line().trim()) {
					current << line;
				} else {
					if (current.trim())
						out.add(current);
					current.clear();
				}
			}
			if (current.trim())
				out.add(current);
		}
		void ensureDatabaseStructure() {
			database.run("PRAGMA foreign_keys=ON");
			ArrayList<String> queries;
			loadDatabaseStructureQueries("res/work/video/model.sql", queries);
			for (const String& sql: queries) database.run(sql);
		}
		UniqueProperties uniqueProperties;
		MultipleProperties multipleProperties;
	public:
		explicit Database(const char* dbname);
		bool hasLibrary(const String& libraryName) const {return library_names.contains(libraryName);};
		size_t countLibraries() const {return library_names.size();};
		const HashSet<String>& getLibraryNames() const {return library_names;};
		Library& addLibrary(const String& name, const String& thumbnailExtension);
		Library& getLibrary(const String& libraryName);
		void removeLibrary(const String& libraryName) {
			if (hasLibrary(libraryName)) {
				database.run("DELETE FROM library WHERE library_name = ?", libraryName);
				libraries.remove(libraryName);
			}
		};
		sqlite::Database& db() {return database;};
		UniqueProperties& monoprops() {return uniqueProperties;}
		MultipleProperties& polyprops() {return multipleProperties;}
		sqlite::Database* operator->() {return &database;}
	};
}


#endif //SRC_CIGMAR_DATABASE_HPP
