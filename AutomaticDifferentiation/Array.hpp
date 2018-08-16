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


struct ArrayShape
{
    std::vector<int> m_dims; // dimension of each axis
    std::vector<int> m_strides; // byte offset for increasing index of this axis
    std::vector<int> m_counter; // current position in each axis
    
    int m_rank; // number of axes
    size_t m_size; // size
    std::vector<int> m_backstrides; // (n_dims[i]-1)*m_strides[i]
    
    ArrayShape(std::vector<int> dims, std::vector<int> strides) :
        m_dims(dims),
        m_strides(strides),
        m_counter(dims.size(), 0),
        m_rank((int) dims.size())
    {
        m_size = 1;
        for (int i = 0; i < m_rank; i++)
        {
            m_size *= m_dims[i];
            m_backstrides.push_back((m_dims[i]-1)*m_strides[i]);
        }
    }
    
    ArrayShape(std::vector<int> dims) :
    m_dims(dims),
    m_counter(dims.size(), 0),
    m_rank((int) dims.size())
    {
        m_strides.resize(m_rank);
        m_backstrides.resize(m_rank);
        m_size = 1;
        for (int i = m_rank - 1; i >= 0; i--)
        {
            m_strides[i] = (int) m_size;
            m_backstrides[i] = (m_dims[i]-1)*m_strides[i];
            m_size *= m_dims[i];
        }
    }

};

template <typename U>
struct ArrayIterator
{
    U* m_ptr; // current iterator
    U* m_begin; // initial ptr
    std::vector<int> m_dims; // dimension of each axis
    std::vector<int> m_strides; // byte offset for increasing index of this axis
    std::vector<int> m_counter; // current position in each axis

    int m_rank; // number of axes
    size_t m_size; // size
    std::vector<int> m_backstrides; // (n_dims[i]-1)*m_strides[i]
    
    ArrayIterator() {}
    
    ArrayIterator(U* ptr, std::vector<int> dims, std::vector<int> strides) :
        m_ptr(ptr),
        m_begin(ptr),
        m_dims(dims),
        m_strides(strides),
        m_counter(dims.size(), 0)
    {
        m_rank = (int) dims.size();
        m_size = 1;
        for (int i = 0; i < m_rank; i++)
        {
            m_size *= m_dims[i];
            m_backstrides.push_back((m_dims[i]-1)*m_strides[i]);
        }
    }
    
    U Dereference() {return *m_ptr;}
    
    U* Begin() {return m_begin;}
    
    void Next()
    {
        int axis = m_rank - 1;
        while (axis >= 0)
        {
            if (m_strides[axis] != 0)
            {
                m_counter[axis] += 1;
                if (m_counter[axis] >= m_dims[axis])
                {
                    m_counter[axis] = 0;
                    m_ptr -= m_backstrides[axis];
                }
                else
                {
                    m_ptr += m_strides[axis];
                    return;
                }
            }
            axis -= 1;
        }
    }
};




#endif /* Array_hpp */
