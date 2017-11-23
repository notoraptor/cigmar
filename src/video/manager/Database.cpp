//
// Created by notoraptor on 17-11-23.
//

#include <video/manager/Database.hpp>

namespace cigmar::video::manager {
	Database::Database(const char* dbname):
		database(dbname), library_names(), libraries(), propertyTypes(), uniqueProperties(), multipleProperties() {
		ensureDatabaseStructure();
		auto query = database.query("SELECT library_name FROM library");
		while (query) {
			library_names.add(query.getText(0));
			++query;
		}
		propertyTypes.init(database);
		uniqueProperties.init(database, propertyTypes);
		multipleProperties.init(database, propertyTypes);
	}
	Library& Database::addLibrary(const String& name, const String& thumbnailExtension) {
		if (hasLibrary(name))
			throw Exception("A library with name \"", name, "\" already exists.");
		database.run("INSERT INTO library(library_name, thumbnail_extension) VALUES(?, ?)", name, thumbnailExtension);
		return libraries.add(Library(*this, database.lastId(), name, thumbnailExtension));
	};
	Library& Database::getLibrary(const String& libraryName) {
		if (libraries.contains(libraryName))
			return libraries.get(libraryName);
		if (!hasLibrary(libraryName))
			throw Exception("This database does not contain a library with name ", libraryName);
		auto query = database.query("SELECT library_id, thumbnail_extension FROM library WHERE library_name = ?", libraryName);
		int64_t id = 0;
		String thumbnailExtension;
		size_t count = 0;
		while (query) {
			id = query.getInt64(0);
			thumbnailExtension = query.getText(1);
			++count;
			++query;
		}
		assert(count == 1);
		return libraries.add(Library(*this, id, libraryName, thumbnailExtension));
	};
	/*
	void Database::removeLibrary(const String& libraryName) {
		if (hasLibrary(libraryName)) {
			database.run("DELETE FROM library WHERE library_name = ?", libraryName);
			libraries.remove(libraryName);
		}
	}
	*/
}
