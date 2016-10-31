#ifndef OCTOPUS_NODE_HPP
#define OCTOPUS_NODE_HPP

#include <set>

namespace octo
{
    //! Basis of all signals and group signals in Octopus
    class Node
    {
    public:
        //! Virtual destructor, because this is a polymorphic base class
        virtual ~Node();
        
    public:
        //! The dependencies of this node
        mutable std::set<Node*> dependencies;
        
    private:
        //! Notify dependencies this node will be destructed
        virtual void dependentWillBeDestructed(Node& dependent) { }
    };
}

#endif
