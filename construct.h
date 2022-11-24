#ifndef CONSTRUCT_H
#define CONSTRUCT_H

#include <new>
#include <utility>
#include <memory>

//理解析构和构造的详细过程，是怎么进行的

//new的三种用法
//1.new operator
//2.operator new
//3.placement new(属于operator new的一种特殊用法，是operator new的特殊重载类型)

// placement new 的new函数重载类型是
// void *operator new(size_t_n,void*_p)


template <typename T>
inline void construct_no_value(T *p)
{
    ::new (static_cast<void*>(p))T;
}

template <typename T, typename... Args>
inline void construct(T *p, Args &&...args)
{
    ::new (static_cast<void *>(p))T(std::forward<Args>(args)...);
}

template <typename T>
inline void Destroy(T *p)
{
   p->~_T();//这里调用析构函数后会free掉内存吗？
}

//为什么要有_construct_aux？
//如果用户不定义析构函数，而是用系统自带的，则说明，析构函数基本没有什么用（但默认会被调用）我们称之为trivial destructor。
//反之，如果特定定义了析构函数，则说明需要在释放空间之前做一些事情，则这个析构函数称为non-trivial destructor。
//如果某个类中只有基本类型的话是没有必要调用析构函数的，delete p的时候基本不会产生析构代码。
template <bool>
struct construct_aux
{
    template <typename Iterator>
    static inline void destroy(Iterator first,Iterator last)
    {
        for (;first !=last; ++first)
           Destroy(&*first);
    }
};


template<>
struct construct_aux<true>
{
    template <typename Iterator>
    static inline void destroy(Iterator first,Iterator last){}
};






// template <typenameIterator>
// inline voidDestroy(_Iteratorfirst,Iteratorlast)
// {
//     usingValue_Type =typename iterator_traits<_Iterator>::value_type;
//     //__has_trivial_destructor是编译器内置函数，判断是否有自定义析构函数
//     //如果是默认析构函数，则析构函数体内什么也不执行，return 1
//     //如果是自定义析构函数，return 0
//    construct_aux<__has_trivial_destructor(_Value_Type)>::_destroy(_first,last); 
// }



#endif