//
//  Sine.hpp
//  octopus
//
//  Created by Stijn on 27/09/2016.
//
//

#ifndef OCTOPUS_SINE_HPP
#define OCTOPUS_SINE_HPP

#include <cmath>
#include <dsperados/math/utility.hpp>

#include "Signal.hpp"
#include "Value.hpp"

#include <iostream>

namespace octo
{
    //! Generates a sine wave with a given frequency and phase offset
    template <class T>
    class Sine : public Signal<T>
    {
    public:
        //! Construct a sum with a frequency of 0 and phase offset of 0
        Sine(Clock& clock) :
            Signal<T>(clock),
            frequency(clock),
            phaseOffset(clock)
        {
            
        }
        
        //! Construct by passing a frequency
        Sine(Clock& clock, float frequency, float phaseOffset = 0.0f) :
            Signal<T>(clock),
            frequency(clock, frequency),
            phaseOffset(clock, phaseOffset)
        {
            
        }
        
        
        //! Construct
        Sine(Clock& clock, Value<float> frequency, Value<float> phaseOffset) :
            Signal<T>(clock),
            frequency(std::move(frequency)),
            phaseOffset(std::move(phaseOffset))
        {
            
        }
        
        GENERATE_MOVE(Sine)
        
    public:
        //! The frequency of the sine
        Value<float> frequency;
        
        //! The phase offset of the sine
        Value<float> phaseOffset;
        
    private:
        //! Generate the next sample of the sine
        void generateSample(T& out) final override
        {
            out = std::sin(phase * 6.28318530718 + phaseOffset[0]);
            phase = math::wrap(phase + this->getClock().getDeltaPeriod() * frequency[0], 0, 1);
        }
        
    private:
        //! The current phase of the sine
        mutable long double phase = 0;
    };
}


#endif
