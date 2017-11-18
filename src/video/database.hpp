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
	class Database;

	struct Report: public Streamable {
		size_t total = 0;
		size_t added = 0;
		size_t removed = 0;
		void toStream(std::ostream& o) const override {
			o << "Report(total: " << total << ", added: " << added << ", removed: " << removed << ")";
		}
	};

	class PropertyType {
		friend class Database;
	private:
		int64_t integer;
		int64_t unsignedInteger;
		int64_t floating;
		int64_t text;
		HashMap<int64_t, String> dbMappingId;
		PropertyType(): dbMappingId() {}
		void read(sqlite::Dataabase& db) {
			HashMap<String, int64_t> dbMappingName;
			auto query = db.query("SELECT property_type_id, property_type_name FROM property_type");
			while (query) {
				int64_t id = query.getInt64(0);
				String name = query.getText(1);
				dbMappingId[id] = name;
				dbMappingName[name] = id;
				++query;
			}
			assert(dbMappingId.size() == 4);
			integer = dbMappingName.get("int");
			unsignedInteger = dbMappingName.get("uint");
			floating = dbMappingName.get("double");
			text = dbMappingName.get("string");
		}
	public:
		bool exists(int64_t id) const {return dbMappingId.contains(id);}
		int64_t integerType() const {return integer;}
		int64_t unsignedIntegerType() const {return unsignedInteger;}
		int64_t floatingType() const {return floating;}
		int64_t textType() const {return text;}
	};

	struct PropertyDefinition: public Streamable {
		int64_t id;
		String name;
		String defaultValue;
		int64_t type;
		void toStream(std::ostream& o) const override {
			// TODO: Not the best: it currently prints property type id instead of name.
			o << "VideoPropertyDefinition(id: " << id
			  << ", name: \"" << name
			  << "\", type: " << type
			  << ", defaultValue: \"" << defaultValue << "\")";
		}
	};

	class UniquePropertiesClass {
		friend class Database;
	private:
		// upd: unique properties definitions.
		HashMap<int64_t, PropertyDefinition> upd;
		sqlite::Dataabase& db;
		PropertyType& type;
		UniquePropertiesClass(sqlite::Dataabase& dataabase, PropertyType& propertyType):
			upd(), db(dataabase), type(propertyType) {}
		void update() {
			auto query = db.query(
				"SELECT unique_property_id, unique_property_name, default_value, property_type_id FROM unique_property");
			while (query) {
				PropertyDefinition def;
				def.id = query.getInt64(0);
				def.name = query.getText(1);
				def.defaultValue = query.isNull(2) ? "" : query.getText(2);
				def.type = query.getInt64(3);
				assert(type.exists(def.type));
				upd[def.id] = def;
				++query;
			}
		}
		void add(const String& name, int64_t type, const String& defaultValue = "") {
			db.run(
				"INSERT INTO unique_property(unique_property_name, default_value, property_type_id) VALUES(?, ?, ?)",
				name, defaultValue, type);
			PropertyDefinition def;
			def.id = db.lastId();
			def.name = name;
			def.defaultValue = defaultValue;
			def.type = type;
			upd[def.id] = def;
		}
	public:
		bool has(const String& name) {
			auto query = db.query("SELECT COUNT(unique_property_id) FROM unique_property WHERE unique_property_name = ?", name);
			int count;
			while (query) {
				count = query.getInt(0);
				++query;
			}
			return (bool)count;
		}
		void addInteger(const String& name, const String& defaultValue = "") {
			add(name, type.integerType(), defaultValue);
		}
		void addUnsignedInteger(const String& name, const String& defaultValue = "") {
			add(name, type.unsignedIntegerType(), defaultValue);
		}
		void addFloating(const String& name, const String& defaultValue = "") {
			add(name, type.floatingType(), defaultValue);
		}
		void addText(const String& name, const String& defaultValue = "") {
			add(name, type.textType(), defaultValue);
		}
		void remove(int64_t id) {
			if (upd.contains(id)) {
				db.run("DELETE FROM unique_property WHERE unique_property_id = ?", id);
				upd.remove(id);
			}
		}
	};

	class MultiplePropertiesClass {
		friend class Database;
	private:
		// mpd: multiple properties definitions.
		HashMap<int64_t, PropertyDefinition> mpd;
		sqlite::Dataabase& db;
		PropertyType& type;
		MultiplePropertiesClass(sqlite::Dataabase& database, PropertyType& propertyType):
			mpd(), db(database), type(propertyType) {}
		void update() {
			auto query = db.query(
				"SELECT multiple_property_id, multiple_property_name, property_type_id FROM multiple_property");
			while (query) {
				PropertyDefinition def;
				def.id = query.getInt64(0);
				def.name = query.getText(1);
				def.type = query.getInt64(2);
				// def.defaultValue = "";
				assert(type.exists(def.type));
				mpd[def.id] = def;
				++query;
			}
		}
		void add(const String& name, int64_t type) {
			db.run(
				"INSERT INTO multiple_property(multiple_property_name, property_type_id) VALUES(?, ?)",
				name, type);
			PropertyDefinition def;
			def.id = db.lastId();
			def.name = name;
			def.type = type;
			mpd[def.id] = def;
		}
	public:
		bool has(const String& name) {
			auto query = db.query("SELECT COUNT(multiple_property_id) FROM multiple_property WHERE multiple_property_name = ?", name);
			int count;
			while (query) {
				count = query.getInt(0);
				++query;
			}
			return (bool)count;
		}
		void addInteger(const String& name) {
			add(name, type.integerType());
		}
		void addUnsignedInteger(const String& name) {
			add(name, type.unsignedIntegerType());
		}
		void addFloating(const String& name) {
			add(name, type.floatingType());
		}
		void addText(const String& name) {
			add(name, type.textType());
		}
		void remove(int64_t id) {
			if (mpd.contains(id)) {
				db.run("DELETE FROM multiple_property WHERE multiple_property_id = ?", id);
				mpd.remove(id);
			}
		}
	};

	class Folder: public Streamable {
		friend class VideoCollection;
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
			sys::err::println(report);
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
		friend class Database;
	private:
		sqlite::Dataabase* db;
		int64_t collection_id;
		String collection_name;
		String thumbnail_extension;
		VideoCollection(sqlite::Dataabase& database,
						int64_t collectionId,
						const String& collectionName,
						const String& thumnailExtension):
			db(&database), collection_id(collectionId), collection_name(collectionName), thumbnail_extension(thumnailExtension) {
			Report report = update();
			sys::err::println("Collection:", collection_name);
			sys::err::println(report);
		}
		Report update() {
			Report report;
			auto query = db->query("SELECT video_folder_id, absolute_path FROM video_folder WHERE collection_id = ?", collection_id);
			ArrayList<int64_t> idx;
			ArrayList<String> paths;
			while (query) {
				idx.add(query.getInt64(0));
				paths.add(query.getText(1));
				++query;
			}
			report.total = paths.size();
			for (size_t i = 0; i < paths.size(); ++i) {
				if (!sys::path::isDirectory((const char*)paths[i])) {
					db->run("DELETE FROM video_folder WHERE video_folder_id = ?", idx[i]);
					++report.removed;
					--report.total;
				}
			}
			return report;
		}
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
		sqlite::Dataabase db;
		PropertyType propertyType;
	public:
		UniquePropertiesClass uniqueProperties;
		MultiplePropertiesClass multipleProperties;
	public:
		explicit Database(const char* dbname): db(dbname), propertyType(), uniqueProperties(db, propertyType), multipleProperties(db, propertyType) {
			ensureDatabaseStructure();
			propertyType.read(db);
			uniqueProperties.update();
			multipleProperties.update();
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
