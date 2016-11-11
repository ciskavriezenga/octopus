//
//  Negation.hpp
//  octopus
//
//  Created by Stijn on 17/10/2016.
//
//

#ifndef OCTOPUS_NEGATION_HPP
#define OCTOPUS_NEGATION_HPP

#include "UnaryOperation.hpp"

namespace octo
{
    //! Negate a signal
    template <class T>
    class Negation : public UnaryOperation<T>
    {
    public:
        // Use the constructor from UnaryOperation
        using UnaryOperation<T>::UnaryOperation;
        
        // Generate the moveToHeap() function
        GENERATE_MOVE(Negation)
        
    private:
        //! Generate a negative sample
        void generateSample(T& out) final override
        {
            out = -this->input();
        }
    };
}

#endif
