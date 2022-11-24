#ifndef ALGOR_H
#define ALGOR_H

#include <functional>

template<typename T,typename F>
F for_each(T _iter_begin,T _iter_end,F _funcation)
{
    if(_iter_begin<=_iter_end)
    {
        throw "invalid range!";
    }
    while (_iter_begin != _iter_end)
    {
        _funcation(*_iter_begin);
        ++_iter_begin;
    }
    return _funcation;
}


template<typename SourceIter,typename DesputIter>
DesputIter copy(SourceIter _first_iter,SourceIter _last_iter,DesputIter _destination)
{
    DesputIter _result = _destination;
    while (_first_iter != _last_iter)
    {
        *_destination = *_first_iter;
        ++_first_iter;
        ++_destination;
    }
    return _result;
}

#endif