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
        Sieve(unsigned int channel = 0) :
            channel(channel)
        {
            
        }
        
        //! Create the sieve by passing the channel and input
        Sieve(unsigned int channel, Value<std::vector<T>> input) :
            UnaryOperation<std::vector<T>, T>(std::move(input)),
            channel(channel)
        {
            
        }
        
        GENERATE_MOVE(Sieve)
        
    public:
        //! The channel being sifted out
        unsigned int channel = 0;
        
    private:
        //! Generated the sifted ouf signal
        void generateSample(T& out) final override
        {
            const auto& x = this->input[0];
            out = channel < x.size() ? x[channel] : T{};
        }
    };
}

#endif
