#ifndef UNINITIALIZED_H
#define UNINITIALIZED_H


#include "construct.h"

template<typename ForwardIterator,typename T_val>
void uninitialized_fill(ForwardIterator _first,ForwardIterator _last,const T_val& val)
{
    while (_first!=_last)
    {
        _construct(&(*_first), val);
    }
}


#endif