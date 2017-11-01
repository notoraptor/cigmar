#include <iostream>
#include <cigmar/std.hpp>
#include <cigmar/print.hpp>
#include <cigmar/filesystem.hpp>
#include <cigmar/unittests.hpp>
#include <libraries/json/json.hpp>
#include <cigmar/whirlpool.hpp>
#include <video/Video.hpp>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using namespace cigmar;
using json = nlohmann::json;
int main() {
	const String folder = "C:\\Users\\notoraptor\\Downloads\\videos";
	String filename = "Lion.ogv";
	video::Video v(sys::path::join(folder, filename));
	sys::println(v.getFilename());
	sys::println(v.getAudioCodec());
	sys::println(v.getVideoCodec());
	sys::println(v.getFormat());
	sys::println(v.getFrameRate());
	sys::println(v.getSampleRate());
	sys::println(v.getSize());
	sys::println(v.getWidth());
	sys::println(v.getHeight());
	sys::println(v.getDuration());
	sys::println(video::ffmpeg::thumbnail(v, 1));
	unsigned char x = 254;
	sys::println((int)x);
	sys::println((int)((int)x >> 4));
	sys::println((int)((int)x ^ ((x >> 4) << 4)));
	sys::println(crypto::hash::whirlpool("test"));
	const char* jstring = R"(
{
"a": true,
"b": 1,
"salue": -2.9,
"hello": "merci"
}
)";
	json j = json::parse(jstring);
	sys::println(j);
	sys::println(j.size());
	sys::println(j["a"].get<bool>());
	j["ccc"] = -27.56;
	sys::println(j.type() == json::value_t::object);
	sys::println(j.dump(4));
	tests::run();
	return 0;
}
