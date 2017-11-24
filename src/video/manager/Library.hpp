//
// Created by notoraptor on 17-11-23.
//

#ifndef SRC_CIGMAR_LIBRARY_HPP
#define SRC_CIGMAR_LIBRARY_HPP

#include <video/manager/TableRow.hpp>
#include <video/manager/RowMapping.hpp>
#include <video/manager/Folder.hpp>

namespace cigmar::video::manager {
	class Database;
	class Library: public TableRow {
		Database& db;
		int64_t library_id;
		String library_name;
		String thumbnail_extension;
		RowMapping<Folder> folders;
	public:
		Library(Database& database, int64_t id, const String& name, const String& thumbnailExtension);
		bool hasFolder(const String& folder_path) const;
		bool hasFolder(int64_t folderId) const;
		size_t countFolders() const;
		Folder& addFolder(const String& folder_path);
		Folder& getFolder(const String& folder_path);
		Folder& getFolder(int64_t folderId);
		void removeFolder(const String& folderPath);
		void removeFolder(int64_t folderId);
	};
}


#endif //SRC_CIGMAR_LIBRARY_HPP
