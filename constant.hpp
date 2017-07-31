//
//  constant.hpp
//  Octopus
//
//  Created by Stijn on 31/07/2017.
//
//

#ifndef OCTOPUS_CONSTANT_HPP
#define OCTOPUS_CONSTANT_HPP

#include <cached_signal.hpp>

namespace octo
{
    //! Outputs a constant value
    template <typename T>
    class Constant : public CachedSignal<T>
    {
    public:
        //! Construct the constant
        Constant(Clock* clock, const T& initialValue) :
            CachedSignal<T>(clock) :
            constant(initialValue)
        {
            
        }
        
    public:
        //! The constant value that will be outputted
        T constant;
        
    private:
        //! Generate a new cache value
        void generateSample(T& out) final
        {
            out = constant;
        }
    };
}

#endif /* OCTOPUS_CONSTANT_HPP */
