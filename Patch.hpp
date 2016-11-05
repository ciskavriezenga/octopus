//
//  Patch.hpp
//  octopus
//
//  Created by Stijn Frishert on 10/19/16.
//
//

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
        void addOutput(const std::string& name, Clock& clock)
        {
            outputValues.emplace(name, std::make_unique<Value<T>>(clock));
            Group::addOutput(name, dynamic_cast<Value<T>&>(*outputValues[name]));
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
            
            dynamic_cast<Value<T>&>(*outputValues[output]) = dynamic_cast<Signal<T>&>(node.asSignalBase());
        }
        
        //! Change one of the outputs
        /*! @param output: The name of the output that should be assigned
            @param node: The name of the node that should be assigned to the output */
        template <class T>
        void unassignOutput(const std::string& output)
        {
            dynamic_cast<Value<T>&>(*outputValues[output]) = T{};
        }
        
    private:
        //! The nodes in the patch
        std::unordered_map<std::string, Node> nodes;
        
        //! These values (as polymorphic signal bases) will be used when one of the outputs is being unassigned
        std::unordered_map<std::string, std::unique_ptr<SignalBase>> outputValues;
    };
}

#endif
