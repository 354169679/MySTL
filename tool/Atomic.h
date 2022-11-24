#ifndef ATOMIC_H
#define ATOMIC_H

#include "NoCopyAble.h"
#include <stdint.h>

template<typename T>
class AtomicIntegerT:NoCopyAble
{
private:
    volatile T value_;

public:
    AtomicIntegerT() : value_(0){}
    T get()
    {   
        return __sync_val_compare_and_swap(&value_, 0, 0);
    }

    T getAndAdd(T x)
    {
        return __sync_fetch_and_add(&value_, x);
    }

    T addAndGet(T x)
    {
        return getAndAdd(x) + x;
    }

    T incrementAndGet()
    {
        return addAndGet(1);
    }

    T decrementAndGet()
    {
        return addAndGet(-1);
    }

    void add(T x)
    {
        getAndAdd(x);
    }

    void increment()
    {
        incrementAndGet();
    }

    void decrement()
    {
        decrementAndGet();
    }

    T getAndSet(T new_value)
    {
        return __sync_lock_test_and_set(&value_, new_value);
    }

    ~AtomicIntegerT();
};

using AtomicInt32 = AtomicIntegerT<int32_t>;
using AtomicInt64 = AtomicIntegerT<int64_t>;

#endif