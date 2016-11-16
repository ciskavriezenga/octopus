//
//  CrossFade.hpp
//  octopus
//
//  Created by Stijn Frishert on 15/11/16.
//
//

#ifndef OCTOPUS_CROSS_FADE_HPP
#define OCTOPUS_CROSS_FADE_HPP

#include <dsperados/math/ease.hpp>
#include <dsperados/math/utility.hpp>
#include <functional>

#include "BinaryOperation.hpp"

namespace octo
{
    //! Cross fade between two signals
    template <class T>
    class CrossFade : public BinaryOperation<T>
    {
    public:
        //! Construct the cross fade
        CrossFade(octo::Clock& clock) :
            BinaryOperation<T>(clock),
            fade(clock)
        {

        }
        
        GENERATE_MOVE(CrossFade)
        
    public:
        //! The fading value (0 = left, 1 = right)
        Value<float> fade;
        
        //! The function that will be used for easing
        std::function<float(float)> ease = [](float v){ return math::easeCosine(v); };
        
    private:
        // Inherited from BinaryOperation
        void combineSamples(const T& left, const T& right, T& out) final override
        {
            if (!ease)
            {
                out = T{};
            } else {
                const auto phase = math::clamp<float>(fade(), 0, 1);
                out = ease(1 - phase) * left + ease(phase) * right;
            }
        }
    };
}

#endif
