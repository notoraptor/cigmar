//
// Created by notoraptor on 17-11-22.
//

#ifndef SRC_CIGMAR_VIDEOMANAGER_HPP
#define SRC_CIGMAR_VIDEOMANAGER_HPP

#include <cigmar/utils/Initializable.hpp>
#include <cigmar/classes/String.hpp>
#include <cigmar/classes/HashMap.hpp>
#include <cigmar/classes/HashSet.hpp>
#include <cigmar/classes/TreeSet.hpp>
#include <cigmar/classes/Stack.hpp>
#include <cigmar/file/Lines.hpp>
#include <cigmar/sqlite.hpp>
#include <cigmar/filesystem.hpp>
#include <cassert>
#include <video/Video.hpp>

namespace videomanager {
	using namespace cigmar;

	void collect(const char *dirpath, const std::function<void(const cigmar::video::Video&)>& collector) {
		sys::Dir dir(dirpath);
		for (const String pathname: dir) {
			if (pathname != "." && pathname != "..") {
				String path = sys::path::join(dirpath, pathname);
				if (sys::path::isDirectory((const char *) path)) {
					collect((const char *) path, collector);
				} else {
					String extension = sys::path::extension((const char *) pathname);
					if (cigmar::video::Video::extensionIsSupported(extension.lower())) {
						collector(cigmar::video::Video(sys::path::join(dirpath, pathname)));
					}
				}
			}
		}
	}

	void collectRelativePaths(const char *dirpath, const std::function<void(const String&)>& collector) {
		sys::Dir dir(dirpath);
		for (const String pathname: dir) {
			if (pathname != "." && pathname != "..") {
				const String path = sys::path::join(dirpath, pathname);
				if (sys::path::isDirectory((const char *) path)) {
					collectRelativePaths((const char *) path, collector);
				} else {
					String extension = sys::path::extension((const char *) pathname);
					if (cigmar::video::Video::extensionIsSupported(extension.lower())) {
						collector(path);
					}
				}
			}
		}
	}

	struct Report: public Streamable {
		size_t total = 0;
		size_t added = 0;
		size_t removed = 0;
		void toStream(std::ostream& o) const override {
			o << "Report(total: " << total << ", added: " << added << ", removed: " << removed << ")";
		}
	};

	class TableRow {
	private:
		int64_t m_id;
		String m_key;
	protected:
		void setId(int64_t value) {
			m_id = value;
		}
		void setKey(const String& value) {
			m_key = value;
		}
		void setKey(String&& moved) {
			m_key = std::move(moved);
		}
	public:
		int64_t id() const {return m_id;}
		const String& key() const {return m_key;}
	};

	template<typename T>
	class RowMapping {
		static_assert(std::is_base_of<TableRow, T>{}, "We can only map a TableRow (or derived class) instance.");
	private:
		HashSet<T> values;
		HashMap<int64_t, T*> fromIndices;
		HashMap<String, T*> fromKeys;
	public:
		typedef typename Stack<T>::iterator_t iterator_t;
		typedef typename Stack<T>::const_iterator_t const_iterator_t;
		size_t size() const {return values.size();}
		bool contains(const T& value) const {return fromIndices.contains(value.id());}
		bool contains(int64_t id) const {return fromIndices.contains(id);}
		bool contains(const String& key) const {return fromKeys.contains(key);}
		T& add(const T& value) {
			if (!fromIndices.contains(value.id())) {
				T& added = *values.add(value);
				fromIndices.put(added.id(), &added);
				fromKeys.put(added.key(), &added);
			}
			return fromIndices.get(value.id());
		}
		T& add(T&& value) {
			if (!fromIndices.contains(value.id())) {
				T& added = *values.add(std::move(value));
				fromIndices.put(added.id(), &added);
				fromKeys.put(added.key(), &added);
			}
			return fromIndices.get(value.id());
		}
		T& get(int64_t id) {return *fromIndices.get(id);}
		T& get(const String& key) {return *fromKeys.get(key);}
		const T& get(int64_t id) const {return *fromIndices.get(id);}
		const T& get(const String& key) const {return *fromKeys.get(key);}
		void remove(const String& key) {
			if (fromKeys.contains(key)) {
				T* ptr = fromKeys.get(key);
				fromKeys.remove(key);
				fromIndices.remove(ptr->id());
				values.remove(*ptr);
			}
		}
		void remove(int64_t id) {
			if (fromIndices.contains(id)) {
				T* ptr = fromIndices.get(id);
				fromKeys.remove(ptr->key());
				fromIndices.remove(id);
				values.remove(*ptr);
			}
		}
		iterator_t begin() {return values.begin();}
		iterator_t end() {return values.end();}
		const_iterator_t begin() const {return values.begin();}
		const_iterator_t end() const {return values.end();}
	};

	struct PropertyType {
		int64_t property_type_id;
		String property_type_name;
	};

	struct UniqueProperty: public TableRow, public Hashable {
		int64_t unique_property_id;
		String unique_property_name;
		String default_value;
		const PropertyType& property_type;
		UniqueProperty(int64_t propertyId, const String& propertyName,
					   const String& defaultValue, const PropertyType& type):
			unique_property_id(propertyId), unique_property_name(propertyName),
			default_value(defaultValue), property_type(type) {
			setId(unique_property_id);
			setKey(unique_property_name);
		}
		size_t hash() const override {
			return (size_t)unique_property_id;
		}
	};

	struct MultipleProperty: public TableRow, public Hashable {
		int64_t multiple_property_id;
		String multiple_property_name;
		const PropertyType& property_type;
	public:
		MultipleProperty(int64_t propertyId, const String& propertyName, const PropertyType& type):
			multiple_property_id(propertyId), multiple_property_name(propertyName), property_type(type) {
			setId(multiple_property_id);
			setKey(multiple_property_name);
		}
		size_t hash() const override {
			return (size_t)multiple_property_id;
		}
	};

	class PropertyTypes {
		PropertyType int_type;
		PropertyType uint_type;
		PropertyType double_type;
		PropertyType string_type;
		HashMap<int64_t, PropertyType*> index;
	public:
		void init(sqlite::Database& db) {
			HashMap<String, int64_t> mapping;
			auto query = db.query("SELECT property_type_id, property_type_name FROM property_type");
			size_t count = 0;
			while (query) {
				int64_t id = query.getInt64(0);
				String name = query.getText(1);
				mapping[name] = id;
				++query;
				++count;
			}
			assert(count == 4);

			auto it = mapping.iterator("int");
			int_type = {it->second, it->first};
			index[int_type.property_type_id] = &int_type;

			it = mapping.iterator("uint");
			uint_type = {it->second, it->first};
			index[uint_type.property_type_id] = &uint_type;

			it = mapping.iterator("double");
			double_type = {it->second, it->first};
			index[double_type.property_type_id] = &double_type;

			it = mapping.iterator("string");
			string_type = {it->second, it->first};
			index[string_type.property_type_id] = &string_type;
		}
		const PropertyType& get(int64_t id) const {
			return *index.get(id);
		}
		const PropertyType& integer() const {return int_type;}
		const PropertyType& unsignedInteger() const {return uint_type;}
		const PropertyType& floating() const {return double_type;}
		const PropertyType& text() const {return string_type;}
	};

	class UniqueProperties {
		RowMapping<UniqueProperty> properties;
		sqlite::Database* db = nullptr;
		PropertyTypes* types = nullptr;
		void add(const String& unique_property_name, const String& default_value, const PropertyType& property_type) {
			int countMultipleProperties = 0;
			auto query = db->query(
				"SELECT COUNT(multiple_property_id) FROM multiple_property WHERE multiple_property_name = ?",
				unique_property_name);
			while (query) {
				countMultipleProperties = query.getInt(0);
				++query;
			}
			if (countMultipleProperties)
				throw Exception("Cannot create a (unique) property with name \"",
								unique_property_name, "\": a multiple property already has this name.");
			db->run("INSERT INTO unique_property(unique_property_name, default_value, property_type_id) VALUES(?, ?, ?)",
					unique_property_name, default_value, property_type.property_type_id);
			properties.add(UniqueProperty(db->lastId(), unique_property_name, default_value, property_type));
		};
	public:
		void init(sqlite::Database& database, PropertyTypes& propertyTypes) {
			db = &database;
			types = &propertyTypes;
			auto query = db->query(
				"SELECT unique_property_id, unique_property_name, default_value, property_type_id FROM unique_property");
			while (query) {
				properties.add(UniqueProperty(query.getInt64(0), query.getText(1),
											  query.isNull(2) ? "" : query.getText(2),
											  types->get(query.getInt64(3))));
				++query;
			}
		}
		void addInteger(const String& unique_property_name, const String& default_value = "") {
			add(unique_property_name, default_value, types->integer());
		};
		void addUnsigned(const String& unique_property_name, const String& default_value = "") {
			add(unique_property_name, default_value, types->unsignedInteger());
		};
		void addDouble(const String& unique_property_name, const String& default_value = "") {
			add(unique_property_name, default_value, types->floating());
		};
		void addString(const String& unique_property_name, const String& default_value = "") {
			add(unique_property_name, default_value, types->text());
		};
		void remove(const String& unique_property_name) {
			if (properties.contains(unique_property_name)) {
				db->run("DELETE FROM unique_property WHERE unique_property_name = ?", unique_property_name);
				properties.remove(unique_property_name);
			}
		};
		void remove(int64_t unique_property_id) {
			if (properties.contains(unique_property_id)) {
				db->run("DELETE FROM unique_property WHERE unique_property_id = ?", unique_property_id);
				properties.remove(unique_property_id);
			}
		};
		const UniqueProperty& get(const String& unique_property_name) const {
			return properties.get(unique_property_name);
		};
		const UniqueProperty& get(int64_t unique_property_id) const {
			return properties.get(unique_property_id);
		};
	};

	class MultipleProperties {
		RowMapping<MultipleProperty> properties;
		sqlite::Database* db = nullptr;
		PropertyTypes* types = nullptr;
		void add(const String& multiple_property_name, const PropertyType& property_type) {
			int countUniqueProperties = 0;
			auto query = db->query(
				"SELECT COUNT(unique_property_id) FROM unique_property WHERE unique_property_name = ?",
				multiple_property_name);
			while (query) {
				countUniqueProperties = query.getInt(0);
				++query;
			}
			if (countUniqueProperties)
				throw Exception("Cannot create a (multiple) property with name \"",
								multiple_property_name, "\": an unique property already has this name.");
			db->run(
				"INSERT INTO multiple_property(multiple_property_name, property_type_id) VALUES(?, ?)",
				multiple_property_name, property_type.property_type_id);
			properties.add(MultipleProperty(db->lastId(), multiple_property_name, property_type));
		};
	public:
		void init(sqlite::Database& database, PropertyTypes& propertyTypes) {
			db = &database;
			types = &propertyTypes;
			auto query = db->query(
				"SELECT multiple_property_id, multiple_property_name, property_type_id FROM multiple_property");
			while (query) {
				properties.add(MultipleProperty(query.getInt64(0), query.getText(1), types->get(query.getInt64(2))));
				++query;
			}
		}
		void addInteger(const String& multiple_property_name) {
			add(multiple_property_name, types->integer());
		};
		void addUnsigned(const String& multiple_property_name) {
			add(multiple_property_name, types->unsignedInteger());
		};
		void addDouble(const String& multiple_property_name) {
			add(multiple_property_name, types->floating());
		};
		void addString(const String& multiple_property_name) {
			add(multiple_property_name, types->floating());
		};
		void remove(const String& multiple_property_name) {
			if (properties.contains(multiple_property_name)) {
				db->run("DELETE FROM multiple_property WHERE multiple_property_name = ?", multiple_property_name);
				properties.remove(multiple_property_name);
			}
		};
		void remove(int64_t multiple_property_id) {
			if (properties.contains(multiple_property_id)) {
				db->run("DELETE FROM multiple_property WHERE multiple_property_id = ?", multiple_property_id);
				properties.remove(multiple_property_id);
			}
		};
		const MultipleProperty& get(const String& multiple_property_name) const {
			return properties.get(multiple_property_name);
		};
		const MultipleProperty& get(int64_t multiple_property_id) const {
			return properties.get(multiple_property_id);
		};
	};

	class Video: public TableRow {
		Database& db;
		Folder& video_folder;
		int64_t video_id;
		String relative_path;
		cigmar::video::Video video;
		HashMap<const UniqueProperty&, String> unique_properties;
		HashMap<const MultipleProperty&, HashSet<String>> multiple_properties;
		void loadProperties() {
			auto query = db->query(
				"SELECT unique_property_id, value FROM unique_property_to_video WHERE video_id = ?", video_id);
			while (query) {
				unique_properties.put(db.monoprops().get(query.getInt64(0)), query.getText(1));
				++query;
			}
			query = db->query(
				"SELECT multiple_property_id, value FROM multiple_property_to_video WHERE video_id = ?", video_id);
			while (query) {
				const MultipleProperty& pp = db.polyprops().get(query.getInt64(0));
				if (multiple_properties.contains(pp)) {
					multiple_properties[pp].add(query.getText(1));
				} else {
					multiple_properties.put(pp, {query.getText(1)});
				}
				++query;
			}
		}
	public:
		Video(Database& database, Folder& folder, int64_t id, const cigmar::video::Video& diskVideo):
			db(database), video_folder(folder), video_id(id), video(diskVideo),
			relative_path(sys::path::relativePath(folder.getAbsolutePath(), video.getAbsolutePath())),
			unique_properties(), multiple_properties() {
			setId(video_id);
			setKey(relative_path);
			loadProperties();
		}
		bool hasUniqueProperty(const UniqueProperty& unique_property) const {
			return unique_properties.contains(unique_property);
		};
		bool hasMultipleProperty(const MultipleProperty& multiple_property) const {
			return multiple_properties.contains(multiple_property);
		};
		void setUniqueProperty(const UniqueProperty& unique_property, const String& value) {
			if (unique_properties.contains(unique_property)) {
				db->run("UPDATE unique_property_to_video SET value = ? WHERE video_id = ? AND unique_property_id = ?",
						value, video_id, unique_property.unique_property_id);
				unique_properties[unique_property] = value;
			} else {
				db->run("INSERT INTO unique_property_to_video (video_id, unique_property_id, value) VALUES(?, ?, ?)",
						video_id, unique_property.unique_property_id, value);
				unique_properties.put(unique_property, value);
			}
		};
		void addMultipleProperty(const MultipleProperty& multiple_property, const String& value) {
			if (!multiple_properties.contains(multiple_property) || !multiple_properties[multiple_property].contains(value)) {
				db->run("INSERT INTO multiple_property_to_video (video_id, multiple_property_id, value) VALUES(?, ?, ?)",
						video_id, multiple_property.multiple_property_id, value);
				multiple_properties[multiple_property].add(value);
			}
		};
		void clearUniqueProperty(const UniqueProperty& unique_property) {
			if (unique_properties.contains(unique_property)) {
				db->run("DELETE FROM unique_property_to_video WHERE video_id = ? AND unique_property_id = ?",
						video_id, unique_property.unique_property_id);
				unique_properties.remove(unique_property);
			}
		};
		void clearMultipleProperty(const MultipleProperty& multiple_property) {
			if (multiple_properties.contains(multiple_property)) {
				db->run("DELETE FROM multiple_property_to_video WHERE video_id = ? AND multiple_property_id = ?",
						video_id, multiple_property.multiple_property_id);
				multiple_properties.remove(multiple_property);
			}
		};
		void removeMultiplePropertyValue(const MultipleProperty& multiple_property, const String& value) {
			if (multiple_properties.contains(multiple_property) && multiple_properties[multiple_property].contains(value)) {
				db->run("DELETE FROM multiple_property_to_video WHERE video_id = ? AND multiple_property_id = ? AND value = ?",
						video_id, multiple_property.multiple_property_id, value);
				multiple_properties[multiple_property].remove(value);
			}
		};
		const String& getUniquePropertyValue(const UniqueProperty& unique_property) {
			if (!unique_properties.contains(unique_property))
				throw Exception("No unique property '", unique_property.unique_property_name,
								"' for video '", video.getAbsolutePath(), "'");
			return unique_properties[unique_property];
		};
		const HashSet<String>& getMultiplePropertyValues(const MultipleProperty& multiple_property) {
			if (!multiple_properties.contains(multiple_property))
				throw Exception("No multiple property '", multiple_property.multiple_property_name,
								"' for video '", video.getAbsolutePath(), "'");
		};
		const cigmar::video::Video onDisk() const {return video;}
	};

	class Folder: public TableRow {
		Database& db;
		Library& library;
		int64_t video_folder_id;
		String absolute_path;
		RowMapping<Video> videos;
		Report report;
		typedef typename RowMapping<Video>::iterator_t iterator_t;
		void recordVideo(const cigmar::video::Video &video) {
			db->run(R"(INSERT INTO video (
                       date_added_microseconds, video_size, format, audiocodec, videocodec, samplerate, framerate,
                       duration, width, height, relative_path, video_hash, video_folder_id
                       ) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?))",
					video.getDateAddedMicroseconds(), video.getSize(), video.getFormat(),
					video.getAudioCodec(), video.getVideoCodec(), video.getSampleRate(), video.getFrameRate(),
					video.getDuration(), video.getWidth(), video.getHeight(),
					sys::path::relativePath(absolute_path, video.getAbsolutePath()),
					video.getAbsolutePathHash(), video_folder_id
			);
			videos.add(Video(db, *this, db->lastId(), video));
		}
		void init() {
			TreeSet<cigmar::video::Video> diskVideos;
			collect((const char*)absolute_path,
					[&diskVideos](const cigmar::video::Video& video) {
						diskVideos.add(video);
					});
			report.total = diskVideos.size();
			auto query = db->query("SELECT video_id, relative_path FROM video WHERE video_folder_id = ?",
								   video_folder_id);
			HashMap<String, int64_t> videosAbsolutePathsFromDb;
			HashSet<String> videosAbsolutePathsFromDisk;
			while (query) {
				String videoAbsolutePath =
					sys::path::absolute((const char*)sys::path::join(absolute_path, query.getText(1)));
				videosAbsolutePathsFromDb[videoAbsolutePath] = query.getInt64(0);
				++query;
			}
			for (const auto& video: diskVideos) {
				const String& videoAbsolutePath = video.getAbsolutePath();
				videosAbsolutePathsFromDisk.add(videoAbsolutePath);
				if (videosAbsolutePathsFromDb.contains(videoAbsolutePath)) {
					// Load database video.
					videos.add(Video(db, *this, videosAbsolutePathsFromDb.get(videoAbsolutePath), video));
				} else {
					// We must add this video to the database.
					recordVideo(video);
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
		}
	public:
		Folder(Database& database, Library& lib, int64_t id, const String& folder_path):
			db(database), library(lib), video_folder_id(id), absolute_path(folder_path), videos(), report() {
			setId(video_folder_id);
			setKey(absolute_path);
			init();
		}
		size_t countVideos() const {return videos.size();};
		Video& getVideo(const String& video_relative_path) {return videos.get(video_relative_path);};
		Video& getVideo(int64_t video_id) {return videos.get(video_id);};
		void refresh() {
			HashSet<String> relativePaths;
			report.total = report.added = report.removed = 0;
			collectRelativePaths((const char*)absolute_path,
								 [&relativePaths](const String& path) {
									 relativePaths.add(sys::path::relativePath(absolute_path, path));
								 });
			report.total = relativePaths.size();
			for (const String& relativePath: relativePaths) {
				if (!videos.contains(relativePath)) {
					// We must add this video to the database.
					cigmar::video::Video video(sys::path::join(absolute_path, relativePath));
					recordVideo(video);
					++report.added;
				}
			}
			Stack<const Video*> toRemove;
			for (const Video& video: videos) {
				if (!relativePaths.contains(video.key())) {
					toRemove << (&video);
				}
			}
			for (const Video* video: toRemove) {
				db->run("DELETE FROM video WHERE video_id = ?", video->id());
				videos.remove(video->id());
				++report.removed;
			}
		};
		const String& getAbsolutePath() const {return absolute_path;}
		// iteration on videos
		iterator_t begin() {return videos.begin();};
		iterator_t end() {return videos.end();};
	};

	class Library: TableRow {
		Database& db;
		int64_t library_id;
		String library_name;
		String thumbnail_extension;
		RowMapping<Folder> folders;
		typedef typename RowMapping<Folder>::iterator_t iterator_t;
	public:
		Library(Database& database, int64_t id, const String& name, const String& thumbnailExtension):
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
		bool hasFolder(const String& folder_path) const {
			return folders.contains(sys::path::absolute((const char*)folder_path));
		};
		bool hasFolder(int64_t folderId) const {
			return folders.contains(folderId);
		}
		size_t countFolders() const {return folders.size();};
		Folder& addFolder(const String& folder_path) {
			String folderPath = sys::path::absolute((const char*)folder_path);
			if (hasFolder(folderPath))
				throw Exception("Library '", library_name, "': folder '", folderPath, "' already registered.");
			db->run("INSERT INTO video_folder(absolute_path, library_id) VALUES(?, ?)", folderPath, library_id);
			return folders.add(Folder(db, *this, db->lastId(), folderPath));
		};
		Folder& getFolder(const String& folder_path) {
			return folders.get(folder_path);
		};
		Folder& getFolder(int64_t folderId) {
			return folders.get(folderId);
		}
		void removeFolder(const String& folderPath) {
			String absolutePath = sys::path::absolute((const char*)folderPath);
			if (hasFolder(folderPath)) {
				db->run("DELETE FROM video_folder WHERE absolute_path = ?", absolutePath);
				folders.remove(absolutePath);
			}
		}
		void removeFolder(int64_t folderId) {
			if (hasFolder(folderId)) {
				db->run("DELETE FROM video_folder WHERE video_folder_id = ?", folderId);
				folders.remove(folderId);
			}
		}
		// iteration on folders
		iterator_t begin() {return folders.begin();};
		iterator_t end() {return folders.end();};
	};

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
		explicit Database(const char* dbname):
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
		bool hasLibrary(const String& libraryName) const {return library_names.contains(libraryName);};
		size_t countLibraries() const {return library_names.size();};
		const HashSet<String>& getLibraryNames() const {return library_names;};
		Library& addLibrary(const String& name, const String& thumbnailExtension) {
			if (hasLibrary(name))
				throw Exception("A library with name \"", name, "\" already exists.");
			database.run("INSERT INTO library(library_name, thumbnail_extension) VALUES(?, ?)", name, thumbnailExtension);
			return libraries.add(Library(*this, database.lastId(), name, thumbnailExtension));
		};
		Library& getLibrary(const String& libraryName) {
			return libraries.get(libraryName);
		};
		void removeLibrary(const String& libraryName) {
			if (hasLibrary(libraryName)) {
				database.run("DELETE FROM library WHERE library_name = ?", libraryName);
				libraries.remove(libraryName);
			}
		}
		sqlite::Database& db() {return database;};
		UniqueProperties& monoprops() {return uniqueProperties;}
		MultipleProperties& polyprops() {return multipleProperties;}
		sqlite::Database* operator->() {return &database;}
	};

}

#endif //SRC_CIGMAR_VIDEOMANAGER_HPP
