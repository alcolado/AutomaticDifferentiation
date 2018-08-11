//
//  T.hpp
//  AutoDiff3
//
//  Created by Adam Alcolado on 2018-08-07.
//  Copyright © 2018 Adam. All rights reserved.
//

#ifndef T_hpp
#define T_hpp

#include <iostream>
#include <stdio.h>
#include <Eigen/Dense>


struct T
{
    typedef double ScalarType;
    typedef Eigen::Array<ScalarType, Eigen::Dynamic, Eigen::Dynamic> ArrayType;
    
    T();
    T(ScalarType s);
    T(ArrayType t);
    
    operator ScalarType();
    operator ArrayType();
    
    ScalarType& operator()(int i, int j);
    
    T Multiply(T rhs);
    T Add(T rhs);
    T MatMul(T rhs);
    T Transpose();
    
    T ZerosLike();
    T OnesLike();
    T ReduceSum();
    
    static T Rotation2D(T angle);
    
    const static T HalfPi;
    
    ArrayType Get();
    ArrayType m_value;
};

#endif /* T_hpp */
