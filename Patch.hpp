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

#ifndef OCTOPUS_PATCH_HPP
#define OCTOPUS_PATCH_HPP

#include <memory>
#include <string>
#include <unordered_map>

#include "Group.hpp"
#include "Node.hpp"
#include "SignalBase.hpp"
#include "Value.hpp"

namespace octo
{
    class Clock;
    
    //! Owner of multiple nodes, some of them used as outputs for the patch (= group)
    class Patch : public Group
    {
    public:
        //! Add a new node into the patch
        void addNode(const std::string& name, Node node);
        
        //! Remove a node from the patch
        void removeNode(const std::string& name);
        
        //! Retrieve a node from the patch
        Node getNode(const std::string& name) const;
        
        //! Add a new output to the patch
        template <class T>
        Signal<T>& addOutput(const std::string& name, Clock& clock)
        {
            outputValues.emplace(name, std::make_unique<Value<T>>(clock));
            Group::addOutput(name, dynamic_cast<Value<T>&>(*outputValues[name]));
            return dynamic_cast<Signal<T>&>(*outputValues[name]);
        }
        
        //! Change one of the outputs
        /*! @param output: The name of the output that should be assigned
            @param node: The name of the node that should be assigned to the output */
        template <class T>
        void assignOutput(const std::string& output, const std::string& nodeName)
        {
            auto node = getNode(nodeName);
            if (!node.isSignalBase())
                throw std::runtime_error("node at '" + nodeName + "' is not a signal base");
            
            static_cast<Value<T>&>(*outputValues[output]) = static_cast<Signal<T>&>(node.asSignalBase());
        }
        
        //! Change one of the outputs
        /*! @param output: The name of the output that should be assigned
            @param node: The name of the node that should be assigned to the output */
        template <class T>
        void unassignOutput(const std::string& output)
        {
            static_cast<Value<T>&>(*outputValues[output]) = T{};
        }
        
    private:
        //! The nodes in the patch
        std::unordered_map<std::string, Node> nodes;
        
        //! These values (as polymorphic signal bases) will be used when one of the outputs is being unassigned
        std::unordered_map<std::string, std::unique_ptr<SignalBase>> outputValues;
    };
}

#endif
