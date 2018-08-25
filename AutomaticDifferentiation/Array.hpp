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
#include <iostream>
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
        return static_cast<const Derived*>(this)->DereferenceImplementation();
    }
    
    U ConstReference() const
    {
        return static_cast<const Derived*>(this)->ConstReferenceImplementation();
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

template <typename U, typename Derived>
std::ostream& operator<<(std::ostream& os, ArrayBase<U, Derived>& a)
{
    os << "printing array!" << std::endl;
    for (int i = 0; i < a.m_size; ++i)
    {
        std::cout << a.ConstReference();
        a.Increment();
    }
    return os;
}


template <typename U>
struct ArrayView : public ArrayBase<U, ArrayView<U>>
{
    typedef ArrayBase<U, ArrayView<U>> Base;
    U* const m_begin;
    U* m_ptr;
    
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
    
    U& DereferenceImplementation() const
    {
        return *m_ptr;
    }
    
    U ConstReferenceImplementation() const
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
            DereferenceImplementation() = x;
            Increment();
        }
        return *this;
    }
};

template <typename ULhs, typename DerivedLhs, typename URhs, typename DerivedRhs>
struct AddArrayArray : public ArrayBase<ULhs, AddArrayArray<ULhs, DerivedLhs, URhs, DerivedRhs>>
{
    typedef ArrayBase<ULhs, AddArrayArray<ULhs, DerivedLhs, URhs, DerivedRhs>> Base;
    DerivedLhs m_lhs;
    DerivedRhs m_rhs;
    
    AddArrayArray(const DerivedLhs& lhs, const DerivedRhs& rhs) :
    m_lhs(lhs),
    m_rhs(rhs),
    Base(lhs.m_shape)
    {}
    
    ULhs ConstReferenceImplementation() const
    {
        return m_lhs.ConstReference() + m_rhs.ConstReference();
    }
};

template <typename ULhs, typename DerivedLhs, typename URhs>
struct AddArrayScalar : public ArrayBase<ULhs, AddArrayScalar<ULhs, DerivedLhs, URhs>>
{
    typedef ArrayBase<ULhs, AddArrayScalar<ULhs, DerivedLhs, URhs>> Base;
    DerivedLhs m_lhs;
    URhs m_rhs;
    
    AddArrayScalar(const DerivedLhs& lhs, const URhs& rhs) :
    m_lhs(lhs),
    m_rhs(rhs),
    Base(lhs.m_shape)
    {}
    
    ULhs ConstReferenceImplementation() const
    {
        return m_lhs.ConstReference() + m_rhs;
    }
};


#endif /* Array_hpp */
