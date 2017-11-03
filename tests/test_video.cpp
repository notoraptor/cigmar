#include <cigmar/unittests.hpp>
#include <cigmar/classes/String.hpp>
#include <video/Video.hpp>
#include <cigmar/print.hpp>
using namespace cigmar;

/*
Tested file: tests/Lion.ogv
 - 2017/11/03: https://commons.wikimedia.org/wiki/File:Lion.ogv
*/

utt_begin(TestVideo);

utt(test_video) {
	String folder = "tests";
	String filename = "Lion.ogv";
	video::Video v(sys::path::join(folder, filename));
	utt_assert(v.getFilename() == sys::path::join(folder, filename));
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
	utt_assert(video::ffmpeg::thumbnail(v, 1));
}

utt_end();
