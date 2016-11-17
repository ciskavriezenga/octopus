/*
 
 This file is a part of Octopus, a modern C++ library for embedding digital
 signal processing as a language inside your software. It transcends a single
 domain (audio, video, math, etc.), combining multiple clocks in one graph.
 
 Copyright (C) 2016 Dsperados <info@dsperados.com>
 
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

#ifndef OCTOPUS_SPLIT_HPP
#define OCTOPUS_SPLIT_HPP

#include <memory>
#include <vector>

#include "Group.hpp"
#include "Sieve.hpp"
#include "Value.hpp"

namespace octo
{
    class Clock;
    
    //! Splits a multi-channel signal into multiple single-channel ones
    template <class T>
    class Split : public Group
    {
    public:
        //! Construct the split
        /*! size The amount of sieves in the split */
        Split(Clock& clock, std::size_t size = 0) :
            input(clock)
        {
            resize(size);
        }
        
        //! Construct the split by providing size and input
        Split(Clock& clock, Value<std::vector<T>> input, std::size_t size) :
            input(clock, std::move(input))
        {
            resize(size);
        }
        
        //! Construct the split by providing size and input
        Split(Value<std::vector<T>> input, std::size_t size) :
            Split(input.getClock(), std::move(input), size)
        {
            
        }
        
        //! Resize the number of sieves
        void resize(std::size_t size)
        {
            // Store the current size for use during down-/upsizing
            const auto oldSize = getOutputCount();
            
            // Remove sieves if we're downsizing
            for (auto i = size; i < oldSize; ++i)
                removeOutput("channel" + std::to_string(i));
            
            sieves.resize(size);
            
            // Add sieves if we're upsizing
            for (auto i = oldSize; i < size; ++i)
            {
                sieves[i] = std::make_unique<Sieve<T>>(input, i);
                addOutput("channel" + std::to_string(i), *sieves[i]);
            }
        }
        
    public:
        //! The input to the split
        Value<std::vector<T>> input;
        
        //! The sieves that make up this split
        std::vector<std::unique_ptr<Sieve<T>>> sieves;
    };
}


#endif
