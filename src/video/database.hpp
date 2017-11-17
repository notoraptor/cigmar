//
// Created by HPPC on 12/11/2017.
//

#ifndef SRC_CIGMAR_DATABASE_HPP
#define SRC_CIGMAR_DATABASE_HPP

#include <video/Video.hpp>
#include <cigmar/sqlite.hpp>
#include <cigmar/classes/HashMap.hpp>
#include <cigmar/classes/HashSet.hpp>
#include <cigmar/classes/TreeSet.hpp>
#include <cigmar/interfaces/Streamable.hpp>

namespace cigmar::video::database {

	void collect(const char *dirpath, ArrayList<Video> &videos) {
		sys::Dir dir(dirpath);
		for (const String pathname: dir) {
			if (pathname != "." && pathname != "..") {
				String path = sys::path::join(dirpath, pathname);
				if (sys::path::isDirectory((const char *) path)) {
					collect((const char *) path, videos);
				} else {
					String extension = sys::path::extension((const char *) pathname);
					if (Video::extensionIsSupported(extension.lower())) {
						videos.add(Video(sys::path::join(dirpath, pathname)));
					}
				}
			}
		}
	}
	void collect(const char *dirpath, const std::function<void(const Video&)>& collector) {
		sys::Dir dir(dirpath);
		for (const String pathname: dir) {
			if (pathname != "." && pathname != "..") {
				String path = sys::path::join(dirpath, pathname);
				if (sys::path::isDirectory((const char *) path)) {
					collect((const char *) path, collector);
				} else {
					String extension = sys::path::extension((const char *) pathname);
					if (Video::extensionIsSupported(extension.lower())) {
						collector(Video(sys::path::join(dirpath, pathname)));
					}
				}
			}
		}
	}
	ArrayList<Video> collect(const char *dirpath) {
		ArrayList<Video> videos;
		collect(dirpath, videos);
		return videos;
	}

	class VideoCollection;

	class Folder: public Streamable {
		friend class VideoCollection;
	public:
		struct Report {
			size_t total = 0;
			size_t added = 0;
			size_t removed = 0;
		};
	private:
		sqlite::Dataabase* db;
		VideoCollection* collection;
		int64_t video_folder_id;
		String absolute_path;
		TreeSet<Video> videos;
		Folder(sqlite::Dataabase& database, VideoCollection& parent, int64_t folderId, const String& absolutePath):
			db(&database), collection(&parent), video_folder_id(folderId), absolute_path(absolutePath), videos() {
			Report report = update();
			sys::err::println("Folder:", absolutePath);
			sys::err::println("\tTotal:  ", report.total);
			sys::err::println("\tAdded:  ", report.added);
			sys::err::println("\tRemoved:", report.removed);
		}
		Report update() {
			Report report;
			collect((const char*)absolute_path, [this](const Video& video) { videos.add(video); });
			report.total = videos.size();
			auto query = db->query(
				"SELECT video_id, relative_path FROM video WHERE video_folder_id = ?",
				video_folder_id);
			HashMap<String, int64_t> videosAbsolutePathsFromDb;
			HashSet<String> videosAbsolutePathsFromDisk;
			while (query) {
				String videoAbsolutePath =
					sys::path::absolute((const char*)sys::path::join(absolute_path, query.getText(1)));
				videosAbsolutePathsFromDb[videoAbsolutePath] = query.getInt64(0);
				++query;
			}
			for (const Video& video: videos) {
				String videoAbsolutePath = video.getAbsolutePath();
				videosAbsolutePathsFromDisk.add(videoAbsolutePath);
				if (!videosAbsolutePathsFromDb.contains(videoAbsolutePath)) {
					// We must add this video to the database.
					db->run(R"(
                            INSERT INTO video (
                                date_added_microseconds, video_size, format,
                                audiocodec, videocodec, samplerate, framerate,
                                duration, width, height,
                                relative_path, video_hash, video_folder_id
                            ) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?))",
							video.getDateAddedMicroseconds(), video.getSize(), video.getFormat(),
							video.getAudioCodec(), video.getVideoCodec(), video.getSampleRate(), video.getFrameRate(),
							video.getDuration(), video.getWidth(), video.getHeight(),
							video.getRelativePath(absolute_path), video.getId(), video_folder_id
					);
					++report.added;
				}
			}
			for (auto it = videosAbsolutePathsFromDb.begin(); it != videosAbsolutePathsFromDb.end(); ++it) {
				if (!videosAbsolutePathsFromDisk.contains(it->first)) {
					// We must remove this video from the database.
					db->run("DELETE FROM video WHERE video_id = ?", it->second);
					++report.removed;
				}
			}
			return report;
		}
	public:
		void toStream(std::ostream& o) const override {
			o << absolute_path;
		}
		Folder(): db(nullptr), collection(nullptr), video_folder_id(-1), absolute_path() {}
		explicit operator bool() const {return (bool)db;}
		int64_t getId() const {return video_folder_id;}
		const String& getAbsolutePath() const {return absolute_path;}
	};

	class VideoCollection {
	private:
		sqlite::Dataabase* db;
		int64_t collection_id;
		String collection_name;
		String thumbnail_extension;
		VideoCollection(sqlite::Dataabase& database,
						int64_t collectionId,
						const String& collectionName,
						const String& thumnailExtension):
			db(&database), collection_id(collectionId), collection_name(collectionName), thumbnail_extension(thumnailExtension) {}
		friend class Database;
	public:
		VideoCollection(): db(nullptr), collection_id(-1), collection_name(), thumbnail_extension() {}
		explicit operator bool() const {return (bool)db;}
		int64_t getId() const {return collection_id;}
		const String& getName() const {return collection_name;}
		const String& getThumbnailExtension() const {return thumbnail_extension;}
		size_t countFolders() {
			auto query = db->query("SELECT COUNT(video_folder_id) FROM video_folder");
			size_t count = 0;
			while (query) {
				count = (size_t)query.getInt64(0);
				++query;
			}
			return count;
		}
		bool folderExists(const String& dirname) {
			String absolutePath = sys::path::absolute((const char*)dirname);
			auto query = db->query(
				"SELECT COUNT(video_folder_id) FROM video_folder WHERE collection_id = ? AND absolute_path = ?",
				collection_id, absolutePath
			);
			int count = 0;
			while (query) {
				count = query.getInt(0);
				++query;
			}
			return count == 1;
		}
		Folder createFolder(const String& dirname) {
			String absolutePath = sys::path::absolute((const char*)dirname);
			db->run(
				"INSERT INTO video_folder (collection_id, absolute_path) VALUES(?, ?)",
				collection_id, absolutePath);
			return Folder(*db, *this, db->lastId(), absolutePath);
		}
		Folder getFolder(const String& dirname) {
			String absolutePath = sys::path::absolute((const char*)dirname);
			auto query = db->query(
				"SELECT video_folder_id FROM video_folder WHERE collection_id = ? AND absolute_path = ?",
				collection_id, absolutePath);
			int64_t id;
			size_t count = 0;
			while (query) {
				id = query.getInt64(0);
				++count;
				++query;
			}
			assert(count == 1);
			return Folder(*db, *this, id, absolutePath);
		}
		ArrayList<Folder> getFolders() {
			ArrayList<Folder> folders;
			auto query = db->query("SELECT video_folder_id, absolute_path FROM video_folder WHERE collection_id = ?", collection_id);
			while (query) {
				int64_t id = query.getInt64(0);
				String absolutePath = query.getText(1);
				folders.add(Folder(*db, *this, id, absolutePath));
				++query;
			}
			return folders;
		}
	};

	class Database {
	private:
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
			if (current.trim()) {
				out.add(current);
			}
		}
		void ensureDatabaseStructure() {
			ArrayList<String> queries;
			loadDatabaseStructureQueries("res/work/video/model.sql", queries);
			for (const String& sql: queries) db.run(sql);
		}
	private:
		sqlite::Dataabase db;
	public:
		explicit Database(const char* dbname): db(dbname) {
			ensureDatabaseStructure();
		}
		size_t countCollections() {
			auto query = db.query("SELECT COUNT(collection_id) FROM collection");
			size_t count = 0;
			while (query) {
				count = (size_t)query.getInt64(0);
				++query;
			}
			return count;
		}
		bool collectionExists(const String& name) {
			auto query = db.query("SELECT COUNT(collection_id) FROM collection WHERE collection_name = ?", name);
			int64_t count = 0;
			while (query) {
				count = query.getInt64(0);
				++query;
			}
			return count == 1;
		}
		VideoCollection createCollection(const String& name, const String& thumbnailExtension = "jpg") {
			db.run("INSERT INTO collection(collection_name, thumbnail_extension) VALUES(?, ?)", name, thumbnailExtension);
			return VideoCollection(db, db.lastId(), name, thumbnailExtension);
		}
		VideoCollection getCollection(const String& name) {
			auto query = db.query(
				"SELECT collection_id, collection_name, thumbnail_extension FROM collection WHERE collection_name = ?",
				name);
			int64_t id;
			String thumbnailExtension;
			int count = 0;
			while (query) {
				id = query.getInt64(0);
				thumbnailExtension = query.getText(2);
				++count;
				++query;
			}
			assert(count == 1);
			return VideoCollection(db, id, name, thumbnailExtension);
		}
	};
}

#endif //SRC_CIGMAR_DATABASE_HPP
