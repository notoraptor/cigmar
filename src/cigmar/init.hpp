#ifndef CIGMAR_INIT
#define CIGMAR_INIT

#include <iostream>

namespace cigmar {

class CigmarInit {
public:
	CigmarInit() noexcept {
		std::set_terminate(CigmarInit::terminate);
	}
	static void terminate() {
		std::exception_ptr current_exception = std::current_exception();
		if (current_exception) {
			try {
				std::rethrow_exception(current_exception);
			} catch (std::exception& ex) {
				std::cerr << "Exception not caught: " << ex.what() << std::endl;
			}
		}
		std::cerr << "Unexpected program end." << std::endl;
		exit(EXIT_FAILURE);
	}
};

extern CigmarInit cigmar_init;
/**< Instantiation of library initialization class CigmarInit,
to automatically initialize library before call to main() function.
**/

}

#endif // CIGMAR_INIT
