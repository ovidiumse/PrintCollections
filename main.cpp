// Type your code here, or load an example.

#include <set>
#include <iostream>
#include <type_traits>

namespace tag
{
    struct Container
    {
    };

    struct Other
    {
    };
}

template <typename T, typename Properties = void>
struct ContainerTraits : std::false_type
{
};

template <typename T, typename Properties = void>
struct IsString : std::false_type
{
};

template <typename T>
struct IsString<T, std::void_t<
                               std::enable_if_t<
                                                std::is_pointer<T>::value 
                                                && std::is_same<decltype(*std::declval<T>()), char>::value>>> : std::true_type
{
};

template <typename T>
struct IsString<T, std::void_t<std::enable_if_t<std::is_same<char, typename T::value_type>::value>, decltype(std::declval<T>().data()), decltype(std::declval<T>().c_str())>> : std::true_type
{
};

template <typename T>
struct ContainerTraits<T, 
                       std::void_t<std::enable_if_t<!IsString<T>::value>, 
                                   decltype(std::begin(std::declval<T>()))>> : std::true_type
{
    using kind = tag::Container;
};

template <typename T>
void print(std::ostream& out, const T& container)
{
    bool needsComma = false;
    for (const auto& val : container)
    {
        if (needsComma)
        {
            out << ", ";
        }
        else
        {
            needsComma = true;
        }

        out << val;
    }
}

namespace std
{
template <typename T, typename Properties = std::enable_if_t<ContainerTraits<T>::value>>
std::ostream& operator<<(std::ostream& out, const T& container)
{
    print(out, container);
    return out;
}
}

int main()
{
    std::set<int> s = {1,3,2};

    char message[] = "Hello!";
    std::cout << "s: " << s << '\n';
    std::cout << "Message: " << message << '\n';
    std::cout << std::string("This is cool!") << '\n';

    return 0;
}
