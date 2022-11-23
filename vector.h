#ifndef VECTOR_H
#define VECTOR_H

#include "allocate.h"
#include "construct.h"
#include <vector>

template <typename T, typename Alloc>
class vector_base
{
public:
    using allocator_type = Alloc;
    using M_data_allocator = simple_alloc<T, Alloc>;

protected:
    T *M_start;
    T *M_finish;
    T *M_end_of_strage;
    // 分配n个vector_base<T>对象的内存
    T *M_allocate(size_t n)
    {
        return M_data_allocator::allocate(n);
    }
    // 释放由p指向n个vector_base<T>对象的内存
    void M_deallocate(T *p, size_t n)
    {
        M_data_allocator::deallocate(p, n);
    }

    allocator_type get_allocator() const
    {
        return allocator_type();
    }

    vector_base() : M_start(nullptr), M_finish(nullptr), M_end_of_strage(nullptr) {}
    vector_base(const Alloc &) : M_start(nullptr), M_finish(nullptr), M_end_of_strage(nullptr) {}
    vector_base(size_t n, const Alloc &a)
    {
        M_start = M_allocate(n);
        M_finish = M_start;
        M_end_of_strage = M_start + n; //内存++根据变量大小
    }

    ~vector_base()
    {
        M_deallocate(M_start, M_end_of_strage - M_start);
        M_start = M_finish = M_end_of_strage = nullptr;
    }
};

template <typename T, typename Alloc = _malloc_allocate<0>> // Alloc depends what allocator will be used
class vector : protected vector_base<T, Alloc>              // usage of template "protected"
{
public:
    using iterator = T *;
    using const_iterator = const T *;
    using size_type = size_t;
    using reference = T &;
    using const_reference = const T &;
    using base_ = vector_base<T, Alloc>;
    using allocator_type = typename base_::allocator_type; // usage of "using"

public:
    vector() = default;
    vector(const Alloc &a) : base_(a) {}
    vector(size_t n, const Alloc &a) : base_(n, a) {} // allocate N of T item

    allocator_type get_allocator() const
    {
        return base_::get_allocator(); // usage funcation of base;
    }

    //踩坑记录
    // why use "base_::M_start"
    // return M_start;  error!!!!
    iterator begin()
    {

        return base_::M_start;
        // or return this->M_start;
    }

    iterator end()
    {
        return base_::M_finish;
    }

    const_iterator begin() const
    {
        return base_::M_start;
    }

    const_iterator end() const
    {
        return base_::M_finish;
    }

    size_type size() const
    {
        return static_cast<size_type>(end() - begin());
    }

    size_type capacity() const
    {
        return static_cast<size_type>(base_::M_end_of_strage - begin());
    }

    bool empty() const
    {
        return end() == begin();
    }

    reference operator[](size_type index)
    {
        return *(base_::M_start + index);
    }

    reference operator[](size_type index) const
    {
        return *(base_::M_start + index);
    }

    void push_back(const T &val)
    {
        if (base_::M_end_of_strage != base_::M_finish)
        {
            _construct(base_::_finish, val);
            base_::M_finish++;
        }
        else
        {
        }
    }

private:
};

#endif