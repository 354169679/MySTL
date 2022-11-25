#ifndef VECTOR_H
#define VECTOR_H

#include "allocate.h"
#include "construct.h"
#include "algor.h"
#include "uninitialized.h"
#include <vector>

template <typename T, typename Alloc>
class vector_base
{
protected:
    using allocator_type = Alloc;
    using M_data_allocator = simple_alloc<T, Alloc>;
    T *M_start;
    T *M_finish;
    T *M_end_of_strage;

protected:
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
};

template <typename T, typename Alloc = malloc_allocate<0>> // Alloc depends what allocator will be used
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

protected: //使用using声明
    using base_::M_allocate;
    using base_::M_deallocate;
    using base_::M_end_of_strage;
    using base_::M_finish;
    using base_::M_start;

private:
    void M_insert_aux(iterator, const T &);
    iterator M_alloc_and_copy(iterator fist, iterator last, size_t n)
    {
        iterator result = M_allocate(n);
        uninitialized_copy(fist, last, result);
        return result;
    }
    void destroy(iterator begin, iterator end)
    {
        auto temp_first =begin;
        while (begin !=end)
        {
           Destroy(begin);
            ++begin;
        }
    }

    void require_vector_no_empty() const noexcept
    {
        if (empty())
            throw "empty";
    }

public:
    vector() = default;
    vector(const Alloc &a) : base_(a) {}
    vector(size_t n, const Alloc &a) : base_(n, a) {} // allocate N of T item
    vector(size_t n,const T& val)
    {
        M_start= M_allocate(n);
        M_finish = M_end_of_strage = M_start + n;
        uninitialized_fill(M_start, M_finish, val);
    }
    allocator_type get_allocator() const
    {
        return base_::get_allocator(); // usage funcation of base_;
    }

    //踩坑记录
    // why use "base_::M_start"
    // return M_start;  error!!!!
    iterator begin()
    {

        // or return base_::M_start;
        return M_start;
    }

    iterator end()
    {
        return M_finish;
    }

    const_iterator begin() const
    {
        return M_start;
    }

    const_iterator end() const
    {
        return M_finish;
    }

    size_type size() const
    {
        return static_cast<size_type>(end() - begin());
    }

    size_type length() const
    {
        return static_cast<size_t>(end() - begin());
    }

    size_type capacity() const
    {
        return static_cast<size_type>(M_end_of_strage - begin());
    }

    bool empty() const
    {
        return end() == begin();
    }

    reference operator[](size_type index)
    {
        return *(M_start + index);
    }

    reference operator[](size_type index) const
    {
        return *(M_start + index);
    }

    vector &operator=(const vector &);

    void push_back(const T &val)
    {
        if (M_end_of_strage != M_finish)
        {
           construct(M_finish, val);
            ++M_finish;
        }
        else
        {
            M_insert_aux(end(), val);
        }
    }

    const_reference at(size_t n) const
    {
        if (n >= length())
        {
            throw "out of range";
        }
        return *(M_start + n);
    }

    reference at(size_t n)
    {
        if (n >= length())
        {
            throw "out of range";
        }
        return *(M_start + n);
    }

    const_reference back() const
    {
        if (empty())
        {
            throw "empty";
        }
        return *(M_finish - 1);
    }

    reference back()
    {
        require_vector_no_empty();
        return *(M_finish - 1);
    }

    reference front()
    {
        require_vector_no_empty();
        return *(M_finish - 1);
    }

    const_reference front() const
    {
        require_vector_no_empty();
        return *(M_finish - 1);
    }

    void pop_back() noexcept
    {
        require_vector_no_empty();
        --M_finish;
       Destroy(M_finish);
    }

    void clear()noexcept
    {
        require_vector_no_empty();
        destroy(begin(), end());
        M_finish = M_start;
    }

    void shrink_to_fit()noexcept
    {
        if(M_finish!=M_end_of_strage)
        {
            M_deallocate(M_finish, capacity() - end());
            M_end_of_strage = M_finish;
        }
    }

    ~vector()
    {
        destroy(begin(), end());
    }
};

// explanation:

// destination   M_start-----M_finish-------M_end_of_strage
// source         M_start---M_finish(length)

template <typename T, typename Alloc>
vector<T, Alloc> &vector<T, Alloc>::operator=(const vector<T, Alloc> &v)
{
    if (&v != this)
    {
        const size_t v_len =v.length();
        if (capacity() <v_len)
        {
            iterator temp_begin = M_alloc_and_copy(v.begin(),v.end(),v_len);
            destroy(begin(), end());
            M_deallocate(begin(), capacity());
            M_start =temp_begin;
            M_end_of_strage = M_finish;
        }
        else if (capacity() >=v_len && size() >v_len)
        {
            copy(v.begin(),v.begin() + size(), begin());
            uninitialized_copy(v.begin() + size(),v.end(), end());
        }
        else
        {
            copy(v.begin(),v.end(), M_start);
            destroy(M_start +v_len, M_finish);
        }
        M_finish = M_start +v_len;
    }
    return *this;
}

template <typename T, typename Alloc>
void vector<T, Alloc>::M_insert_aux(iterator position, const T &val)
{
    if (M_end_of_strage != M_finish)
    {
       construct(M_finish, *(M_finish - 1));
    }
    else
    {
    }
}

#endif