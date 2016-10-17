//
//  Latest.hpp
//  octopus
//
//  Created by Stijn on 11/10/2016.
//
//

#ifndef OCTOPUS_LATEST_HPP
#define OCTOPUS_LATEST_HPP

#include "Signal.hpp"
#include "Value.hpp"

namespace octo
{
    //! Converts between two different domains
    template <class FromDomain, class ToDomain, class T>
    class Latest : public Signal<ToDomain, T>
    {
    public:
        //! Construct the converter
        Latest() = default;
        
        //! Construct the converter by passing in an input
        Latest(Value<FromDomain, T> input) : input(std::move(input)) { }
        
        // Generate the moveToHeap() function
        GENERATE_MOVE(Latest)
        
    public:
        //! The input of the conversion
        Value<FromDomain, T> input;
        
    private:
        //! Generate a sample in the destination domain by retrieving the latest from the source domain
        void generateSample(T& out) final override { out = input[0]; }
    };
}

#endif
