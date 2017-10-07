#ifndef CIGMAR_INIT
#define CIGMAR_INIT

#include <iostream>

namespace cigmar {

class CigmarInit {
public:
	CigmarInit() {
		std::cout << std::boolalpha;
		std::cerr << std::boolalpha;
	}
};

extern CigmarInit cigmar_init;
/**< Instantiation of library initialization class CigmarInit,
to automatically initialize library before call to main() function.
**/

}

#endif // CIGMAR_INIT
