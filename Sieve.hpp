//
//  Sieve.hpp
//  octopus
//
//  Created by Stijn on 17/10/2016.
//
//

#ifndef OCTOPUS_SIEVE_HPP
#define OCTOPUS_SIEVE_HPP

#include <vector>

#include "UnaryOperation.hpp"

namespace octo
{
    //! Sifts out a single channel from a multi-channel signal
    template <class T>
    class Sieve : public UnaryOperation<std::vector<T>, T>
    {
    public:
        //! Create the sieve by passing the channel
        Sieve(Clock& clock, unsigned int channel = 0) :
            UnaryOperation<std::vector<T>, T>(clock),
            channel(channel)
        {
            
        }
        
        //! Create the sieve by passing the channel and input
        Sieve(Clock& clock, Value<std::vector<T>> input, unsigned int channel = 0) :
            UnaryOperation<std::vector<T>, T>(clock, std::move(input)),
            channel(channel)
        {
            
        }
        
        //! Create the sive by passin the channel and input
        Sieve(Value<std::vector<T>> input, unsigned int channel = 0) :
            Sieve(input.getClock(), std::move(input), channel)
        {
            
        }
        
        GENERATE_MOVE(Sieve)
        
    public:
        //! The channel being sifted out
        unsigned int channel = 0;
        
    private:
        //! Generated the sifted ouf signal
        void generateSample(T& out) const final override
        {
            const auto& x = this->input[0];
            out = channel < x.size() ? x[channel] : T{};
        }
    };
}

#endif
