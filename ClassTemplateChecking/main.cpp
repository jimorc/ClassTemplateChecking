#include <type_traits>
#include <iostream>

namespace test1 {
    // From http://coliru.stacked-crooked.com/a/eaeac2b9008a97d9 in answer to 'is_base_of of generic type'
    // at http://stackoverflow.com/questions/32477691/is-base-of-of-generic-type/32489730#32489730
    template <template <typename...> class Base, typename Derived>
    struct is_base_of_template
    {
        using U = typename std::remove_cv<Derived>::type;

        template <typename... Args>
        static std::true_type test(Base<Args...>*);

        static std::false_type test(void*);

        using type = decltype(test(std::declval<U*>()));
    };

    template <template <typename...> class Base, typename Derived>
    using is_base_of_template_t = typename is_base_of_template<Base, Derived>::type;
}

namespace test2 {
    // Based on Arunmu's answer to a somewhat related question that I asked on Stack Overflow:
    // http://stackoverflow.com/questions/37509611/templating-classes-and-aliases-causes-compile-errors/37510350#37510350
    template <typename T>
    constexpr auto has_func(const T& obj) -> decltype(std::declval<T&>().func(), bool())
    {
        return true;
    }

    constexpr auto has_func(...)
    {
        return false;
    }
}

template<typename T>
class A
{
public:
    // note that func only needs to be declared and not defined for test2 and test3 static_asserts to work
    void func();
};

class B : public A<int>
{};

class D {};

template<typename T>
class C
{
public:
    C() 
    {
        static_assert(test1::is_base_of_template_t<A, T>::value, "No!");
        static_assert(test2::has_func(T()), "Another No!");
    }
};

int main(int argsCount, char** args)
{
    C<B> c;
    C<D> d;
}