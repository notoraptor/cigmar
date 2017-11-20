//
// Created by HPPC on 19/11/2017.
//

#ifndef SRC_CIGMAR_VIDEOMANAGER_HPP
#define SRC_CIGMAR_VIDEOMANAGER_HPP

#include <cstdint>
#include <cigmar/classes/String.hpp>
#include <cigmar/classes/HashSet.hpp>
#include <cigmar/classes/HashMap.hpp>
#include <cigmar/sqlite.hpp>
#include <cigmar/classes/Exception.hpp>
#include <cigmar/file/Lines.hpp>
#include <cassert>
#include <cigmar/classes/Queue.hpp>
#include <cigmar/classes/Stack.hpp>

namespace cigmar::videomanager {
	class DatabaseTable {
	public:
		virtual int64_t getId() = 0;
		virtual const String& getIdentifier() = 0;
	};
	template<typename T> class Initializer: public DatabaseTable {
		T object;
		bool initialized = false;
		void check() const {
			if (!initialized) throw Exception("This object is not initialized.");
		}
	public:
		Initializer(): object(), initialized(false) {}
		template<typename... Args>
		Initializer(Args&&... args): object(std::forward<Args>(args)...), initialized(true) {}
		explicit operator bool() const {return initialized;}
		template<typename... Args>
		void init(Args&&... args) {
			if (!initialized) {
				object.init(std::forward<Args>(args)...);
				initialized = true;
			}
		}
		T* operator->() {
			check();
			return &object;
		}
		const T* operator->() const {
			check();
			return &object;
		}
		int64_t getId() override {
			return object.getId();
		}
		const String& getIdentifier() override {
			return object.getIdentifier();
		}
	};
	template<typename T> class DatabaseMapping {
		HashMap<int64_t, T> data;
		HashMap<String, T*> identifiers;
	public:
		typedef typename HashSet<T>::iterator_t iterator_t;
		bool has(const T& value) {
			return data.contains(value.getId());
		}
		bool has(const int64_t id) {
			return data.contains(id);
		}
		bool has(const const String& identifier) {
			return identifiers.contains(identifier);
		}
		T& add(T& value) {
			if (!data.contains(value.getId())) {
				data.put(value.getId(), value);
				identifiers.put(value.getIdentifier(), &data[value.getId()]);
			}
			return data[value.getId()];
		}
		T& add(T&& value) {
			if (!data.contains(value.getId())) {
				data[value.getId()] = std::move(value);
				identifiers.put(value.getIdentifier(), &data[value.getId()]);
			}
			return data[value.getId()];
		}
		T& get(int64_t id) {
			return data[id];
		}
		T& get(const String& identifier) {
			return *identifiers[identifier];
		}
		iterator_t begin() {return data.begin();};
		iterator_t end() {return data.end();};
	};
	struct PropertyType {
		int64_t property_type_id;
		String property_type_name;
	};
	struct UniqueProperty: public DatabaseTable {
		int64_t unique_property_id;
		String unique_property_name;
		String default_value;
		const PropertyType* property_type;
	public:
		int64_t getId() override {return unique_property_id;}
		const String& getIdentifier() override {return unique_property_name;}
	};
	class MultipleProperty: public DatabaseTable {
		int64_t multiple_property_id;
		String multiple_property_name;
		PropertyType& property_type;
	public:
		int64_t getId() override {return multiple_property_id;}
		const String& getIdentifier() override {return multiple_property_name;}
	};
	namespace internal {
		class PropertyTypes {
			friend class cigmar::videomanager::Database;
			friend class cigmar::videomanager::UniqueProperties;
			PropertyType int_type;
			PropertyType uint_type;
			PropertyType double_type;
			PropertyType string_type;
			// internal
			const PropertyType& intType();
			const PropertyType& uintType();
			const PropertyType& doubleType();
			const PropertyType& stringType();
			HashMap<int64_t, PropertyType*> index;
			void init(sqlite::Dataabase& db) {
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
			const PropertyType* get(int64_t id) {
				return *index.get(id);
			}
		};
		class UniqueProperties {
			DatabaseMapping<UniqueProperty> properties;
			void add(const String& unique_property_name, const String& default_value, const PropertyType& property_type);
		public:
			void init(sqlite::Dataabase& db, PropertyTypes& types) {
				auto query = db.query(
					"SELECT unique_property_id, unique_property_name, default_value, property_type_id FROM unique_property");
				while (query) {
					properties.add(UniqueProperty{query.getInt64(0),
												  query.getText(1),
												  query.isNull(2) ? "" : query.getText(2),
												  types.get(query.getInt64(3))});
					++query;
				}
			}
			void addInteger(const String& unique_property_name, const String& default_value);
			void addUnsigned(const String& unique_property_name, const String& default_value);
			void addDouble(const String& unique_property_name, const String& default_value);
			void addString(const String& unique_property_name, const String& default_value);
			void remove(const String& unique_property_name);
			void remove(int64_t unique_property_id);
			const UniqueProperty& get(const String& unique_property_name);
			const UniqueProperty& get(int64_t unique_property_id);
		};
		class MultipleProperties {
			DatabaseMapping<MultipleProperty> properties;
			void add(const String& multiple_property_name, const PropertyType& property_type);
		public:
			void addInteger(const String& multiple_property_name);
			void addUnsigned(const String& multiple_property_name);
			void addDouble(const String& multiple_property_name);
			void addString(const String& multiple_property_name);
			void remove(const String& multiple_property_name);
			void remove(int64_t multiple_property_id);
			const MultipleProperty& get(const String& multiple_property_name);
			const MultipleProperty& get(int64_t multiple_property_id);
		};
		class Video: public DatabaseTable {
			int64_t video_id;
			int64_t date_added_microseconds;
			size_t video_size;
			String format;
			String audiocodec;
			String videocodec;
			double samplerate;
			double framerate;
			double duration;
			size_t width;
			size_t height;
			String relative_path;
			String video_hash;
			Folder& video_folder;
			String absolute_path;
			HashMap<UniqueProperty, String> unique_properties;
			HashMap<MultipleProperty, HashSet<String>> multiple_properties;
			bool hasUniqueProperty(const UniqueProperty& unique_property);
			bool hasMultipleProperty(const MultipleProperty& multiple_property);
			void setUniqueProperty(const UniqueProperty& unique_property, const String& value);
			void addMultipleProperty(const MultipleProperty& multiple_property, const String& value);
			void clearUniqueProperty(const UniqueProperty& unique_property);
			void clearMultipleProperty(const MultipleProperty& multiple_property);
			void removeMultiplePropertyValue(const MultipleProperty& multiple_property, const String& value);
			const String& getUniquePropertyValue(const UniqueProperty& unique_property);
			const HashSet<String>& getMultiplePropertyValues(const MultipleProperty& multiple_property);
			int64_t getId() override {return video_id;}
			const String& getIdentifier() override {return absolute_path;}
		};
		class Folder: public DatabaseTable {
			int64_t video_folder_id;
			String absolute_path;
			Library& library;
			DatabaseMapping<Video> videos;
			HashMap<int64_t, Video*> videosById;
			HashMap<String, Video*> videosByRelativePath;
			typedef typename DatabaseMapping<Video>::iterator_t iterator_t;
			size_t countVideos();
			Video& getVideo(const String& video_relative_path);
			Video& getVideo(int64_t video_id);
			void refresh();
			String getVideoAbsolutePath(const Video& video);
			// iteration on videos
			iterator_t begin();
			iterator_t end();
		};
		class Library: DatabaseTable {
			int64_t library_id;
			String library_name;
			String thumbnail_extension;
			DatabaseMapping<Folder> folders;
			typedef typename DatabaseMapping<Folder>::iterator_t iterator_t;
			int64_t getId() override {return library_id;}
			const String& getIdentifier() override {return library_name;}
			Library(int64_t id, const String& name, const String& thumbnailExtension):
				library_id(id), library_name(name), thumbnail_extension(thumbnailExtension), folders() {}
		public:
			bool hasFolder(const String& folder_path);
			size_t countFolders();
			Folder& addFolder(const String& folder_path);
			Folder& getFolder(const String& folder_path);
			// iteration on folders
			iterator_t begin();
			iterator_t end();
		};
	}
	typedef Initializer<internal::Library> Library;
	class Database {
		sqlite::Dataabase db;
		HashSet<String> library_names;
		DatabaseMapping<Library> libraries;
		internal::PropertyTypes propertyTypes;
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
			db.run("PRAGMA foreign_keys=ON");
			ArrayList<String> queries;
			loadDatabaseStructureQueries("res/work/video/model.sql", queries);
			for (const String& sql: queries) db.run(sql);
		}
	public:
		internal::UniqueProperties uniqueProperties;
		internal::MultipleProperties multipleProperties;
		explicit Database(const char* dbname):
			db(dbname), library_names(), libraries(), propertyTypes(), uniqueProperties(), multipleProperties() {
			ensureDatabaseStructure();
			auto query = db.query("SELECT library_name FROM library");
			while (query) {
				library_names.add(query.getText(0));
				++query;
			}
			propertyTypes.init(db);

		}
		bool hasLibrary(const String& libraryName) const {return library_names.contains(libraryName);};
		size_t countLibraries() const {return library_names.size();};
		const HashSet<String>& getLibraryNames() const {return library_names;};
		Library& addLibrary(const String& name, const String& thumbnailExtension) {
			db.run("INSERT INTO library(library_name, thumbnail_extension) VALUES(?, ?)", name, thumbnailExtension);
			return libraries.add(Library(db, db.lastId(), name, thumbnailExtension));
		};
		Library& getLibrary(const String& libraryName) {
			return libraries.get(libraryName);
		};
	};
}

#endif //SRC_CIGMAR_VIDEOMANAGER_HPP
