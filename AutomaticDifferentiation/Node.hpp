#ifndef Node_hpp
#define Node_hpp

#define NODE_DEBUG_LEVEL 1
#if NODE_DEBUG_LEVEL > 0
#define NodeDebug(x) x
#else
#define NodeDebug(x)
#endif



#include <stdio.h>
#include <iostream>
#include "T.hpp"

class NodeBase
{
    typedef std::pair<NodeBase*, int> NodeConnection;
public:
    NodeBase(int num_inputs, const char* name = "Node");
    
    virtual void Forward();
    virtual void Reverse();
    virtual void Reset();
    
    virtual void Grad(); 
    
    virtual T ComputeValue();
    virtual void InitializeGrad();
    
    virtual T ComputeGradAtInput(int i);
    
    // setters and getters
    T GetValue();
    T GetGrad();
    void SetValue(T value);
    void SetGrad(T grad);
    
    NodeBase* GetInput(int i);
    void AddInput(int i, NodeBase& source);
    
    T GetValueAtInput(int i);
    
    const char* GetName();
    
protected:
    
    // members
    int m_num_inputs;
    NodeBase** m_inputs;
    
    T m_value;
    T m_grad;
    
    //bool m_isValueComputed;
    //bool m_isGradInitialized;
    int m_visits_forward;
    int m_visits_reverse;
    
    const char* m_name;
};

class ValueNode : public NodeBase
{
public:
    ValueNode();
};

class MultiplicationNode : public NodeBase
{
public:
    MultiplicationNode(NodeBase& lhs, NodeBase& rhs);
    virtual T ComputeValue();
    virtual T ComputeGradAtInput(int i);
};

class AdditionNode : public NodeBase
{
public:
    AdditionNode(NodeBase& lhs, NodeBase& rhs);
    virtual T ComputeValue();
    virtual T ComputeGradAtInput(int i);
};

class ReduceSumNode : public NodeBase
{
public:
    ReduceSumNode(NodeBase& lhs);
    virtual T ComputeValue();
    virtual T ComputeGradAtInput(int i);
};

class MatrixMultiplicationNode : public NodeBase
{
public:
    MatrixMultiplicationNode(NodeBase& lhs, NodeBase& rhs);
    virtual T ComputeValue();
    virtual T ComputeGradAtInput(int i);
};

class RotationMatrix2DNode : public NodeBase
{
public:
    RotationMatrix2DNode(NodeBase& angles);
    virtual T ComputeValue();
    virtual T ComputeGradAtInput(int i);
};


#endif 
