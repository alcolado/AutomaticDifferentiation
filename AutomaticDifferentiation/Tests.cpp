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


void TestArray()
{
    float data[30] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29};
//    int size = 30;
//    int rank = 3;
//    int dims[3] = {3, 2, 5};
//    int strides[3] = {10, 5, 1};
//    int backstrides[3] = {20, 5, 4};
    
    ArrayIterator<float> x(data, {3, 2, 5}, {10, 5, 1});
//    x.m_size = size;
//    x.m_rank = rank;
//    x.m_dims = dims;
//    x.m_strides = strides;
//    x.m_backstrides = backstrides;
//    x.m_begin = data;
//    x.m_ptr = data;
//    
//    int counter[3] = {0, 0, 0};
//    x.m_counter = counter;
    
    for (int i = 0; i < x.m_size; i++)
    {
        std::cout << x.m_counter[0] << x.m_counter[1] << x.m_counter[2] << std::endl;
        std::cout << i << " " << x.Dereference() << std::endl;
        x.Next();
    }
    
    ArrayShape shape({11, 3, 4, 8});
    std::cout << std::endl;
    for (int i = 0; i < shape.m_rank; i ++)
        std::cout << shape.m_strides[i] << " ";
    std::cout << std::endl;

}



