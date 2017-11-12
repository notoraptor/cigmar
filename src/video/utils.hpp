//
// Created by HPPC on 12/11/2017.
//

#ifndef SRC_CIGMAR_UTILS_HPP
#define SRC_CIGMAR_UTILS_HPP

#include <cigmar/classes/ArrayList.hpp>
#include <cigmar/classes/HashSet.hpp>
#include <cigmar/classes/String.hpp>
#include <cigmar/file/Lines.hpp>
#include <cigmar/filesystem.hpp>
#include <video/Video.hpp>

namespace cigmar::video {
	inline ArrayList<Video> loadVideosFromDirectory(const char* dirpath) {
		sys::Dir dir(dirpath);
		ArrayList<Video> videos;
		for (const char* pathname: dir) {
			String extension = sys::path::extension(pathname);
			if (extension && Video::extensionIsSupported(extension.lower())) {
				videos.add(Video(sys::path::join(dirpath, pathname)));
			}
		}
		return videos;
	}
	class TsvVideoRecorder: public Streamable {
	public:
		static const char* fields[];
		static const size_t fieldsCount;
		static void header(std::ostream& o);
		static String header() {
			std::ostringstream s;
			header(s);
			return s.str();
		}
	private:
		const Video& video;
	public:
		int notation;
	public:
		explicit TsvVideoRecorder(const Video& v): video(v), notation(0) {}
		void toStream(std::ostream& o) const override {
			String id = video.getId();
			String absolutePath = sys::path::absolute((const char*)video.getFilename());
			String thumbnailName = id;
			thumbnailName << '.' << Video::thumbnailExtension;
			o << id;
			o << '\t' << video.getFormat();
			o << '\t' << video.getFrameRate();
			o << '\t' << video.getSampleRate();
			o << '\t' << video.getAudioCodec();
			o << '\t' << video.getVideoCodec();
			o << '\t' << video.getSize();
			o << '\t' << video.getDuration();
			o << '\t' << video.getWidth();
			o << '\t' << video.getHeight();
			o << '\t' << notation;
			o << '\t' << thumbnailName;
			o << '\t' << video.getDateAddedMicroseconds();
			o << '\t' << absolutePath;
		}
	};
}

#endif //SRC_CIGMAR_UTILS_HPP
