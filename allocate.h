#ifndef ALLOCATE_H
#define ALLOCATE_H

#include <stddef.h>
#include <stdlib.h>

//先实现第一种配置器
/*first allocator*/
template <int __inst>
class _malloc_allocate
{
private:
    static void *_s_oom_malloc(size_t);
    static void *_s_oom_realloc(void *, size_t);

    // function pointor
    static void (*_malloc_alloc_oom_handler)();

public:
    static void *allocate(size_t _n)
    {
        void *_result = malloc(_n);
        if (_result == NULL)
            _result = _s_oom_malloc(_n);
        return _result;
    }

    static void deallocate(void *_p, size_t _n)
    {
        free(_p);
    }

    static void *reallocate(void *_p, size_t _new_size)
    {
        void *_result = realloc(_p, _new_size);
        if (_result == NULL)
            _result = _s_oom_realloc(_p, _new_size);
        return _result;
    }

    static void (*_set_malloc_handler(void (*_f)()))()
    {
        void (*_old)() = _malloc_alloc_oom_handler;
        _malloc_alloc_oom_handler = _f;
        return _old;
    }
};

template <int __inst>
void (*_malloc_allocate<__inst>::_malloc_alloc_oom_handler)() = 0;

template <int __inst>
void *_malloc_allocate<__inst>::_s_oom_malloc(size_t _n)
{
    void (*_my_malloc_handler)();
    void *_result;

    while(true)
    {
        _my_malloc_handler = _malloc_alloc_oom_handler;
        if(_my_malloc_handler==nullptr)
        {
            throw "bad_alloc";
        }
        _my_malloc_handler();
        _result = malloc(_n);
        if(_result)
            return _result;
    }
}


template <int __inst>
void *_malloc_allocate<__inst>::_s_oom_realloc(void *_p, size_t _n)
{
    void (*_my_alloc_handler)();
    void *_result;
    while (true)
    {
        _my_alloc_handler = _malloc_alloc_oom_handler;
        if(_my_alloc_handler==nullptr)
        {
            throw "bad_reallocate";
        }
        _my_alloc_handler();
        _result= realloc(_p, _n);
        if(_result)
            return _result;
    }
}


//default _inst=0
using malloc_alloc = _malloc_allocate<0>;

template<typename  _T,typename _Alloc>
class simple_alloc
{
public:
    static _T* allocate(size_t _n)//分配n个T类型对象
    {
        return _n == 0 ? 0 : (_T *)_Alloc::allocate(_n * sizeof(_T));
    }
    static _T* allocate()
    {
        return (_T *)_Alloc::allocate(sizeof(_T));
    }
    static void deallocate(_T* _p,size_t _n)//释放n个T类型对象
    {
        _Alloc::deallocate(_p, sizeof(_T)*_n);
    }
};



#endif