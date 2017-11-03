//
// Created by notoraptor on 01/11/2017.
//

#ifndef CIGMAR_VIDEO_HPP
#define CIGMAR_VIDEO_HPP

#include <cmath>
#include <cigmar/filesystem.hpp>
#include <cigmar/classes/String.hpp>
#include <libraries/json/json.hpp>
#include <cigmar/classes/Exception.hpp>

using Json = nlohmann::json;

// TODO: PNG: http://lodev.org/lodepng/
// TODO: https://en.wikipedia.org/wiki/Netpbm#PAM_graphics_format

namespace cigmar::video {
	class Video;

	namespace ffmpeg {
		/** Call ``ffprobe`` to get video infos in JSON format. **/
		String infos(const String& filename);
		bool thumbnail(const Video& video, int number = 0);
	}

	class Video {
	private:
		String filename;
		String format;
		String audioCodec;
		String videoCodec;
		size_t size;
		size_t width;
		size_t height;
		double duration;
		double frameRate;
		double sampleRate;
	private:
		template<typename T> void extractValue(const std::string& in, T& out) {
			std::stringstream ss;
			ss << in;
			ss >> out;
		}
		template<typename T> T extractValue(const std::string& in) {
			T out;
			extractValue(in, out);
			return out;
		}
		void getRateSplitPosition(const std::string& parsed, size_t& out) {
			out = std::string::npos;
			for (size_t i = 0; i < parsed.length(); ++i)
				if (parsed[i] == '/') {
					if (out == std::string::npos)
						out = i;
					else
						throw Exception("Unable to parse rate.");
				}
		}
		void getInfos() {
			Json jsonInfos = Json::parse(ffmpeg::infos(filename));
			Json::value_type *firstAudioStream = nullptr;
			Json::value_type *firstVideoStream = nullptr;
			Json::reference streams = jsonInfos.at("streams");
			if (!streams.is_array())
				throw Exception("No streams array in ffmpeg output for filename ", filename);
			for (size_t i = 0; i < streams.size(); ++i) {
				Json::reference stream = streams[i];
				String codecType = stream.at("codec_type").get<std::string>();
				if (codecType == "audio") {
					if (!firstAudioStream)
						firstAudioStream = &stream;
				} else if (codecType == "video") {
					if (!firstVideoStream)
						firstVideoStream = &stream;
				}
			}
			if (!firstVideoStream)
				throw Exception("No video stream in file ", filename);
			Json::reference infosFormat = jsonInfos.at("format");
			format = infosFormat.at("format_long_name").get<std::string>();
			extractValue(infosFormat["size"].get<std::string>(), size);
			extractValue(infosFormat.at("duration").get<std::string>(), duration);
			width = firstVideoStream->at("width").get<size_t>();
			height = firstVideoStream->at("height").get<size_t>();
			// Video codec and frame rate.
			videoCodec = firstVideoStream->at("codec_name").get<std::string>();
			size_t pos_slash;
			std::string f = firstVideoStream->at("avg_frame_rate").get<std::string>();
			if (f == "0" || f == "0/0")
				f = firstVideoStream->at("r_frame_rate");
			getRateSplitPosition(f, pos_slash);
			extractValue(f.substr(0, pos_slash), frameRate);
			if (pos_slash != std::string::npos)
				frameRate /= extractValue<decltype(frameRate)>(f.substr(pos_slash + 1));
			if (std::isnan(frameRate))
				frameRate = 24; // Let's say 24 frames per second.
			// Audio codec and sample rate.
			if (firstAudioStream) {
				audioCodec = firstAudioStream->at("codec_name").get<std::string>();
				f = firstAudioStream->at("sample_rate").get<std::string>();
				getRateSplitPosition(f, pos_slash);
				extractValue(f.substr(0, pos_slash), sampleRate);
				if (pos_slash != std::string::npos)
					sampleRate /= extractValue<decltype(sampleRate)>(f.substr(pos_slash + 1));
				if (std::isnan(sampleRate))
					sampleRate = 44100; // Let's say 44100 Hz.
			}
		}
	public:
		explicit Video(const String& filepath):
				filename(filepath), format(), audioCodec(), videoCodec(),
				size(0), width(0), height(0), duration(0), frameRate(0), sampleRate(0) {
			getInfos();
		};
		explicit Video(String&& filepath):
				filename(std::move(filepath)), format(), audioCodec(), videoCodec(),
				size(0), width(0), height(0), duration(0), frameRate(0), sampleRate(0) {
			getInfos();
		};

		const String& getFilename() const { return filename; }
		const String& getFormat() const { return format; }
		const String& getAudioCodec() const { return audioCodec; }
		const String& getVideoCodec() const { return videoCodec; }
		size_t getSize() const { return size; }
		size_t getWidth() const { return width; }
		size_t getHeight() const { return height; }
		double getDuration() const { return duration; }
		double getFrameRate() const { return frameRate; }
		double getSampleRate() const { return sampleRate; }
	};

}

#endif //CIGMAR_VIDEO_HPP
