
#include "vector.h"
#include <iostream>


template<typename T>
class MyIter
{
    public:
        using value_type=T;
};

template<typename T>
typename T::value_type find(T iter)
{
    return iter;
}

int main()
{
    vector<std::string> v;
    auto p=v.begin();
}