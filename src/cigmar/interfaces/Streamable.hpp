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
	};

	namespace {
		struct AutoStreamer {
			template<typename E>
			void operator()(std::ostream& o, const E& value) const {
				o << value;
			}
		};
		struct DefaultStreamer {
			template<typename E>
			void operator()(std::ostream& o, const E& value) const {
				o << "{object&" << (void*)(&value) << '}';
			}
		};
	}

	template<typename T>
	class Streamer: public Streamable {
	private:
		const T& v;
	public:
		explicit Streamer(const T& value): v(value) {};
		void toStream(std::ostream& o) const override {
			typedef typename std::conditional<is_streamable<T>::value, AutoStreamer, DefaultStreamer>::type streamer_t;
			streamer_t()(o, v);
		}
	};

	std::ostream& operator<<(std::ostream& o, const Streamable& s);

}

#endif // CIGMAR_STREAMABLE
