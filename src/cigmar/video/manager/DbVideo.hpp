//
// Created by notoraptor on 17-11-23.
//

#ifndef SRC_CIGMAR_VIDEO_HPP
#define SRC_CIGMAR_VIDEO_HPP

#include <cigmar/classes/HashMap.hpp>
#include <cigmar/classes/HashSet.hpp>
#include <cigmar/video/manager/TableRow.hpp>
#include <cigmar/video/manager/UniqueProperty.hpp>
#include <cigmar/video/manager/MultipleProperty.hpp>
#include <cigmar/video/Video.hpp>

namespace cigmar::video::manager {
	class Database;
	class Folder;
	class DbVideo: public TableRow {
		Database& db;
		String absolute_parent;
		int64_t video_id;
		String relative_path;
		cigmar::video::Video video;
		HashMap<const UniqueProperty*, String> unique_properties;
		HashMap<const MultipleProperty*, HashSet<String>> multiple_properties;
		void loadProperties();
	public:
		DbVideo(Database& database, const String& absoluteParent, int64_t id, const cigmar::video::Video& diskVideo);
		bool hasUniqueProperty(const UniqueProperty& unique_property) const {
			return unique_properties.contains(&unique_property);
		};
		bool hasMultipleProperty(const MultipleProperty& multiple_property) const {
			return multiple_properties.contains(&multiple_property);
		};
		bool hasMultiplePropertyValue(const MultipleProperty& multiple_property, const String& value) const {
			return multiple_properties.contains(&multiple_property) && multiple_properties.get(&multiple_property).contains(value);
		}
		void setUniqueProperty(const UniqueProperty* unique_property, const String& value);
		void addMultipleProperty(const MultipleProperty& multiple_property, const String& value);
		void clearUniqueProperty(const UniqueProperty* unique_property);
		void clearMultipleProperty(const MultipleProperty* multiple_property);
		void removeMultiplePropertyValue(const MultipleProperty* multiple_property, const String& value);
		const String& getUniquePropertyValue(const UniqueProperty* unique_property) {
			if (!unique_properties.contains(unique_property))
				throw Exception("No unique property '", unique_property->unique_property_name,
								"' for video '", video.getAbsolutePath(), "'");
			return unique_properties[unique_property];
		};
		const HashSet<String>& getMultiplePropertyValues(const MultipleProperty& multiple_property) {
			if (!multiple_properties.contains(&multiple_property))
				throw Exception("No multiple property '", multiple_property.multiple_property_name,
								"' for video '", video.getAbsolutePath(), "'");
		};
		cigmar::video::Video* operator->() {return &video;}
		const cigmar::video::Video* operator->() const {return &video;}
		bool operator==(const DbVideo& other) const {
			return video_id == other.video_id;
		}
	};
}
#endif //SRC_CIGMAR_VIDEO_HPP
