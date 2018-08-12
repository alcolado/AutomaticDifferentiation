//
//  NodeRef.hpp
//  AutomaticDifferentiation
//
//  Created by Adam Alcolado on 2018-08-11.
//  Copyright © 2018 Adam. All rights reserved.
//

#ifndef NodeRef_hpp
#define NodeRef_hpp

#include <stdio.h>
#include "Node.hpp"


class NodeRef
{
public:
    NodeRef() : m_node(new ValueNode()) {}
    NodeRef(NodeBase* node) : m_node(node) {}
    
    NodeRef operator=(NodeRef rhs) {m_node = rhs.m_node; return *this;}
    NodeRef operator+(NodeRef rhs) {return NodeRef(new AdditionNode(*m_node, *rhs.m_node));}
    
    NodeBase* m_node;
    
};


#endif /* NodeRef_hpp */
