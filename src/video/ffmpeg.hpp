#ifndef CIGMAR_VIDEO_FFMPEG
#define CIGMAR_VIDEO_FFMPEG
#include <cigmar/classes/String.hpp>

namespace cigmar::video {
	class Video;
}

namespace cigmar::video {
	/** Call ``ffprobe`` to get video infos in JSON format. **/
	String infos(const String& filename);
	bool thumbnail(const Video& video, int number = 0);
}

#endif // CIGMAR_VIDEO_FFMPEG
