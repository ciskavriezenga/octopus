/*
 
 This file is a part of Octopus, a modern C++ library for embedding digital
 signal processing as a language inside your software. It transcends a single
 domain (audio, video, math, etc.), combining multiple clocks in one graph.
 
 Copyright (C) 2016 Dsperados <info@dsperados.com>
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>
 
 --------------------------------------------------------------------
 
 If you would like to use Octopus for commercial or closed-source
 purposes, please contact us for a commercial license.
 
 */

#include <stdexcept>
#include <utility>

#include "Patch.hpp"

using namespace std;

namespace octo
{
    void Patch::addNode(const string& name, Node node)
    {
        nodes.emplace(name, node);
    }
    
    void Patch::removeNode(const string& name)
    {
        nodes.erase(name);
    }
    
    Node Patch::getNode(const string& name) const
    {
        auto it = nodes.find(name);
        if (it == nodes.end())
            throw runtime_error("no node with the name '" + name + "'");
        
        return it->second;
    }
}
