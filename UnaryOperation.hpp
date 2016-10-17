//
//  UnaryOperation.hpp
//  octopus
//
//  Created by Stijn on 17/10/2016.
//
//

#ifndef OCTOPUS_UNARY_OPERATION_HPP
#define OCTOPUS_UNARY_OPERATION_HPP

#include "Signal.hpp"
#include "Value.hpp"

namespace octo
{
    //! Transform a single signal into another
    template <class Domain, class In, class Out = In>
    class UnaryOperation : public Signal<Domain, Out>
    {
    public:
        //! Construct an empty unary operation
        UnaryOperation() = default;
        
        //! Construct the unary operation with its input
        UnaryOperation(Value<Domain, In> input) :
            input(std::move(input))
        {
            
        }
        
    public:
        //! The input to the operation
        Value<Domain, In> input;
    };
}

#endif
