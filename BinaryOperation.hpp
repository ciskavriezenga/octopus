//
//  BinaryOperation.hpp
//  octopus
//
//  Created by Stijn on 17/10/2016.
//
//

#ifndef OCTOPUS_BINARY_OPERATION_HPP
#define OCTOPUS_BINARY_OPERATION_HPP

#include "Signal.hpp"
#include "Value.hpp"

namespace octo
{
    //! Combine two signals into one
    template <class Domain, class T>
    class BinaryOperation : public Signal<Domain, T>
    {
    public:
        //! Construct an empty binary operation
        BinaryOperation() = default;
        
        //! Construct the binary operation with two terms
        BinaryOperation(Value<Domain, T> lhs, Value<Domain, T> rhs) :
            lhs(std::move(lhs)),
            rhs(std::move(rhs))
        {
            
        }
        
    public:
        //! The left-hand side of the operation
        Value<Domain, T> lhs;
        
        //! The left-hand side of the operation
        Value<Domain, T> rhs;
        
    private:
        //! Generate a new sample
        void generateSample(T& out) override final
        {
            combineSamples(lhs[0], rhs[0], out);
        }
        
        //! Combine two samples into a new one
        virtual void combineSamples(const T& lhs, const T& rhs, T& out) = 0;
    };
}

#endif
