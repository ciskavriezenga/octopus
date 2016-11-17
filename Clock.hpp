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

#ifndef OCTOPUS_CLOCK_HPP
#define OCTOPUS_CLOCK_HPP

#include <chrono>
#include <cstdint>
#include <unit/discrete.hpp>
#include <unit/hertz.hpp>

namespace octo
{
    //! A clock used for keeping time with signals
    class Clock
    {
    public:
        //! Return the rate at which the clock runs
        virtual unit::hertz<float> getRate() const = 0;
        
        //! Return the delta between ticks
        virtual unit::period<float> delta() const { return getRate(); }
        
        //! Move the clock to its next time index
        virtual unit::discrete<uint64_t> tick() = 0;
        
        //! Return the clocks current time index
        virtual unit::discrete<uint64_t> now() const = 0;
    };
    
    //! A clock with a variable sample rate
    class VariableClock : public Clock
    {
    public:
        //! Construct the clock
        /*! @param startingRate The rate at which the clocks starts running (will be influenced by subsequent ticks */
        VariableClock(unit::hertz<float> startingRate) :
            rate(startingRate)
        {
            lastNow = std::chrono::high_resolution_clock::now();
        }
        
        //! Return the rate at which the clock runs
        unit::hertz<float> getRate() const final override { return rate; }
        
        //! Move the clock to its next time index
        unit::discrete<uint64_t> tick() final override
        {
            auto now = std::chrono::high_resolution_clock::now();
            rate = 1.0 / std::chrono::duration_cast<std::chrono::duration<double>>(lastNow - now).count();
            lastNow = now;
            
            return ++timestamp;
        }
        
        //! Return the clocks current time index
        unit::discrete<uint64_t> now() const final override { return timestamp; }
        
    private:
        //! The rate at which the clock currently runs
        unit::hertz<float> rate = 0;
        
        //! The current time index of the clock
        unit::discrete<uint64_t> timestamp = 0;
        
        //! The time at the previous tick() call
        std::chrono::high_resolution_clock::time_point lastNow;
    };
    
    //! A clock with an invariable sample rate
    class InvariableClock : public Clock
    {
    public:
        //! Construct the clock
        /*! @param rate The rate at which the clocks runs (changes only with setRate) */
        InvariableClock(unit::hertz<float> rate) :
            rate(rate)
        {
            
        }
        
        //! Set the sample rate of the clock
        void setRate(unit::hertz<float> rate) { this->rate = rate; }
        
        //! Return the rate at which the clock runs
        unit::hertz<float> getRate() const final override { return rate; }
        
        //! Move the clock to its next time index
        unit::discrete<uint64_t> tick() final override { return ++timestamp; }
        
        //! Return the clocks current time index
        unit::discrete<uint64_t> now() const final override { return timestamp; }
        
    private:
        //! The rate at which the clock runs
        unit::hertz<float> rate = 0;
        
        //! The current time index of the clock
        unit::discrete<uint64_t> timestamp = 0;
    };
}

#endif
