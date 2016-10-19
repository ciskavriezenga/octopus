//
//  Split.hpp
//  octopus
//
//  Created by Stijn on 17/10/2016.
//
//

#ifndef OCTOPUS_SPLIT_HPP
#define OCTOPUS_SPLIT_HPP

#include <vector>

#include "Group.hpp"
#include "Sieve.hpp"
#include "Value.hpp"

namespace octo
{
    //! Splits a multi-channel signal into multiple single-channel ones
    template <class T>
    class Split : public Group
    {
    public:
        //! Construct the split
        /*! size The amount of sieves in the split */
        Split(std::size_t size = 0)
        {
            resize(size);
        }
        
        //! Construct the split by providing size and input
        Split(std::size_t size, Value<std::vector<T>> input) :
            input(std::move(input))
        {
            resize(size);
        }
        
        //! Resize the number of sieves
        void resize(std::size_t size)
        {
            auto oldSize = sieves.size();
            sieves.resize(size);
            
            for (auto i = oldSize; i < size; ++i)
            {
                sieves[i].input = input;
                sieves[i].channel = static_cast<unsigned int>(i);
            }
        }
        
        //! Return one of the output sieves
        Sieve<T>& getOutput(std::size_t index) final override { return sieves.at(index); }
        
        //! Return the number of output sieves
        std::size_t getOutputCount() const final override { return sieves.size(); }
        
    public:
        //! The input to the split
        Value<std::vector<T>> input;
        
    private:
        //! The sieves that split the signal
        std::vector<Sieve<T>> sieves;
    };
}


#endif
