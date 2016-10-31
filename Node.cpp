//
//  Node.cpp
//  octopus
//
//  Created by Stijn on 28/10/2016.
//
//

#include <cassert>

#include "Node.hpp"

namespace octo
{
    Node::~Node()
    {
        auto temp = dependencies;
        for (auto& dependency : temp)
            dependency->dependentWillBeDestructed(*this);
        
        assert(dependencies.empty());
    }
}
