#ifndef ALGOR_H
#define ALGOR_H

#include <functional>

template <typename T, typename F>
F for_each(T iter_begin, T iter_end, F funcation)
{
    if (iter_begin <= iter_end)
    {
        throw "invalid range!";
    }
    while (iter_begin != iter_end)
    {
        funcation(*iter_begin);
        ++iter_begin;
    }
    return funcation;
}

template <typename SourceIter, typename DesIter>
DesIter copy(SourceIter first_iter, SourceIter last_iter, DesIter destination)
{
    DesIter result = destination;
    while (first_iter != last_iter)
    {
        *destination = *first_iter;
        ++first_iter;
        ++destination;
    }
    return result;
}

#endif