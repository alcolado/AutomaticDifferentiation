#include "T.hpp"

T::T() 
{
}

T::ArrayType T::Get()
{
    return m_value;
}

T::T(T::ScalarType s)
{
    m_value.resize(1,1);
    m_value(0,0) = s;
}

T::T(T::ArrayType t)
{
    m_value = t;
}

T::ScalarType& T::operator()(int i, int j)
{
    return m_value(i, j);
}

T::operator ScalarType()
{
    return m_value(0,0);
}

T::operator ArrayType()
{
    return Get();
}

T T::Multiply(T rhs)
{
    return T(m_value*rhs.m_value);
}

T T::Add(T rhs)
{
    return T(m_value + rhs.m_value);
}

T T::MatMul(T rhs)
{
    return T(m_value.matrix()*rhs.m_value.matrix());
}

T T::Transpose()
{
    return T(m_value.transpose());
}

T T::OnesLike()
{
    return T(ArrayType::Ones(m_value.rows(), m_value.cols()));
}

T T::ZerosLike()
{
    return T(ArrayType::Zero(m_value.rows(), m_value.cols()));
}

T T::ReduceSum()
{
    return T(m_value.sum());
}

T T::Rotation2D(T angle)
{
    Eigen::Rotation2D<ScalarType> r(angle);
    return T(r.matrix());
}

const T T::HalfPi = 3.1415926/2.0;
