//
//  Array.hpp
//  AutomaticDifferentiation
//
//  Created by Adam Alcolado on 2018-08-12.
//  Copyright © 2018 Adam. All rights reserved.
//

#ifndef Array_hpp
#define Array_hpp

#include <stdio.h>
#include <vector>

template <typename U, typename Derived>
struct ArrayBase
{
    std::vector<int> m_shape;
    std::vector<int> m_strides;
    std::vector<int> m_backstrides;
    std::vector<int> m_counter;
    int m_rank;
    size_t m_size;
    
    ArrayBase(const std::vector<int>& shape, const std::vector<int>& strides) :
        m_shape(shape),
        m_strides(strides),
        m_counter(shape.size(), 0),
        m_rank((int) shape.size())
    {
        m_backstrides.resize(m_rank);
        m_size = 1;
        for (int i = 0; i < m_rank; ++i)
        {
            m_backstrides[i] = (shape[i]-1)*m_strides[i];
            m_size *= m_shape[i];
        }
    }
    
    ArrayBase(const std::vector<int>& shape) :
    m_shape(shape),
    m_counter(shape.size(), 0),
    m_rank((int) shape.size())
    {
        m_strides.resize(m_rank);
        m_backstrides.resize(m_rank);
        m_size = 1;
        for (int i = m_rank - 1; i >= 0; --i)
        {
            m_strides[i] = (int) m_size;
            m_backstrides[i] = (shape[i]-1)*m_strides[i];
            m_size *= m_shape[i];
        }
    }
    
    ArrayBase<U, Derived>& Increment()
    {
        return static_cast<Derived*>(this)->Increment();
    }
    
    ArrayBase<U, Derived>& Reset()
    {
        return static_cast<Derived*>(this)->Reset();
    }
    
    U& Dereference() const
    {
        return static_cast<Derived*>(this)->Derefence();
    }
    
    U ConstReference() const
    {
        return static_cast<Derived*>(this)->ConstRefence();
    }
    
    U& Element(std::vector<int> pos) const
    {
        return static_cast<Derived*>(this)->Element(pos);
    }
    
    ArrayBase<U, Derived> Transpose(const std::vector<int>& perm) const
    {
        return static_cast<Derived*>(this)->Transpose(perm);
    }
    
    ArrayBase<U, Derived> SelectRow(int i) const
    {
        return static_cast<Derived*>(this)->SelectRow(i);
    }
};


template <typename U>
struct ArrayView : public ArrayBase<U, ArrayView<U>>
{
    U* const m_begin;
    U* m_ptr;
    
//    std::vector<int> m_shape;
//    std::vector<int> m_strides;
//    std::vector<int> m_backstrides;
//    std::vector<int> m_counter;
//    int m_rank;
//    size_t m_size;
//    
//    ArrayView(U* begin, const std::vector<int>& shape, const std::vector<int>& strides) :
//    m_begin(begin),
//    m_ptr(begin),
//    m_shape(shape),
//    m_strides(strides),
//    m_counter(shape.size(), 0),
//    m_rank((int) shape.size())
//    {
//        m_backstrides.resize(m_rank);
//        m_size = 1;
//        for (int i = 0; i < m_rank; ++i)
//        {
//            m_backstrides[i] = (shape[i]-1)*m_strides[i];
//            m_size *= m_shape[i];
//        }
//    }
//    
//    ArrayView(U* begin, const std::vector<int>& shape) :
//    m_begin(begin),
//    m_ptr(begin),
//    m_shape(shape),
//    m_counter(shape.size(), 0),
//    m_rank((int) shape.size())
//    {
//        m_strides.resize(m_rank);
//        m_backstrides.resize(m_rank);
//        m_size = 1;
//        for (int i = m_rank - 1; i >= 0; --i)
//        {
//            m_strides[i] = (int) m_size;
//            m_backstrides[i] = (shape[i]-1)*m_strides[i];
//            m_size *= m_shape[i];
//        }
//    }
    
    typedef ArrayBase<U, ArrayView<U>> Base;
    
    ArrayView(U* begin, const std::vector<int>& shape, const std::vector<int>& strides) :
        m_begin(begin),
        m_ptr(begin),
        Base(shape, strides)
    {}
    
    ArrayView(U* begin, const std::vector<int>& shape) :
    m_begin(begin),
    m_ptr(begin),
    Base(shape)
    {}
    
    ArrayView<U>& Increment()
    {
        for (int axis = Base::m_rank - 1; axis >= 0; --axis)
        {
            Base::m_counter[axis] += 1;
            if (Base::m_counter[axis] >= Base::m_shape[axis])
            {
                m_ptr -= Base::m_backstrides[axis];
                Base::m_counter[axis] = 0;
            }
            else
            {
                m_ptr += Base::m_strides[axis];
                break;
            }
        }
        return *this;
    }
    
    ArrayView<U>& Reset()
    {
        for (int i = 0; i < Base::m_rank; i++)
        {
            Base::m_counter[i] = 0;
        }
        m_ptr = m_begin;
        return *this;
    }
    
    U& Dereference() const
    {
        return *m_ptr;
    }
    
    U ConstReference() const
    {
        return *m_ptr;
    }
    
    U& Element(std::vector<int> pos) const
    {
        int offset = 0;
        for (int i = 0; i < Base::m_rank; ++i)
        {
            offset += pos[i]*Base::m_strides[i];
        }
        return *(m_begin + offset);
    }
    
    ArrayView<U>& TransposeInPlace(const std::vector<int>& perm)
    {
        std::vector<int> old_shape = Base::m_shape;
        std::vector<int> old_strides = Base::m_strides;
        std::vector<int> old_backstrides = Base::m_backstrides;
        std::vector<int> old_counter = Base::m_counter;
        for(int i = 0; i < Base::m_rank; ++i)
        {
            int j = perm[i];
            Base::m_shape[i] = old_shape[j];
            Base::m_strides[i] = old_strides[j];
            Base::m_backstrides[i] = old_backstrides[j];
            Base::m_counter[i] = old_counter[j];
        }
        return *this;
    }
    
    ArrayView<U> Transpose(const std::vector<int>& perm) const
    {
        ArrayView<U> result = *this;
        result.TransposeInPlace(perm);
        return result;
    }
    
    ArrayView<U> SelectRow(int i) const
    {
        std::vector<int> shape(Base::m_shape.begin()+1, Base::m_shape.end());
        std::vector<int> strides(Base::m_strides.begin()+1, Base::m_strides.end());
        return ArrayView<U>(m_begin + i*Base::m_strides[0], shape, strides);
    }
    
    // data changing operations
    ArrayView<U>& SetConst(const U& x)
    {
        Reset();
        for (int i = 0; i < Base::m_size; i++)
        {
            Dereference() = x;
            Increment();
        }
        return *this;
    }
};

template <typename U>
struct Add : public ArrayBase<U, Add<U>>
{
    typedef ArrayBase<U, Add<U>> Base;
    ArrayView<U> m_lhs;
    ArrayView<U> m_rhs;
    
    Add(const ArrayView<U>& lhs, const ArrayView<U>& rhs) :
        m_lhs(lhs),
        m_rhs(rhs),
        Base(lhs.m_shape)
    {}
    
    U ConstReference()
    {
        return m_lhs.Dereference() + m_rhs.Dereference();
    }
};


#endif /* Array_hpp */
