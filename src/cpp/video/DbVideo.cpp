//
// Created by notoraptor on 17-11-23.
//
#include <cigmar/video/manager/DbVideo.hpp>
#include <cigmar/video/manager/Database.hpp>
#include <cigmar/video/manager/Folder.hpp>
namespace cigmar::video::manager {
	void DbVideo::loadProperties() {
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
			if (multiple_properties.contains(&pp)) {
				multiple_properties[&pp].add(query.getText(1));
			} else {
				multiple_properties.put(&pp, {query.getText(1)});
			}
			++query;
		}
	}
	DbVideo::DbVideo(Database& database, const String& absoluteParent, int64_t id, const cigmar::video::Video& diskVideo):
		db(database), absolute_parent(absoluteParent), video_id(id), relative_path(), video(diskVideo),
		unique_properties(), multiple_properties() {
		relative_path = sys::path::relative(absolute_parent, video.getAbsolutePath());
		setId(video_id);
		setKey(relative_path);
		loadProperties();
	}
	void DbVideo::setUniqueProperty(const UniqueProperty* unique_property, const String& value) {
		if (unique_properties.contains(unique_property)) {
			db->run("UPDATE unique_property_to_video SET value = ? WHERE video_id = ? AND unique_property_id = ?",
					value, video_id, unique_property->unique_property_id);
			unique_properties[unique_property] = value;
		} else {
			db->run("INSERT INTO unique_property_to_video (video_id, unique_property_id, value) VALUES(?, ?, ?)",
					video_id, unique_property->unique_property_id, value);
			unique_properties.put(unique_property, value);
		}
	};
	void DbVideo::addMultipleProperty(const MultipleProperty& multiple_property, const String& value) {
		if (!multiple_properties.contains(&multiple_property) || !multiple_properties[&multiple_property].contains(value)) {
			db->run("INSERT INTO multiple_property_to_video (video_id, multiple_property_id, value) VALUES(?, ?, ?)",
					video_id, multiple_property.multiple_property_id, value);
			multiple_properties[&multiple_property].add(value);
		}
	};
	void DbVideo::clearUniqueProperty(const UniqueProperty* unique_property) {
		if (unique_properties.contains(unique_property)) {
			db->run("DELETE FROM unique_property_to_video WHERE video_id = ? AND unique_property_id = ?",
					video_id, unique_property->unique_property_id);
			unique_properties.remove(unique_property);
		}
	};
	void DbVideo::clearMultipleProperty(const MultipleProperty* multiple_property) {
		if (multiple_properties.contains(multiple_property)) {
			db->run("DELETE FROM multiple_property_to_video WHERE video_id = ? AND multiple_property_id = ?",
					video_id, multiple_property->multiple_property_id);
			multiple_properties.remove(multiple_property);
		}
	};
	void DbVideo::removeMultiplePropertyValue(const MultipleProperty* multiple_property, const String& value) {
		if (multiple_properties.contains(multiple_property) && multiple_properties[multiple_property].contains(value)) {
			db->run("DELETE FROM multiple_property_to_video WHERE video_id = ? AND multiple_property_id = ? AND value = ?",
					video_id, multiple_property->multiple_property_id, value);
			multiple_properties[multiple_property].remove(value);
		}
	};
}
