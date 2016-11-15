//
//  CrossFade.hpp
//  octopus
//
//  Created by Stijn Frishert on 15/11/16.
//
//

#ifndef OCTOPUS_CROSS_FADE_HPP
#define OCTOPUS_CROSS_FADE_HPP

#include <dsperados/math/utility.hpp>
#include <dsperados/math/interpolation.hpp>

#include "BinaryOperation.hpp"

namespace octo
{
    template <class T>
    class CrossFade : public BinaryOperation<T>
    {
    public:
        CrossFade(octo::Clock& clock) :
            BinaryOperation<T>(clock),
            fade(clock)
        {
            
        }
        
        GENERATE_MOVE(CrossFade)
        
    public:
        Value<float> fade;
        
    private:
        void combineSamples(const T& left, const T& right, T& out) final override
        {
            const auto phase = math::clamp<float>(fade(), 0, 1);
            out = math::interpolateLinear(left, right, phase);
        }
    };
}

#endif
