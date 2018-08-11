//
//  main.cpp
//  AutoDiff3
//
//  Created by Adam Alcolado on 2018-07-29.
//  Copyright © 2018 Adam. All rights reserved.
//

#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include "Node.hpp"

void TestAutoDiff()
{
    T::ArrayType alpha = T::ArrayType::Constant(2, 2, 3);
    T::ArrayType beta = T::ArrayType::Constant(2, 2, 2);
    
    ValueNode A;
    ValueNode B;
    A.SetValue(alpha);
    B.SetValue(beta);
    
    T::ArrayType tmp = A.GetValue();
    
    
    
    //ValueNode B(b);
    //MultiplicationNode C(A, A);
    MatrixMultiplicationNode C(A, B);
    ReduceSumNode D(C);
    
    D.Forward();
    D.Grad();
    
    std::cout << A.GetValue().Get() << std::endl;
    std::cout << B.GetValue().Get() << std::endl;
    std::cout << C.GetValue().Get() << std::endl;
    std::cout << D.GetValue().Get() << std::endl;
    
    std::cout << A.GetGrad().Get() << std::endl;
    std::cout << B.GetGrad().Get() << std::endl;
    std::cout << C.GetGrad().Get() << std::endl;
    std::cout << D.GetGrad().Get() << std::endl;
    
}

void TestAutoDiff2()
{
    T::ArrayType alpha = T::ArrayType::Constant(1, 1, 0.13);
    
    ValueNode A;
    A.SetValue(0.13);

    RotationMatrix2DNode B(A);
    
    MatrixMultiplicationNode C(B, B);

    ReduceSumNode D(C);
    
    D.Forward();
    D.Grad();
    
    
    std::cout << A.GetValue().Get() << std::endl;
    std::cout << B.GetValue().Get() << std::endl;
    std::cout << C.GetValue().Get() << std::endl;
    std::cout << D.GetValue().Get() << std::endl;
    
    
    std::cout << "grad : " << A.GetGrad().Get() << std::endl;
    std::cout << B.GetGrad().Get() << std::endl;
    std::cout << C.GetGrad().Get() << std::endl;
}

void TestAutoDiff3()
{
    ValueNode A;
    ValueNode B;
    A.SetValue(0.13);
    B.SetValue(-0.2);
    
    RotationMatrix2DNode RA(A);
    RotationMatrix2DNode RB(B);
    
    AdditionNode S(RA, RB);
    
    MatrixMultiplicationNode C(RA, S);
    
    ReduceSumNode D(C);
    
    D.Forward();
    D.Grad();
    
    std::cout << "grad : " << A.GetGrad().Get() << std::endl;
}


int main(int argc, const char * argv[])
{
    TestAutoDiff3();
}
