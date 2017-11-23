//
// Created by notoraptor on 17-11-23.
//

#include <video/manager/Library.hpp>
#include <video/manager/Database.hpp>
namespace cigmar::video::manager {
	Library::Library(Database& database, int64_t id, const String& name, const String& thumbnailExtension):
		db(database), library_id(id), library_name(name), thumbnail_extension(thumbnailExtension), folders() {
		setId(library_id);
		setKey(library_name);
		auto query = db->query("SELECT video_folder_id, absolute_path FROM video_folder WHERE library_id = ?",
							   library_id);
		while (query) {
			folders.add(Folder(database, *this, query.getInt64(0), query.getText(1)));
			++query;
		}
	}
	Folder& Library::addFolder(const String& folder_path) {
		String folderPath = sys::path::absolute((const char*)folder_path);
		if (hasFolder(folderPath)) {
			throw Exception("Library '", library_name, "': folder '", folderPath, "' already registered.");
		}
		db->run("INSERT INTO video_folder(absolute_path, library_id) VALUES(?, ?)", folderPath, library_id);
		return folders.add(Folder(db, *this, db->lastId(), folderPath));
	};
	void Library::removeFolder(const String& folderPath) {
		String absolutePath = sys::path::absolute((const char*)folderPath);
		if (hasFolder(folderPath)) {
			db->run("DELETE FROM video_folder WHERE absolute_path = ?", absolutePath);
			folders.remove(absolutePath);
		}
	}
	void Library::removeFolder(int64_t folderId) {
		if (hasFolder(folderId)) {
			db->run("DELETE FROM video_folder WHERE video_folder_id = ?", folderId);
			folders.remove(folderId);
		}
	}
}
