//
//  Patch.cpp
//  octopus
//
//  Created by Stijn Frishert on 10/21/16.
//
//

#include <stdexcept>
#include <utility>

#include "Patch.hpp"

using namespace std;

namespace octo
{
    void Patch::addNode(const string& name, Node& node)
    {
        nodes.emplace(name, &node);
    }
    
    void Patch::removeNode(const string& name)
    {
        nodes.erase(name);
    }
    
    Node& Patch::getNode(const string& name)
    {
        auto it = nodes.find(name);
        if (it == nodes.end())
            throw runtime_error("no node with the name '" + name + "'");
        
        return *it->second;
    }
    
    const Node& Patch::getNode(const string& name) const
    {
        auto it = nodes.find(name);
        if (it == nodes.end())
            throw runtime_error("no node with the name '" + name + "'");
        
        return *it->second;
    }
}
