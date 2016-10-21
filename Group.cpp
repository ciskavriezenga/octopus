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
    const SignalBase& Group::getOutput(const string& name) const
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
    
    void Group::setOutput(const string& name, const SignalBase& signal)
    {
        outputs[name] = &signal;
    }
    
    void Group::removeOutput(const string& name)
    {
        outputs.erase(name);
    }
}
