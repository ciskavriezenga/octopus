#ifndef OCTOPUS_CLOCK_HPP
#define OCTOPUS_CLOCK_HPP

#include <cstdint>
#include <unit/discrete.hpp>
#include <unit/hertz.hpp>

namespace octo
{
    class Clock
    {
    public:
        //! Construct a clock at a given rate
        Clock(unit::hertz<float> rate) : rate_(rate) { }
        
        //! Move to the next timestamp
        unit::discrete<uint64_t> tick() { return ++now_; }
        
        //! Return the current timestamp of the clock
        unit::discrete<uint64_t> now() const { return now_; }
        
        //! Return the rate at which the clock runs
        unit::hertz<float> rate() const { return rate_; }
        
    private:
        //! The current timestamp of the clock
        unit::discrete<uint64_t> now_ = 0;
        
        //! The rate at which the clock runs
        unit::hertz<float> rate_ = 0;
    };
}

#endif
