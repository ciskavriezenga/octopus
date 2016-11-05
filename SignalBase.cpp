//
//  SignalBase.cpp
//  octopus
//
//  Created by Stijn Frishert on 11/5/16.
//
//

#include <cassert>

#include "SignalBase.hpp"

namespace octo
{
    SignalBase::~SignalBase()
    {
        auto temp = dependencies;
        for (auto& dependency : temp)
            dependency->dependentWillBeDestructed(*this);
        
        assert(dependencies.empty());
    }
}
