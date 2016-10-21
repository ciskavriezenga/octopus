//
//  BinaryOperation.hpp
//  octopus
//
//  Created by Stijn on 17/10/2016.
//
//

#ifndef OCTOPUS_BINARY_OPERATION_HPP
#define OCTOPUS_BINARY_OPERATION_HPP

#include <stdexcept>

#include "Signal.hpp"
#include "Value.hpp"

namespace octo
{
    //! Combine two signals into one
    template <class T>
    class BinaryOperation : public Signal<T>
    {
    public:
        //! Construct an empty binary operation
        BinaryOperation(Clock& clock) :
            Signal<T>(clock),
            lhs(clock),
            rhs(clock)
        {
            
        }
        
        //! Construct the binary operation with two terms and its own clock
        BinaryOperation(Clock& clock, Value<T> lhs, Value<T> rhs) :
            Signal<T>(clock),
            lhs(std::move(lhs)),
            rhs(std::move(rhs))
        {
            
        }
        
        //! Construct a binary operation with two terms
        BinaryOperation(Value<T> lhs, Value<T> rhs) :
            BinaryOperation(lhs.getClock(), std::move(lhs), std::move(rhs))
        {
            if (&lhs.getClock() != &rhs.getClock())
                throw std::runtime_error("two clocks of binary operation don't match");
        }
        
    public:
        //! The left-hand side of the operation
        Value<T> lhs;
        
        //! The left-hand side of the operation
        Value<T> rhs;
        
    private:
        //! Generate a new sample
        void generateSample(T& out) final override
        {
            combineSamples(lhs[0], rhs[0], out);
        }
        
        //! Combine two samples into a new one
        virtual void combineSamples(const T& lhs, const T& rhs, T& out) = 0;
    };
}

#endif
