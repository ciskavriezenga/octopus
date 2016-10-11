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
    template <class Domain, class T>
    class Sine : public Signal<Domain, T>
    {
    public:
        //! Construct an empty sum
        Sine() = default;
        
        Sine(Value<Domain, float> frequency, Value<Domain, float> phase = 0) :
            frequency(std::move(frequency)),
            phase(std::move(phase))
        {
            
        }
        
        GENERATE_MOVE(Sine)
        
    public:
        //! The frequency of the sine
        Value<Domain, float> frequency;
        
        //! The phase offset of the sine
        Value<Domain, float> phase;
        
    private:
        void generateSample(T& out) override final
        {
            out = std::sin(theta * 6.28318530718 + phase[0]);
            theta += 1.0 / Clock<Domain>::getRate() * frequency[0];
        }
        
    private:
        long double theta = 0;
    };
}


#endif
