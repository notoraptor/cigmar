#include <iostream>
#include <sstream>
#include <string>
#include "utils.hpp"
using std::cout;
using std::endl;

template<typename T>
class return_t {
private:
	T* pointer;
	bool allocated;
	bool isarray;
public:
	return_t(T* ptr = nullptr, bool todelete = false, bool isarr = false): pointer(ptr), allocated(todelete), isarray(isarr) {}
	return_t(return_t& other): pointer(other.pointer), allocated(other.allocated), isarray(other.isarray) {
		other.pointer = nullptr;
		other.allocated = other.isarray = false;
	}
	~return_t() {
		if (allocated) {
			if (isarray)
				delete[] pointer;
			else
				delete pointer;
		}
	}
	return_t(return_t&& o): return_t((return_t&)o) {};
	return_t& operator=(return_t& other) {
		pointer = other.pointer;
		allocated = other.allocated;
		isarray = other.isarray;
		other.pointer = nullptr;
		other.allocated = other.isarray = false;
		return *this;
	}
	return_t& operator=(return_t&& o) {
		return (*this = (return_t&)o);
	};
	operator T*() const {return pointer;}
	T& operator*() {return *pointer;}
	T* operator->() {return pointer;}
	T& operator[](size_t pos) {return pointer[pos];}
public:
};

template<typename E> return_t<E> transfer(E* o) {return return_t<E>(o, true, false);}
template<typename E> return_t<E> return_stack(E* o) {return return_t<E>(o, false, false);}
template<typename E> return_t<E> return_stack(E& o) {return return_t<E>(&o, false, false);}
template<typename E> return_t<E> return_array(E* arr) {return return_t<E>(arr, false, true);}
template<typename E> return_t<E> return_newarray(E* arr) {return return_t<E>(arr, true, true);}

template<typename T>
std::ostream& operator<<(std::ostream& o, const return_t<T>& ret) {
	o << (T*)ret;
	return o;
}

struct Point {
public:
	int x;
	int y;
	Point(int myx = 0, int myy = 0): x(myx), y(myy) {}
	~Point() {cout << "deleted Point(" << x << ", " << y << ") at " << this << endl;}
};

int main() {
	return_t<Point> p = transfer(new Point(12, 17));
	return_t<Point> q = p;
	return_t<Point> r;
	r = q;
	p = transfer(new Point(-1, 4));
	cout << p << endl;
	cout << q << endl;
	cout << r << endl;
	return 0;
}
