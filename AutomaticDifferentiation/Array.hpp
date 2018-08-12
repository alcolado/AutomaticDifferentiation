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




template <typename U>
struct View
{
    enum Options
    {
        FULL
    };
    
    int m_size;
    int m_rank;
    int* m_dims;
    int* m_strides;
    U* m_ptr;
    
    View<U> operator[](int i)
    {
        View subview;
        subview.m_size = m_strides[0];
        subview.m_rank = m_rank - 1;
        subview.m_dims = m_dims+1;
        subview.m_strides = m_strides+1;
        subview.m_ptr = m_ptr + m_strides[0]*i;
        return subview;
    }
    
    View<U>& operator=(U val)
    {
        for(int i = 0; i < m_size; i ++)
        {
            m_ptr[i] = val;
        }
        return *this;
    }
    
    void operator[](float x) {}
};


struct Iterator
{
    int m_size;
    int m_rank; // number of axes
    int* m_dims; // dimension of each axis
    int* m_offsets; // offset by increasing the position in each axis by 1
    
    int m_pos; // current offset
    int* m_counter; // current position in each axis
    
    
    void Next()
    {
        int axis = m_rank - 1;
        while (axis >= 0)
        {
            if (m_offsets[axis] != 0)
            {
                m_counter[axis] += 1;
                m_pos += m_offsets[axis];
                if (m_counter[axis] >= m_dims[axis])
                {
                    m_counter[axis] = 0;
                    // careful at the end?
                    m_pos -= m_offsets[axis-1];
                }
                else
                {
                    return;
                }
            }
            axis -= 1;
        }
    }
    
    float* m_ptr;
};




#endif /* Array_hpp */
