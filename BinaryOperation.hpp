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
            left(clock),
            right(clock)
        {
            
        }
        
        //! Construct the binary operation with two terms and its own clock
        BinaryOperation(Clock& clock, Value<T> left, Value<T> right) :
            Signal<T>(clock),
            left(std::move(left)),
            right(std::move(right))
        {
            
        }
        
        //! Construct a binary operation with two terms
        BinaryOperation(Value<T> left, Value<T> right) :
            BinaryOperation(left.getClock(), std::move(left), std::move(right))
        {
            if (&left.getClock() != &right.getClock())
                throw std::runtime_error("two clocks of binary operation don't match");
        }
        
    public:
        //! The left-hand side of the operation
        Value<T> left;
        
        //! The left-hand side of the operation
        Value<T> right;
        
    private:
        //! Generate a new sample
        void generateSample(T& out) final override
        {
            combineSamples(left[0], right[0], out);
        }
        
        //! Combine two samples into a new one
        virtual void combineSamples(const T& left, const T& right, T& out) = 0;
    };
}

#endif
