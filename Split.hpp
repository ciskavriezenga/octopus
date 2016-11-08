//
//  Split.hpp
//  octopus
//
//  Created by Stijn on 17/10/2016.
//
//

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
