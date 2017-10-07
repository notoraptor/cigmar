#ifndef CIGMAR_PRIMITIVE_CLASSES
#define CIGMAR_PRIMITIVE_CLASSES

#include <cigmar/primitive/classes/array_t.hpp>
#include <cigmar/primitive/classes/ArrayList.hpp>
#include <cigmar/primitive/classes/TreeSet.hpp>
#include <cigmar/primitive/classes/HashSet.hpp>
#include <cigmar/primitive/classes/TreeMap.hpp>
#include <cigmar/primitive/classes/HashMap.hpp>
#include <cigmar/primitive/classes/Stack.hpp>
#include <cigmar/primitive/classes/Queue.hpp>

namespace cigmar {
// class AbstractList;
// class AbstractMap;
// class AbstractSet;
class LinkedList; // Double linked list.
class Tensor; // Sophisticated N-D array.

#define dtypeof(variable) decltype(variable)::dtype

}

#endif // CIGMAR_PRIMITIVE_CLASSES
