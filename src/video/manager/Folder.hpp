//
// Created by notoraptor on 17-11-22.
//

#ifndef SRC_CIGMAR_VIDEOMANAGER_HPP
#define SRC_CIGMAR_VIDEOMANAGER_HPP

#include <cigmar/classes/TreeSet.hpp>
#include <cigmar/classes/Stack.hpp>
#include <video/Video.hpp>
#include <video/manager/TableRow.hpp>
#include <video/manager/Report.hpp>
#include <video/manager/RowMapping.hpp>
#include <video/manager/DbVideo.hpp>

namespace cigmar::video::manager {
	using namespace cigmar;

	class Database;
	class Library;
	class Folder: public TableRow {
		Database& db;
		Library& library;
		int64_t video_folder_id;
		String absolute_path;
		RowMapping<DbVideo> videos;
		Report report;
		typedef typename RowMapping<DbVideo>::iterator_t iterator_t;
		void recordVideo(const cigmar::video::Video &video);
		void init();
	public:
		Folder(Database& database, Library& lib, int64_t id, const String& folder_path);
		size_t countVideos() const {return videos.size();};
		DbVideo& getVideo(const String& video_relative_path) {return videos.get(video_relative_path);};
		DbVideo& getVideo(int64_t video_id) {return videos.get(video_id);};
		void refresh();
		const String& getAbsolutePath() const {return absolute_path;}
		// iteration on videos
		iterator_t begin() {return videos.begin();};
		iterator_t end() {return videos.end();};
	};

}

#endif //SRC_CIGMAR_VIDEOMANAGER_HPP
