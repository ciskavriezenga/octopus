//
//  Group.hpp
//  octopus
//
//  Created by Stijn on 17/10/2016.
//
//

#ifndef OCTOPUS_GROUP_HPP
#define OCTOPUS_GROUP_HPP

#include <cstdint>

#include "Node.hpp"
#include "SignalBase.hpp"
#include "Signal.hpp"

namespace octo
{
    //! Multiple signals in one monolith object
    class Group : public Node
    {
    public:
        //! Return one of the outputs of the group
        virtual SignalBase& getOutput(std::size_t index) = 0;
        
        //! Return one of the outputs a a templated signal
        /*! Throws if the output is not of this signal type */
        template <class Domain, class T>
        auto& getOutput(std::size_t index) { return dynamic_cast<Signal<Domain, T>&>(getOutput(index)); }
        
        //! Return the number of outputs in the group
        virtual std::size_t getOutputCount() const = 0;
    };
}

#endif
