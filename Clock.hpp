//
//  Clock.hpp
//  octopus
//
//  Created by Stijn on 11/10/2016.
//
//

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
        
        //! Return the clocks current time index
        virtual unit::discrete<uint64_t> now() const = 0;
    };
    
    //! A clock that has to be manually moved forward
    class StepClock : public Clock
    {
    public:
        //! Set the sample rate of the clock
        void setRate(unit::hertz<float> rate) { _rate = rate; }
        
        //! Return the rate at which the clock runs
        unit::hertz<float> getRate() const override final { return _rate; }
        
        //! Move the clock to its next time index
        unit::discrete<uint64_t> tick() { return ++timestamp; }
        
        //! Return the clocks current time index
        unit::discrete<uint64_t> now() const override final { return timestamp; }
        
    private:
        //! The rate at which the clock runs
        unit::hertz<float> _rate = 44100;
        
        //! The current time index of the clock
        unit::discrete<uint64_t> timestamp = 0;
    };
}

#endif
