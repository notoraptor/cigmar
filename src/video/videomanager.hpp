//
// Created by HPPC on 19/11/2017.
//

#ifndef SRC_CIGMAR_VIDEOMANAGER_HPP
#define SRC_CIGMAR_VIDEOMANAGER_HPP

#include <cstdint>
#include <cigmar/classes/String.hpp>
#include <cigmar/classes/HashSet.hpp>
#include <cigmar/classes/HashMap.hpp>

namespace cigmar::videomanager {
	class DatabaseTable {
		virtual int64_t getId() = 0;
		virtual const String& getIdentifier() = 0;
	};
	template<typename T>
	class DatabaseMapping {
		HashSet<T> data;
		HashMap<int64_t, T*> indices;
		HashMap<String, T*> identifiers;
	public:
		typedef typename HashSet<T>::iterator_t iterator_t;
		bool has(const T& value) {
			return data.contains(value);
		}
		bool has(const int64_t id) {
			return indices.contains(id);
		}
		bool has(const const String& identifier) {
			return identifiers.contains(identifier);
		}
		void add(const T& value) {
			if (!data.contains(value)) {
				data.add(value);
				indices[value.getId()] = &value;
				identifiers[value.getIdentifier()] = &value;
			}
		}
		T& get(int64_t id) {
			return indices[id];
		}
		T& get(const String& identifier) {
			return identifiers[identifier];
		}
		iterator_t begin();
		iterator_t end();
	};
	class PropertyType {
		int64_t property_type_id;
		String property_type_name;
	};
	class PropertyTypes {
		// temporar
		HashMap<String, PropertyType> types;
		// internal
		const PropertyType& intType();
		const PropertyType& uintType();
		const PropertyType& doubleType();
		const PropertyType& stringType();
	};
	class UniqueProperty: public DatabaseTable {
		int64_t unique_property_id;
		String unique_property_name;
		String default_value;
		PropertyType& property_type;
	};
	class MultipleProperty: public DatabaseTable {
		int64_t multiple_property_id;
		String multiple_property_name;
		PropertyType& property_type;
	};
	class UniqueProperties {
		DatabaseMapping<UniqueProperty> properties;
		void add(const String& unique_property_name, const String& default_value, const PropertyType& property_type);
		// public
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
		// public
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
	};
	class Folder: public DatabaseTable {
		int64_t video_folder_id;
		String absolute_path;
		Library& library;
		size_t countVideos();
		DatabaseMapping<Video> videos;
		HashMap<int64_t, Video*> videosById;
		HashMap<String, Video*> videosByRelativePath;
		typedef typename DatabaseMapping<Video>::iterator_t iterator_t;
		Video& getVideo(const String& video_relative_path);
		Video& getVideo(int64_t video_id);
		void refresh();
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
		bool hasFolder(const String& folder_path);
		size_t countFolders();
		Folder& addFolder(const String& folder_path);
		Folder& getFolder(const String& folder_path);
		// iteration on folders
		iterator_t begin();
		iterator_t end();
	};
	class Database {
		HashSet<String> library_names;
		DatabaseMapping<Library> libraries;
		PropertyTypes propertyTypes;
		// public
		UniqueProperties uniqueProperties;
		MultipleProperties multipleProperties;
		bool hasLibrary(const String& libraryPath);
		size_t countLibraries();
		Library& addLibrary(const String& libraryName);
		Library& getLibrary(const String& libraryName);
		const HashSet<String>& getLibraryNames();
	};
}

#endif //SRC_CIGMAR_VIDEOMANAGER_HPP
