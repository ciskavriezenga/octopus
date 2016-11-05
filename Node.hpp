#ifndef OCTOPUS_NODE_HPP
#define OCTOPUS_NODE_HPP

namespace octo
{
    //! Basis of all signals and group signals in Octopus
    class Node
    {
    public:
        //! Virtual destructor, because this is a polymorphic base class
        virtual ~Node() = default;
    };
}

#endif
