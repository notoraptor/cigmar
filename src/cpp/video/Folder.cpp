//
// Created by notoraptor on 17-11-22.
//

#include <cigmar/video/manager/Folder.hpp>
#include <cigmar/video/manager/Database.hpp>
namespace cigmar::video::manager {
	void Folder::recordVideo(const cigmar::video::Video &video) {
		db->run(R"(INSERT INTO video (
                       date_added_microseconds, video_size, format, audiocodec, videocodec, samplerate, framerate,
                       duration, width, height, relative_path, video_hash, video_folder_id
                       ) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?))",
				video.getDateAddedMicroseconds(), video.getSize(), video.getFormat(),
				video.getAudioCodec(), video.getVideoCodec(), video.getSampleRate(), video.getFrameRate(),
				video.getDuration(), video.getWidth(), video.getHeight(),
				sys::path::relative(absolute_path, video.getAbsolutePath()),
				video.getAbsolutePathHash(), video_folder_id
		);
		videos.add(DbVideo(db, absolute_path, db->lastId(), video));
	}
	void Folder::init() {
		TreeSet<cigmar::video::Video> diskVideos;
		cigmar::video::Video::collect((const char*)absolute_path,
									  [&diskVideos](cigmar::video::Video&& video) {
										  diskVideos.add(std::move(video));
									  });
		report.total = diskVideos.size();
		HashMap<String, int64_t> videosAbsolutePathsFromDb;
		HashSet<String> videosAbsolutePathsFromDisk;
		auto query = db->query("SELECT video_id, relative_path FROM video WHERE video_folder_id = ?", video_folder_id);
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
				videos.add(DbVideo(db, absolute_path, videosAbsolutePathsFromDb.get(videoAbsolutePath), video));
			} else {
				// We must add this video to the database.
				recordVideo(video);
				++report.added;
			}
		}
		for (auto it = videosAbsolutePathsFromDb.begin(); it != videosAbsolutePathsFromDb.end(); ++it) {
			if (!videosAbsolutePathsFromDisk.contains(it->first)) {
				std::cout << "we pass here" << std::endl;
				// We must remove this video from the database.
				db->run("DELETE FROM video WHERE video_id = ?", it->second);
				++report.removed;
			}
		}
	}
	Folder::Folder(Database& database, int64_t libraryId, int64_t id, const String& folder_path):
		db(database), library_id(libraryId), video_folder_id(id), absolute_path(folder_path), videos(), report() {
		setId(video_folder_id);
		setKey(absolute_path);
		init();
	}
	void Folder::refresh() {
		HashSet<String> relativePaths;
		report.total = report.added = report.removed = 0;
		cigmar::video::Video::collectPaths((const char*)absolute_path,
										   [&relativePaths, this](const String& path) {
											   relativePaths.add(sys::path::relative(absolute_path, path));
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
		Stack<const DbVideo*> toRemove;
		for (const DbVideo& video: videos) {
			if (!relativePaths.contains(video.key())) {
				toRemove << (&video);
			}
		}
		for (const DbVideo* video: toRemove) {
			db->run("DELETE FROM video WHERE video_id = ?", video->id());
			videos.remove(video->id());
			++report.removed;
		}
	};
}
