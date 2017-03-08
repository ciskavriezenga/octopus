/*
 
 This file is a part of Octopus, a modern C++ library for embedding digital
 signal processing as a language inside your software. It transcends a single
 domain (audio, video, math, etc.), combining multiple clocks in one graph.
 
 Copyright (C) 2017 Dsperados <info@dsperados.com>
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>
 
 --------------------------------------------------------------------
 
 If you would like to use Octopus for commercial or closed-source
 purposes, please contact us for a commercial license.
 
 */

#ifndef OCTOPUS_SIEVE_HPP
#define OCTOPUS_SIEVE_HPP

#include <vector>

#include "unary_operation.hpp"

namespace octo
{
    //! Sifts out a single channel from a multi-channel signal
    /*! Signals with a vector type are the common idiom within Octopus for representing
        multi-channel signals. Sieves are used to filter out one their channels. If you'd
        like to filter out all channels at once, use a Split. */
    template <class T>
    class Sieve : public UnaryOperation<std::vector<T>, T>
    {
    public:
        //! Create the sieve by passing the channel
        Sieve(Clock* clock, unsigned int channel = 0, const T& initialCache = T{}) :
            UnaryOperation<std::vector<T>, T>(clock, initialCache),
            channel(channel)
        {
            
        }
        
        //! Create the sieve by passing the channel and input
        Sieve(Clock* clock, Value<std::vector<T>> input, unsigned int channel = 0) :
            UnaryOperation<std::vector<T>, T>(clock, std::move(input)),
            channel(channel)
        {
            
        }
        
        GENERATE_MOVE(Sieve)
        
    public:
        //! The channel being sifted out
        unsigned int channel = 0;
        
    private:
        //! Generated the sifted ouf signal
        void convertSample(const std::vector<T>& in, T& out) final override
        {
            out = channel < in.size() ? in[channel] : T{};
        }
    };
}

#endif
