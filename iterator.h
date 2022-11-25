#ifndef ITERATOR_H
#define ITERATOR_H

template <typename Iterator>
struct iterator_traits
{
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::difference_type difference_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
};

template<typename T>
struct iterator_traits<T*>
{
    using value_type=T;
};

template<typename T>
struct iterator_traits<const T*>
{
    using value_type=T;
};




#endif