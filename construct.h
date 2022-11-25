#ifndef CONSTRUCT_H
#define CONSTRUCT_H

#include "iterator.h"
#include <new>
#include <utility>

//理解析构和构造的详细过程，是怎么进行的

// new的三种用法
// 1.new operator
// 2.operator new
// 3.placement new(属于operator new的一种特殊用法，是operator new的特殊重载类型)

// placement new 的new函数重载类型是
// void *operator new(size_t n,void* p)

//两种construct函数
template <typename T>
static inline void Construct_NoValue(T *p)
{
    ::new (static_cast<void *>(p)) T;
}

template <typename T, typename... Args>
static inline void Construct(T *p, Args &&...args)
{
    ::new (static_cast<void *>(p)) T(std::forward<Args>(args)...);
}

//三种destroy函数
template <typename T>
static inline void Destroy(T *p)
{
    p->~T(); //这里调用析构函数后会free掉内存吗？
}

template <typename Iterator>
static void Destroy(Iterator, Iterator);

// template <typename Iterator, typename Size>
// static Iterator Destroy(Iterator, Size);

//为什么要有_construct_aux？
//如果用户不定义析构函数，而是用系统自带的，则说明，析构函数基本没有什么用（但默认会被调用）我们称之为trivial destructor。
//反之，如果特定定义了析构函数，则说明需要在释放空间之前做一些事情，则这个析构函数称为non-trivial destructor。
//如果某个类中只有基本类型的话是没有必要调用析构函数的，delete p的时候基本不会产生析构代码。
template <bool>
struct destroy_aux
{
    template <typename Iterator>
    static inline void destroy(Iterator first, Iterator last)
    {
        for (; first != last; ++first)
            Destroy(&*first);
    }
};

template <>
struct destroy_aux<true>
{
    template <typename Iterator>
    static inline void destroy(Iterator first, Iterator last) {}
};

//__has_trivial_destructor是编译器内置函数，判断是否有自定义析构函数
//如果是默认析构函数，则析构函数体内什么也不执行，return 1
//如果是自定义析构函数，return 0
template <typename Iterator>
inline void Destroy(Iterator first, Iterator last)
{

    using Value_Type = typename iterator_traits<Iterator>::value_type;
    destroy_aux<__has_trivial_destructor(Value_Type)>::destroy(first, last);
}

//Destroy的第三个重载，SGI老版本没有该重载函数

// template <bool>
// struct destroy_aux_n
// {
//     template <typename Iterator, typename Size>
//     inline Iterator destroy(Iterator iter, Size size)
//     {
//         while (size>0)
//         {
//             --size;
//             Destroy(&*iter);
//             ++iter;
//         }
//         return iter;
//     }
// };

// template <>
// struct destroy_aux_n<true>
// {
//     template <typename Iterator, typename Size>
//     inline Iterator destroy(Iterator iter, Size size)
//     {
//         advance(iter,size); //iterator header
//         return iter;
//     }
// };

// template <typename Iterator, typename Size>
// static inline Iterator Destroy(Iterator iter, Size size)
// {
//     using Value_Type = typename iterator_traits<Iterator>::value_type;
//     return destroy_aux_n<__has_trivial_destructor(Value_Type)>::destroy(iter, size);
// }


#endif