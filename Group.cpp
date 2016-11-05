//
//  Group.cpp
//  octopus
//
//  Created by Stijn Frishert on 10/21/16.
//
//

#include <stdexcept>

#include "Group.hpp"

using namespace std;

namespace octo
{
    SignalBase& Group::getOutput(const string& name)
    {
        auto it = outputs.find(name);
        if (it == outputs.end())
            throw runtime_error("no output with the name '" + name + "'");
        
        return *it->second;
    }
    
    set<string> Group::getOutputNames() const
    {
        set<string> keys;
        for (auto& output : outputs)
            keys.emplace(output.first);
        
        return keys;
    }
    
    void Group::addOutput(const string& name, SignalBase& signal)
    {
        outputs[name] = &signal;
    }
    
    void Group::removeOutput(const string& name)
    {
        outputs.erase(name);
    }
}
