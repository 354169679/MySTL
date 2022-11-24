#ifndef UNINITIALIZED_H
#define UNINITIALIZED_H


#include "construct.h"

template<typename ForwardIterator,typename T_val>
void uninitialized_fill(ForwardIterator first,ForwardIterator last,const T_val& val)
{
    while (first!=last)
    {
       construct(&(*first), val);
        ++first;
    }
}

template<typename InputIter,typename DesIter>
void uninitialized_copy(InputIter first,InputIter last,DesIter destination)
{
    while (first!=last)
    {
       construct(&(*first), *destination);
        ++destination;
        ++first;
    }
}

#endif