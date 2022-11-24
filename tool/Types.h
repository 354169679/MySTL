#ifndef TYPES_H
#define TYPES_H

#include <string.h>
#include <stdint.h>
#include <string>

#ifndef NDEBUG
#include <assert.h>
#endif

inline void memZero(void *p, size_t len)
{
    memset(p, 0, len);
}

template <typename To, typename From>
inline To implicite_cast(const From &f)
{
    return f;
}

template <typename To, typename From>
inline To down_cast(From *f)
{
    if (false)
    {
        implicit_cast<From *, To>(0);
    }

#if !defined(NDEBUG) && !defined(GOOGLE_PROTOBUF_NO_RTTI)
    assert(f == NULL || dynamic_cast<To>(f) != NULL); // RTTI: debug mode only!
#endif
    return static_cast<To>(f);
}

#endif