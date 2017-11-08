#include <cigmar/unittests.hpp>
#include <cigmar/classes/Path.hpp>
#include <cigmar/print.hpp>

using namespace cigmar;

utt_begin(TestPath);
	utt(test_current_path) {
		sys::Path path(".");
		utt_assert(path.exists());
		utt_assert(path.isDirectory());
		utt_assert(!path.isFile());
		utt_smart_assert(path.basename() != ".", path.basename());
		utt_assert(path.filename() == path.basename());
		utt_assert(!path.hasExtension());
		utt_assert(path.extension() == "");
		sys::Path parent = path.parent();
		utt_assert(sys::Path(parent, path.filename()) == path);
	}
utt_end();
