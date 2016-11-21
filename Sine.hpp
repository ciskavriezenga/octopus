/*
 
 This file is a part of Octopus, a modern C++ library for embedding digital
 signal processing as a language inside your software. It transcends a single
 domain (audio, video, math, etc.), combining multiple clocks in one graph.
 
 Copyright (C) 2016 Dsperados <info@dsperados.com>
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>
 
 --------------------------------------------------------------------
 
 If you would like to use Octopus for commercial or closed-source
 purposes, please contact us for a commercial license.
 
 */

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
            out = std::sin(phase * TWO_PI<decltype(phase)> + phaseOffset());
            phase = math::wrap<long double>(phase + this->getClock().delta() * frequency(), 0, 1);
        }
        
    private:
        //! The current phase of the sine
        long double phase = 0;
    };
}


#endif
