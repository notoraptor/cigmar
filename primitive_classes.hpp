#ifndef CIGMAR_PRIMITIVE_CLASSES
#define CIGMAR_PRIMITIVE_CLASSES

#include <array>

class AbstractMap;
class AbstractSet;

template<typename T, size_t N>
class array_t {
private:
    T mem[N];
public:
    const size_t length;
public:
    array_t(): length(N) {}
    array_t(const array_t&) = delete;

    T& operator[](size_t pos) {return mem[pos];}
    const T& operator[](size_t pos) const {return mem[pos];}

    template<typename F>
    array_t& each(F func) {
        for (size_t i = 0; i < length; ++i) func(mem[i]);
        return *this;
    }
    template<typename F>
    const array_t& each(F func) const {
        for (size_t i = 0; i < length; ++i) func(mem[i]);
        return *this;
    }
    array_t& memset(byte v) {
        memset(mem, v, length);
    }
};

class Array; // -> vector
class LinkedList; // Double linked list.
class Stack;
class Queue;
class HashMap;
class TreeMap;
class HashSet;
class TreeSet;

class Tensor; // Sophisticated N-D array.

#endif // CIGMAR_PRIMITIVE_CLASSES
