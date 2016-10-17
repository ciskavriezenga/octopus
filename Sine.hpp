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

#include "Signal.hpp"
#include "Value.hpp"

namespace octo
{
    //! Generates a sine wave with a given frequency and phase offset
    template <class Domain, class T>
    class Sine : public Signal<Domain, T>
    {
    public:
        //! Construct a sum with a frequency of 0 and phase offset of 0
        Sine() = default;
        
        //! Construct
        Sine(Value<Domain, float> frequency, Value<Domain, float> phaseOffset = 0) :
            frequency(std::move(frequency)),
            phaseOffset(std::move(phaseOffset))
        {
            
        }
        
        GENERATE_MOVE(Sine)
        
    public:
        //! The frequency of the sine
        Value<Domain, float> frequency;
        
        //! The phase offset of the sine
        Value<Domain, float> phaseOffset;
        
    private:
        //! Generate the next sample of the sine
        void generateSample(T& out) final override
        {
            out = std::sin(phase * 6.28318530718 + phaseOffset[0]);
            phase += 1.0 / Domain::clock.getRate() * frequency[0];
        }
        
    private:
        //! The current phase of the sine
        long double phase = 0;
    };
}


#endif
