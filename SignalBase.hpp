#ifndef OCTOPUS_SIGNAL_BASE_HPP
#define OCTOPUS_SIGNAL_BASE_HPP

#include <cstdint>
#include <unit/discrete.hpp>

#include "Node.hpp"

namespace octo
{
    //! Base class for all signals in Octopus, regardless of their output type
    class SignalBase : public Node
    {
    public:
        //! Virtual destructor, because this is a polymorphic base class
        virtual ~SignalBase() = default;
    };
}

#endif
