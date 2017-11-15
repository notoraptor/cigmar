//
// Created by HPPC on 12/11/2017.
//

#ifndef SRC_CIGMAR_DATABASE_HPP
#define SRC_CIGMAR_DATABASE_HPP

#include <cigmar/format/tsv.hpp>
#include <video/Video.hpp>

namespace cigmar::video::database {
	namespace getters {
		String id(const Video& video) {return video.getId();}
		String format(const Video& video) {return video.getFormat();}
		String frameRate(const Video& video) {return String::print(video.getFrameRate());}
		String sampleRate(const Video& video) {return String::print(video.getSampleRate());}
		String audioCodec(const Video& video) {return video.getAudioCodec();}
		String videoCodec(const Video& video) {return video.getVideoCodec();}
		String size(const Video& video) {return String::print(video.getSize());}
		String duration(const Video& video) {return String::print(video.getDuration());}
		String width(const Video& video) {return String::print(video.getWidth());}
		String height(const Video& video) {return String::print(video.getHeight());}
		struct notation {
			String operator()(const Video& video) {return String::write(0);}
		};
		String thumbnailName(const Video& video) {return String::write(video.getId(), '.', Video::thumbnailExtension);}
		String dateAddedMicroseconds(const Video& video) {return String::write(video.getDateAddedMicroseconds());}
		String absolutePath(const Video& video) {return sys::path::absolute((const char*)video.getFilename());}
	}

	format::Tsv<Video> tsv() {
		return format::Tsv<Video>({{"id", getters::id},
								   {"format", getters::format},
								   {"frameRate", getters::frameRate},
								   {"sampleRate", getters::sampleRate},
								   {"audioCodec", getters::audioCodec},
								   {"videoCodec", getters::videoCodec},
								   {"size", getters::size},
								   {"duration", getters::duration},
								   {"width", getters::width},
								   {"height", getters::height},
								   {"notation", getters::notation()},
								   {"thumbnailName", getters::thumbnailName},
								   {"dateAddedMicroseconds", getters::dateAddedMicroseconds},
								   {"absolutePath", getters::absolutePath}});
	}

	ArrayList<Video> loadFromDirectory(const char* dirpath) {
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

	class Database {
	private:
	public:

	};
}

#endif //SRC_CIGMAR_DATABASE_HPP
