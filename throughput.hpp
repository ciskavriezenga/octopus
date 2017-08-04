//
//  throughput.hpp
//  Octopus
//
//  Created by Stijn on 04/08/2017.
//
//

#ifndef OCTOPUS_THROUGHPUT_HPP
#define OCTOPUS_THROUGHPUT_HPP

#include "unary_operation.hpp"

namespace octo
{
    //! Signal that simply sends through its input
    template <typename T>
    class Throughput : public UnaryOperation<T>
    {
    public:
        using UnaryOperation<T>::UnaryOperation;
        
    private:
        void convertSample(const T& in, T& out) final
        {
            out = in;
        }
    };
}

#endif /* OCTOPUS_THROUGHPUT_HPP */
