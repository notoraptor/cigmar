#ifndef CIGMAR_PRIMITIVE_UTILS
#define CIGMAR_PRIMITIVE_UTILS

#include <type_traits>

namespace cigmar {

/* Definition of static trait `is_iterable<type>::value`
https://stackoverflow.com/a/29634934
https://ideone.com/ExTsEO
(2017/09/24)
*/

namespace traits {
    // To allow ADL with custom begin/end
    using std::begin;
    using std::end;

    template <typename T>
    auto is_iterable_impl(int)
    -> decltype (
        begin(std::declval<T&>()) != end(std::declval<T&>()), // begin/end and operator !=
        ++std::declval<decltype(begin(std::declval<T&>()))&>(), // operator ++
        *begin(std::declval<T&>()), // operator*
        std::true_type{});

    template <typename T>
    std::false_type is_iterable_impl(...);

}

template <typename T>
using is_iterable = decltype(traits::is_iterable_impl<T>(0));
/**<
~~~~
cigmar::is_iterable<MyType>::value
~~~~

Type trait to statically check if a symbol T
is iterable according to C++ standard library
iteration model (ie. with begin() and end()
methods/functions.

Reference (2017/09/24):
- https://stackoverflow.com/a/29634934
- https://ideone.com/ExTsEO
**/

}

#endif // CIGMAR_PRIMITIVE_UTILS
