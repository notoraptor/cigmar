//
// Created by HPPC on 02/12/2017.
//

#include <cigmar/classes/String.hpp>
#include <cigmar/classes/HashSet.hpp>
#include <cigmar/video/Video.hpp>

namespace cigmar::video {
	static const char* const supportedExtensions[] = {
			"3g2", "3gp", "asf", "avi", "drc", "f4a", "f4b", "f4p", "f4v", "flv", "gifv", "m2v", "m4p", "m4v", "mkv",
			"mng", "mov", "mp2", "mp4", "mpe", "mpeg", "mpg", "mpv", "mxf", "nsv", "ogg", "ogv", "qt", "rm", "rmvb",
			"roq", "svi", "vob", "webm", "wmv", "yuv",
			nullptr // !nullptr at the end of this array is mandatory!
	};
	const char* const Video::thumbnailExtension = "jpg";

	bool Video::extensionIsSupported(const String &extension) {
		static HashSet<String> supportedExtensionsSet;
		if (!supportedExtensionsSet) {
			for (size_t i = 0; supportedExtensions[i]; ++i) {
				supportedExtensionsSet << supportedExtensions[i];
			}
		}
		return supportedExtensionsSet.contains(extension);
	}

	void Video::collect(const char *dirpath, const std::function<void(Video &&)> &collector) {
		sys::Dir dir(dirpath);
		for (const String pathname: dir) {
			if (pathname != "." && pathname != "..") {
				String path = sys::path::join(dirpath, pathname);
				if (sys::path::isDirectory((const char *) path)) {
					collect((const char *) path, collector);
				} else {
					String extension = sys::path::extension((const char *) pathname);
					if (extensionIsSupported(extension.lower()))
						collector(Video(path));
				}
			}
		}
	}

	void Video::collectPaths(const char *dirpath, const std::function<void(const String &)> &collector) {
		sys::Dir dir(dirpath);
		for (const String pathname: dir) {
			if (pathname != "." && pathname != "..") {
				const String path = sys::path::join(dirpath, pathname);
				if (sys::path::isDirectory((const char *) path)) {
					collectPaths((const char *) path, collector);
				} else {
					String extension = sys::path::extension((const char *) pathname);
					if (cigmar::video::Video::extensionIsSupported(extension.lower()))
						collector(path);
				}
			}
		}
	}
}
