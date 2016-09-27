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
    //! Sums an variadic amount of values into one
    template <class T>
    class Sine : public Signal<T>
    {
    public:
        //! Construct an empty sum
        Sine(Clock& clock) :
            Signal<T>(clock),
            frequency(clock),
            phase(clock)
        {
            
        }
        
        template <class U, class V>
        Sine(Clock& clock, Value<float> frequency, Value<float> phase) :
            Signal<T>(clock),
            frequency(std::move(frequency)),
            phase(std::move(phase))
        {
            
        }
        
        GENERATE_MOVE(Sine)
        
    public:
        //! The frequency of the sine
        Value<float> frequency;
        
        //! The phase offset of the sine
        Value<float> phase;
        
    private:
        void generateSample(T& out) override final
        {
            out = std::sin(theta * 6.28318530718 + phase[0]);
            theta += 1.0 / this->getClock().rate() * frequency[0];
        }
        
    private:
        long double theta = 0;
    };
}


#endif
