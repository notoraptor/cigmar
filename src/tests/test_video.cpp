#include <cigmar/unittests.hpp>
#include <cigmar/classes/String.hpp>
#include <cigmar/filesystem.hpp>
#include <cigmar/print.hpp>
#include <video/Video.hpp>
using namespace cigmar;

/*
Tested file: tests/Lion.ogv
 - 2017/11/03: https://commons.wikimedia.org/wiki/File:Lion.ogv
*/

utt_begin(TestVideo);

	utt(test_video) {
		String folder = "res/video";
		String filename = "Lion.ogv";
		video::Video v(sys::path::join(folder, filename));
		utt_assert(v.getAbsolutePath() == sys::path::absolute((const char*)sys::path::join(folder, filename)));
		utt_assert(v.getAudioCodec() == "vorbis");
		utt_assert(v.getVideoCodec() == "theora");
		utt_assert(v.getFormat() == "Ogg");
		utt_assert(v.getFrameRate() == 25);
		utt_assert(v.getSampleRate() == 44100);
		utt_assert(v.getSize() == 6077595);
		utt_assert(v.getWidth() == 532);
		utt_assert(v.getHeight() == 300);
		utt_assert(v.getDuration() > 84);
		utt_assert(v.getDuration() < 85);
		// utt_assert(video::ffmpeg::thumbnail(v, 1));
	}

	utt(test_video_folder) {
		sys::Dir folder("res/video");
		for (String entry: folder) {
			if (entry.endsWith(".mp4")) {
				String videoPath = sys::path::join("res/video", entry);
				utt_assert(sys::path::isFile((const char*)videoPath));
			}
		}
	}

utt_end();
