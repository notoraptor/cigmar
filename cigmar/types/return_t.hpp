#ifndef CIGMAR_RETURN_T
#define CIGMAR_RETURN_T

#include <iostream>

namespace cigmar {

template<typename T>
class return_t {
private:
	T* p;
	bool todelete;
	bool isarray;
	void unsafe_reset() {
		p = nullptr; todelete = isarray = false;
	}
	return_t(T* pointer, bool toDelete, bool isArray):
		p(pointer), todelete(toDelete), isarray(isArray) {}
public:
	return_t(return_t& o): p(o.p), todelete(o.todelete), isarray(o.isarray) {
		o.unsafe_reset();
	}
	return_t(return_t&& m): return_t((return_t&)m) {}
	~return_t() {
		if (todelete) {
			if (isarray)
				delete[] p;
			else
				delete p;
			p = nullptr;
		}
	}
	return_t& operator=(return_t& o) {
		p = o.p; todelete = o.todelete; isarray = o.isarray;
		o.unsafe_reset();
		return *this;
	}
	return_t& operator=(return_t&& m) {
		return (*this = (return_t&)m);
	}
	// operator T*() {return p;}
	// operator T&() {return *p;}
	operator T&&() {return std::move(*p);}
	template<typename E> friend return_t<E> from_stack(E& o);
	template<typename E> friend return_t<E> array_from_stack(E* o);
	template<typename E> friend return_t<E> transfer(E* o);
	template<typename E> friend return_t<E> transfer_array(E* o);
	template<typename E> friend bool check(return_t<E>& r);
};

template<typename E> return_t<E> from_stack(E& o) {return return_t<E>(&o, false, false);};
template<typename E> return_t<E> array_from_stack(E* o) {return return_t<E>(o, false, true);};
template<typename E> return_t<E> transfer(E* o) {return return_t<E>(o, true, false);};
template<typename E> return_t<E> transfer_array(E* o) {return return_t<E>(o, true, true);};

template<typename E> bool check(return_t<E>& r) {return r.p != nullptr;}

}

#endif // CIGMAR_RETURN_T
