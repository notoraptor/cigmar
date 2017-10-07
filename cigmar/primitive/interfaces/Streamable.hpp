#ifndef CIGMAR_STREAMABLE
#define CIGMAR_STREAMABLE

#include <ostream>

namespace cigmar {

class Streamable {
public:
	virtual void toStream(std::ostream& o) const = 0;
	virtual ~Streamable() {}
};

std::ostream& operator<<(std::ostream& o, const Streamable& s);

}

#endif // CIGMAR_STREAMABLE
