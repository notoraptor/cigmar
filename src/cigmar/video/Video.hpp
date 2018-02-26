//
// Created by notoraptor on 01/11/2017.
//

#ifndef CIGMAR_VIDEO_HPP
#define CIGMAR_VIDEO_HPP

#include <cstdint>
#include <cmath>
#include <chrono>
#include <cigmar/classes/String.hpp>
#include <cigmar/classes/HashMap.hpp>
#include <cigmar/classes/exception/Exception.hpp>
#include <cigmar/cigmar.hpp>
#include <cigmar/video/ffmpeg.hpp>

// TODO: PNG: http://lodev.org/lodepng/
// TODO: https://en.wikipedia.org/wiki/Netpbm#PAM_graphics_format

namespace cigmar::video {
	class Video {
	private:
		String absolutePath;
		String format;
		String audioCodec;
		String videoCodec;
		int64_t dateAddedMicroseconds;
		size_t size;
		size_t width;
		size_t height;
		double duration;
		double frameRate;
		double sampleRate;
		String absolutePathHash;
	private:
		template<typename T> void extractValue(const String& in, T& out) {
			std::stringstream ss;
			ss << in;
			ss >> out;
		}
		void getInformations() {
			HashMap<String, String> audioStream, videoStream, formats;
			absolutePath = sys::path::absolute((const char*)absolutePath);
			String ffprobeInfos = ffmpeg::infos(absolutePath);
			for(String& line: ffprobeInfos.lines()) if (line) {
				auto pieces = line.splits(";", true);
				auto it = pieces.begin(), end = pieces.end();
				if (it == end)
					throw Exception("Unexpected iteration end.");
				if (!it->trim())
					throw Exception("Error while parsing section in ffprobe output.");
				String type = *it;
				HashMap<String, String> stream;
				++it;
				while (it != end) {
					if (it->trimLeft()) {
						pos_t pos = it->indexOf('=');
						if (!pos)
							throw Exception("Error while parsing key/value in ffprobe output");
						String key = (*it)(0, (size_t)pos);
						String value = (*it)((size_t)pos + 1, LAST);
						stream.put(key, value);
					}
					++it;
				}
				if (type == "stream") {
					String& codec_type = stream.get("codec_type");
					if (codec_type == "audio") {
						if (!audioStream)
							audioStream.swap(stream);
					} else if (codec_type == "video") {
						if (!videoStream)
							videoStream.swap(stream);
					};
				} else if (type == "format") {
					if (formats)
						throw Exception("Expected only 1 section 'format' in ffprobe output.");
					formats.swap(stream);
				} else
					throw Exception("Unexpected section (neither stream nor format) in ffprobe output.");
			}
			if (!videoStream)
				throw Exception("Unable to detect a video stream in ffprobe output.");
			if (!formats)
				throw Exception("Unable to detect format infos in ffprobe output.");
			format = formats.get("format_long_name");
			extractValue(formats.get("size"), size);
			extractValue(formats.get("duration"), duration);
			extractValue(videoStream.get("width"), width);
			extractValue(videoStream.get("height"), height);
			videoCodec = videoStream.get("codec_name");
			//
			String f = videoStream.get("avg_frame_rate");
			if (f == "0" || f == "0/0")
				f = videoStream.get("r_frame_rate");
			pos_t pos_slash = f.indexOf('/');
			if (pos_slash) {
				double denominator;
				extractValue(f(0, (size_t)pos_slash), frameRate);
				extractValue(f((size_t)pos_slash + 1, LAST), denominator);
				frameRate /= denominator;
			} else {
				extractValue(f, frameRate);
			}
			if (std::isnan(frameRate))
				frameRate = 24; // Let's say 24 frames per second.
			// Audio codec and sample rate.
			if (audioStream) {
				audioCodec = audioStream.get("codec_name");
				f = audioStream.get("sample_rate");
				pos_slash = f.indexOf('/');
				if (pos_slash) {
					double denominator;
					extractValue(f(0, (size_t)pos_slash), sampleRate);
					extractValue(f((size_t)pos_slash + 1, LAST), denominator);
					sampleRate /= denominator;
				} else {
					extractValue(f, sampleRate);
				}
				if (std::isnan(sampleRate))
					sampleRate = 44100; // Let's say 44100 Hz.
			}
			dateAddedMicroseconds =
					std::chrono::duration_cast<std::chrono::microseconds>(
							std::chrono::high_resolution_clock::now().time_since_epoch()).count();
		}
	public:
		static const char* const thumbnailExtension;
		static bool extensionIsSupported(const String& extension);
		static void collect(const char *dirpath, const std::function<void(Video&&)>& collector);
		static void collectPaths(const char *dirpath, const std::function<void(const String&)>& collector);
	public:
		explicit Video(const String& filepath):
				absolutePath(filepath), format(), audioCodec(), videoCodec(), absolutePathHash(), dateAddedMicroseconds(0),
				size(0), width(0), height(0), duration(0), frameRate(0), sampleRate(0) {
			getInformations();
		};
		explicit Video(String&& filepath):
				absolutePath(std::move(filepath)), format(), audioCodec(), videoCodec(), absolutePathHash(), dateAddedMicroseconds(0),
				size(0), width(0), height(0), duration(0), frameRate(0), sampleRate(0) {
			getInformations();
		};
		Video(const Video&) = default;
		Video(Video&&) noexcept = default;

		const String& getAbsolutePath() const { return absolutePath; }
		const String& getFormat() const { return format; }
		const String& getAudioCodec() const { return audioCodec; }
		const String& getVideoCodec() const { return videoCodec; }
		size_t getSize() const { return size; }
		size_t getWidth() const { return width; }
		size_t getHeight() const { return height; }
		double getDuration() const { return duration; }
		double getFrameRate() const { return frameRate; }
		double getSampleRate() const { return sampleRate; }
		int64_t getDateAddedMicroseconds() const {
			return dateAddedMicroseconds;
		}

		const String& getAbsolutePathHash() {
			if (!absolutePathHash) absolutePathHash = crypto::hash::whirlpool(absolutePath);
			return absolutePathHash;
		}
		String getAbsolutePathHash() const {
			return crypto::hash::whirlpool(absolutePath);
		}

		// For tree sets.
		bool operator<(const Video& other) const {
			return absolutePath < other.absolutePath;
		}
	};

	template <typename C>
	std::basic_ostream<C>& operator<<(std::basic_ostream<C>& o, const Video& v) {
		o << "Video(" << v.getAbsolutePath() << ")";
		return o;
	}
}

#endif //CIGMAR_VIDEO_HPP
