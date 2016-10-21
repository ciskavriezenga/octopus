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
#include "Value.hpp"

namespace octo
{
    class Clock;
    
    class Patch : public Group
    {
    public:
        //! Add a new node into the patch
        void addNode(const std::string& name, std::unique_ptr<Node> node);
        
        //! Remove a node from the patch
        void removeNode(const std::string& name);
        
        //! Retrieve a node from the patch
        Node& getNode(const std::string& name);
        
        //! Add a new output to the patch
        template <class T>
        void addOutput(const std::string& name, Clock& clock)
        {
            Group::addOutput<Value<T>>(name, clock);
        }
        
        //! Change one of the outputs
        /*! @param output: The name of the output that should be assigned
            @param node: The name of the node that should be assigned to the output */
        template <class T>
        void assignOutput(const std::string& output, const std::string& node)
        {
            dynamic_cast<Value<T>&>(getOutput<T>(output)) = dynamic_cast<Signal<T>&>(getNode(node));
        }
        
        //! Change one of the outputs
        /*! @param output: The name of the output that should be assigned
         @param node: The name of the node that should be assigned to the output */
        template <class T>
        void unassignOutput(const std::string& output)
        {
            dynamic_cast<Value<T>&>(getOutput<T>(output)) = T{};
        }
        
    private:
        //! The nodes in the patch
        std::unordered_map<std::string, std::unique_ptr<Node>> nodes;
    };
}

#endif
