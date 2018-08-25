#include "Tests.hpp"

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

void TestAutoDiff4()
{
    //ValueNode A;
    //ValueNode B;
    NodeRef A;
    NodeRef B;
    
    A.m_node->SetValue(0.13);
    B.m_node->SetValue(-0.2);
    
    B = A + B;
    
    MatrixMultiplicationNode C(*B.m_node, *A.m_node);
    
    ReduceSumNode D(C);
    
    D.Forward();
    D.Grad();
    
    std::cout << "grad : " << A.m_node->GetGrad().Get() << std::endl;
}


void TestArray3()
{
    // create array
    float x[30];
    ArrayView<float> t(&x[0], {2,5,3});
    
    // set elements
    for (int i = 0; i < t.m_size; i ++)
    {
        t.Dereference() = i+5;
        t.Increment();
    }
    t.SelectRow(1).SetConst(2.3);
    
    t.Element({1,1,1}) = 987;
    
    // show result
    for (int i = 0; i < t.m_size; i ++)
    {
        std::cout << i << " " << t.Dereference() << std::endl;
        t.Increment();
    }
    
    t.Reset();
    
    AddArrayArray<float, ArrayView<float>, float, ArrayView<float>> sum(t, t);
    
    std::cout << sum.ConstReference() << std::endl;
    
    std::cout << t;
}
