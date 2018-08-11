#include "Node.hpp"


/* NodeBase */

NodeBase::NodeBase(int num_inputs, const char* name) :
    m_num_inputs(num_inputs),
    m_name(name),
    m_inputs(new NodeBase*[num_inputs]),
    m_visits_forward(0),
    m_visits_reverse(0)
{
}

const char* NodeBase::GetName()
{
    return m_name;
}

void NodeBase::Forward()
{
    for (int i = 0; i < m_num_inputs; i++)
    {
        NodeBase* node = GetInput(i);
        node->Forward();
    }
    
    if (m_visits_forward == 0)
        m_value = ComputeValue();
    
    m_visits_forward++;
    NodeDebug(printf("Forward : %d, %s \n", m_visits_forward, GetName()));
}

void NodeBase::Reverse()
{
    for (int i = 0; i < m_num_inputs; i++)
    {
        NodeBase* node = GetInput(i);
        if (node->m_visits_reverse == 0)
            node->InitializeGrad();

        node->SetGrad(node->GetGrad().Add(this->ComputeGradAtInput(i)));
        node->m_visits_reverse++;
        NodeDebug(printf("Reverse : %d, %s\n", node->m_visits_reverse, node->GetName()));
        
        if (node->m_visits_reverse == node->m_visits_forward)
        {
            node->Reverse();
        }
    }
}

void NodeBase::Reset()
{
    m_visits_forward = 0;
    m_visits_reverse = 0;
}

void NodeBase::Grad()
{
    m_grad = m_value.OnesLike();
    Reverse();
}

T NodeBase::ComputeValue()
{
    return m_value;
}

void NodeBase::InitializeGrad()
{
    m_grad = m_value.ZerosLike();
}

T NodeBase::ComputeGradAtInput(int i)
{
    return GetValueAtInput(i).ZerosLike();
}

T NodeBase::GetValue()
{
    return m_value;
}

T NodeBase::GetGrad()
{    
    return m_grad;
}

void NodeBase::SetValue(T value)
{
    m_value = value;
}

void NodeBase::SetGrad(T grad)
{
    m_grad = grad;
}

NodeBase* NodeBase::GetInput(int i)
{
    return m_inputs[i];
}

void NodeBase::AddInput(int i, NodeBase& source)
{
    m_inputs[i] = &source;
}

T NodeBase::GetValueAtInput(int i)
{
    return GetInput(i)->GetValue();
}

/* ValueNode */

ValueNode::ValueNode() : NodeBase(0, "Value Node")
{
    
}

/* Multiplication Node */

MultiplicationNode::MultiplicationNode(NodeBase& lhs, NodeBase& rhs) : NodeBase(2, "Multiplication Node")
{
    AddInput(0, lhs);
    AddInput(1, rhs);
}

T MultiplicationNode::ComputeValue()
{
    return GetValueAtInput(0).Multiply(GetValueAtInput(1));
}

T MultiplicationNode::ComputeGradAtInput(int i)
{
    if (i == 0)
        return GetGrad().Multiply(GetValueAtInput(1));
    else
        return GetValueAtInput(0).Multiply(GetGrad());
}

/* Addition Node */

AdditionNode::AdditionNode(NodeBase& lhs, NodeBase& rhs) : NodeBase(2, "Addition Node")
{
    AddInput(0, lhs);
    AddInput(1, rhs);
}

T AdditionNode::ComputeValue()
{
    return GetValueAtInput(0).Add(GetValueAtInput(1));
}

T AdditionNode::ComputeGradAtInput(int i)
{
    return GetGrad();
}

/* Reduce Sum Node */

ReduceSumNode::ReduceSumNode(NodeBase& x) : NodeBase(1, "Reduce Sum Node")
{
    AddInput(0, x);
}

T ReduceSumNode::ComputeValue()
{
    return GetValueAtInput(0).ReduceSum();
}

T ReduceSumNode::ComputeGradAtInput(int i)
{
    return GetValueAtInput(i).OnesLike();
}

/* Matrix Multiplicaiton Node */
MatrixMultiplicationNode::MatrixMultiplicationNode(NodeBase& lhs, NodeBase &rhs) : NodeBase(2, "Matrix Multiplication Node")
{
    AddInput(0, lhs);
    AddInput(1, rhs);
}

T MatrixMultiplicationNode::ComputeValue()
{
    return GetValueAtInput(0).MatMul(GetValueAtInput(1));
}

T MatrixMultiplicationNode::ComputeGradAtInput(int i)
{
    if (i == 0)
        return GetGrad().MatMul(GetValueAtInput(1).Transpose());
    else
        return GetValueAtInput(0).Transpose().MatMul(GetGrad());
}

/** TODO matrix inverse
 
 C = A^-1
 I = A A^-1
 0 = dA A^-1 + A dA^-1
 dA^-1 = - A^-1 dA A^-1
 dC = ...
 
 A_^T = - A^-1 C_^T A^-1
 A_ = - A^-T C_ A^-T
 A_ = - C^T C_ C^T
 
 **/


/* RotationMatrix2D Node */
RotationMatrix2DNode::RotationMatrix2DNode(NodeBase& angle) : NodeBase(1, "Rotation Matrix 2D Node")
{
    AddInput(0, angle);
}

T RotationMatrix2DNode::ComputeValue()
{
    return T::Rotation2D(GetValueAtInput(0));
}

T RotationMatrix2DNode::ComputeGradAtInput(int i)
{
    return (GetGrad().Multiply(T::Rotation2D(GetValueAtInput(0).Add(T::HalfPi)))).ReduceSum();
}



