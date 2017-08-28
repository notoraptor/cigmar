#ifndef CIGMAR_INIT
#define CIGMAR_INIT

class CigmarInit {
public:
	CigmarInit() {
		std::cout << std::boolalpha;
		std::cerr << std::boolalpha;
	}
};

extern CigmarInit cigmar_init;

#endif // CIGMAR_INIT
