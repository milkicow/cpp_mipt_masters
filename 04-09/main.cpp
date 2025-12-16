#include <type_traits>

namespace my {

template <typename T>
std::true_type test_is_class(int T::*);

template <typename T>
std::false_type test_is_class(...);

template <typename T>
struct is_class : public decltype(test_is_class<T>(nullptr)) {};

template <typename T>
inline constexpr bool is_class_v = is_class<T>::value;

template <typename T>
struct add_const {
    using type = const T;
};

template <typename T>
using add_const_t = typename add_const<T>::type;

template <typename T>
struct remove_const {
    using type = T;
};

template <typename T>
struct remove_const<const T> {
    using type = T;
};

template <typename T>
using remove_const_t = typename remove_const<T>::type;

template <typename T>
struct remove_reference {
    using type = T;
};

template <typename T>
struct remove_reference<T &> {
    using type = T;
};

template <typename T>
struct remove_reference<T &&> {
    using type = T;
};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

template <typename T>
struct decay_selector
    : public std::conditional_t<std::is_function_v<T>, std::add_pointer<T>, std::remove_cv<T>> {};

template <typename T, std::size_t N>
struct decay_selector<T[N]> {
    using type = T *;
};

template <typename T>
struct decay_selector<T[]> {
    using type = T *;
};

template <typename T>
using decay_selector_t = typename decay_selector<T>::type;

template <typename T>
struct decay {
    using type = decay_selector_t<T>;
};

template <typename T>
struct decay<T &> {
    using type = decay_selector_t<T>;
};

template <typename T>
struct decay<T &&> {
    using type = decay_selector_t<T>;
};

template <typename T>
using decay_t = typename decay<T>::type;

template <bool B, typename T, typename F>
struct conditional {
    using type = T;
};

template <typename T, typename F>
struct conditional<false, T, F> {
    using type = F;
};

template <bool B, typename T, typename F>
using conditional_t = typename conditional<B, T, F>::type;

}  // namespace my

int main() {
    struct S {};
    class C {};
    enum E { e1 };
    union U {};

    static_assert(my::is_class_v<int> == false, "int is not a class");
    static_assert(my::is_class_v<S> == true, "struct S is a class");
    static_assert(my::is_class_v<C> == true, "class C is a class");
    static_assert(my::is_class_v<U> == true, "union U is not a class");
    static_assert(my::is_class_v<E> == false, "enum E is not a class");
    static_assert(my::is_class_v<void> == false, "void is not a class");

    static_assert(my::is_class<S>::value == true, "Test 1 failed");
    static_assert(my::is_class<C>::value == true, "Test 2 failed");
    static_assert(my::is_class<int>::value == false, "Test 3 failed");
    static_assert(my::is_class<E>::value == false, "Test 4 failed");

    static_assert(std::is_same_v<my::add_const_t<int>, const int>, "add_const test 1 failed");
    static_assert(std::is_same_v<my::add_const_t<const int>, const int>, "add_const test 2 failed");
    static_assert(std::is_same_v<my::add_const_t<int *>, int *const>, "add_const test 3 failed");
    static_assert(std::is_same_v<my::add_const_t<const int *>, const int *const>,
                  "add_const test 4 failed");

    static_assert(std::is_same_v<my::remove_const_t<const int>, int>, "remove_const test 1 failed");
    static_assert(std::is_same_v<my::remove_const_t<int>, int>, "remove_const test 2 failed");
    static_assert(std::is_same_v<my::remove_const_t<const int *>, const int *>,
                  "remove_const test 3 failed");
    static_assert(std::is_same_v<my::remove_const_t<int *const>, int *>,
                  "remove_const test 4 failed");
    static_assert(std::is_same_v<my::remove_const_t<const int[]>, int[]>,
                  "remove_const test 5 failed");
    static_assert(std::is_same_v<my::remove_const_t<const int[5]>, int[5]>,
                  "remove_const test 6 failed");

    static_assert(std::is_same_v<my::decay_t<int>, int>, "decay test 1 failed");
    static_assert(std::is_same_v<my::decay_t<int &>, int>, "decay test 2 failed");
    static_assert(std::is_same_v<my::decay_t<int &&>, int>, "decay test 3 failed");
    static_assert(std::is_same_v<my::decay_t<const int &>, int>, "decay test 4 failed");
    static_assert(std::is_same_v<my::decay_t<int[3]>, int *>, "decay test 5 failed");
    static_assert(std::is_same_v<my::decay_t<int[]>, int *>, "decay test 6 failed");
    static_assert(std::is_same_v<my::decay_t<int(int)>, int (*)(int)>, "decay test 7 failed");

    static_assert(std::is_same_v<my::conditional_t<true, int, double>, int>,
                  "conditional test 1 failed");
    static_assert(std::is_same_v<my::conditional_t<false, int, double>, double>,
                  "conditional test 2 failed");
    static_assert(std::is_same_v<my::conditional_t<(sizeof(int) > 2), int, short>, int>,
                  "conditional test 3 failed");

    return 0;
}
