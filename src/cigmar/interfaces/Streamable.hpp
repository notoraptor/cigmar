#ifndef CIGMAR_STREAMABLE
#define CIGMAR_STREAMABLE

#include <ostream>
#include <type_traits>
#include <cigmar/symbols.hpp>
#include <cigmar/unicode.hpp>

namespace cigmar {

	struct Streamable {
		virtual void toStream(std::ostream& o) const = 0;
		virtual ~Streamable() = default;
	};

	template<typename T>
	struct Streamer {
		const T& value;
		explicit Streamer(const T& v): value(v) {};
	};

	template <typename T>
	inline std::ostream& operator<<(std::ostream& o, const Streamer<T>& streamer) {
		typedef typename std::conditional<is_streamable<T>{}, AutoStreamer, DefaultStreamer>::type streamer_t;
		streamer_t::print(o, streamer.value);
		return o;
	}

	std::ostream& operator<<(std::ostream& o, const Streamable& s);

}

#endif // CIGMAR_STREAMABLE
