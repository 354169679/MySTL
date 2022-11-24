#ifndef ALLOCATE_H
#define ALLOCATE_H

#include <stddef.h>
#include <stdlib.h>

//先实现第一种配置器
/*first allocator*/
template <int inst>
class malloc_allocate
{
private:
    static void *s_oom_malloc(size_t);
    static void *s_oom_realloc(void *, size_t);

    // function pointor
    static void (*malloc_alloc_oom_handler)();

public:
    static void *allocate(size_t n)
    {
        void *result = malloc(n);
        if (result == NULL)
            result = s_oom_malloc(n);
        return result;
    }

    static void deallocate(void *p, size_t n)
    {
        free(p);
    }

    static void *reallocate(void *p, size_t new_size)
    {
        void *result = realloc(p, new_size);
        if (result == NULL)
            result = s_oom_realloc(p, new_size);
        return result;
    }

    static void (*set_malloc_handler(void (*f)()))()
    {
        void (*old)() = malloc_alloc_oom_handler;
        malloc_alloc_oom_handler = f;
        return old;
    }
};

template <int inst>
void (*malloc_allocate<inst>::malloc_alloc_oom_handler)() = nullptr;

template <int inst>
void *malloc_allocate<inst>::s_oom_malloc(size_t n)
{
    void (*my_malloc_handler)();
    void *result;

    while (true)
    {
        my_malloc_handler = malloc_alloc_oom_handler;
        if (my_malloc_handler == nullptr)
        {
            throw "bad_alloc";
        }
        my_malloc_handler();
        result = malloc(n);
        if (result)
            return result;
    }
}

template <int inst>
void *malloc_allocate<inst>::s_oom_realloc(void *p, size_t n)
{
    void (*my_alloc_handler)();
    void *result;
    while (true)
    {
        my_alloc_handler = malloc_alloc_oom_handler;
        if (my_alloc_handler == nullptr)
        {
            throw "bad_reallocate";
        }
        my_alloc_handler();
        result = realloc(p, n);
        if (result)
            return result;
    }
}

// defaultinst=0
using malloc_alloc = malloc_allocate<0>;

template <typename T, typename Alloc>
class simple_alloc
{
public:
    static T *allocate(size_t n) //分配n个T类型对象
    {
        return n == 0 ? 0 : (T *)Alloc::allocate(n * sizeof(T));
    }
    static T *allocate()
    {
        return (T *)Alloc::allocate(sizeof(T));
    }
    static void deallocate(T *p, size_t n) //释放n个T类型对象
    {
        Alloc::deallocate(p, sizeof(T) * n);
    }
};

#endif