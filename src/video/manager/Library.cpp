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
	}
	bool Library::hasFolder(const String& folder_path) const {
		String folderPath = sys::path::absolute((const char*)folder_path);
		if (folders.contains(folderPath))
			return true;
		String command;
		if (sys::isWindows())
			command = "SELECT COUNT(video_folder_id) FROM video_folder WHERE windows_absolute_path = ?";
		else
			command = "SELECT COUNT(video_folder_id) FROM video_folder WHERE unix_absolute_path = ?";
		auto query = db->query(command, folderPath);
		int64_t count = 0;
		while (query) {
			count += query.getInt64(0);
			++query;
		}
		return count == 1;
	};
	size_t Library::countFolders() const {
		auto query = db->query("SELECT count(video_folder_id) FROM video_folder WHERE library_id = ?", library_id);
		size_t count = 0;
		while (query) {
			count = (size_t)query.getInt64(0);
			++query;
		}
		return count;
	};
	bool Library::hasFolder(int64_t folderId) const {
		if (folders.contains(folderId))
			return true;
		auto query = db->query("SELECT COUNT(video_folder_id) FROM video_folder WHERE video_folder_id = ?", folderId);
		int64_t count = 0;
		while (query) {
			count += query.getInt64(0);
			++query;
		}
		return count == 1;
	};
	Folder& Library::getFolder(const String& folder_path) {
		String folderPath = sys::path::absolute((const char*)folder_path);
		if (folders.contains(folderPath))
			return folders.get(folderPath);
		String command;
		int64_t video_folder_id;
		if (sys::isWindows())
			command = "SELECT video_folder_id FROM video_folder WHERE windows_absolute_path = ?";
		else
			command = "SELECT video_folder_id FROM video_folder WHERE unix_absolute_path = ?";
		auto query = db->query(command, folderPath);
		size_t count = 0;
		while (query) {
			video_folder_id = query.getInt64(0);
			++count;
			++query;
		}
		if (count != 1)
			throw Exception("Unable to find folder ", folderPath);
		return folders.add(Folder(db, library_id, video_folder_id, folderPath));
	};
	Folder& Library::getFolder(int64_t folderId) {
		if (folders.contains(folderId))
			return folders.get(folderId);
		auto query = db->query("SELECT windows_absolute_path, unix_absolute_path FROM video_folder WHERE video_folder_id = ?",
							   folderId);
		String absolutePath, windowsAbsolutePath, unixAbsolutePath;
		size_t count;
		while (query) {
			windowsAbsolutePath = query.isNull(0) ? "" : query.getText(0);
			unixAbsolutePath = query.isNull(1) ? "" : query.getText(1);
			if (sys::isWindows()) {
				if (!windowsAbsolutePath)
					throw Exception("Folder does not have Windows path name: Unix path is ", unixAbsolutePath);
				absolutePath = windowsAbsolutePath;
			} else {
				if (!unixAbsolutePath)
					throw Exception("Folder does not have Unix path name: Windows path is ", windowsAbsolutePath);
				absolutePath = unixAbsolutePath;
			}
			++count;
			++query;
		}
		if (count != 1)
			throw Exception("Unable to find folder with id", folderId);
		return folders.add(Folder(db, library_id, folderId, absolutePath));
	};
	Folder& Library::addFolder(const String& folder_path) {
		String folderPath = sys::path::absolute((const char*)folder_path);
		if (hasFolder(folderPath)) {
			throw Exception("Library '", library_name, "': folder '", folderPath, "' already registered.");
		}
		if (sys::isWindows())
			db->run("INSERT INTO video_folder(windows_absolute_path, library_id) VALUES(?, ?)", folderPath, library_id);
		else
			db->run("INSERT INTO video_folder(unix_absolute_path, library_id) VALUES(?, ?)", folderPath, library_id);
		return folders.add(Folder(db, library_id, db->lastId(), folderPath));
	};
	void Library::removeFolder(const String& folderPath) {
		String absolutePath = sys::path::absolute((const char*)folderPath);
		if (hasFolder(folderPath)) {
			if (sys::isWindows())
				db->run("DELETE FROM video_folder WHERE windows_absolute_path = ?", absolutePath);
			else
				db->run("DELETE FROM video_folder WHERE unix_absolute_path = ?", absolutePath);
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
