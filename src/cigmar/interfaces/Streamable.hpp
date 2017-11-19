#ifndef CIGMAR_STREAMABLE
#define CIGMAR_STREAMABLE

#include <ostream>
#include <type_traits>
#include <cigmar/symbols.hpp>

namespace cigmar {

	class Streamable {
	public:
		virtual void toStream(std::ostream& o) const = 0;
		virtual ~Streamable() = default;
		struct Streamer {
			template<typename T>
			void operator()(std::ostream& o, const T& v) const {
				o << v;
			}
		};
		struct DefaultStreamer {
			template<typename T>
			void operator()(std::ostream& o, const T& v) const {
				o << "{object&" << (void*)(&v) << '}';
			}
		};
		template<typename T>
		static void stream(std::ostream& o, const T& v) {
			typedef typename std::conditional<is_streamable<T>::value, Streamer, DefaultStreamer>::type streamer_t;
			streamer_t()(o, v);
		}
	};

	std::ostream& operator<<(std::ostream& o, const Streamable& s);

}

#endif // CIGMAR_STREAMABLE
