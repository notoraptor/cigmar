#include <video/ffmpeg.hpp>
#include <video/Video.hpp>

namespace cigmar::video::ffmpeg {
	/** Call ``ffprobe`` to get video infos in JSON format. **/
	String infos(const String& filename) {
		String command = "ffprobe -v quiet -print_format json -show_error -show_format -show_streams ";
		command << '"' << filename << '"';
		return sys::run(command.cstring());
	}
	bool thumbnail(const Video& video, int number) {
		String pathname = video.getFilename();
		pathname << "." << number << ".jpg";
		String command;
		command << "ffmpeg -y -ss " << (int)(video.getDuration() / 2);
		command << " -i \"" << video.getFilename() << "\" -vframes 1 \"" << pathname << "\" 2>&1";
		sys::run(command.cstring());
		return sys::path::isFile(pathname.cstring());
	}
}
