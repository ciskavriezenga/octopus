//
//  Clock.hpp
//  octopus
//
//  Created by Stijn on 11/10/2016.
//
//

#ifndef OCTOPUS_CLOCK_HPP
#define OCTOPUS_CLOCK_HPP

#include <cstdint>
#include <unit/discrete.hpp>
#include <unit/hertz.hpp>

namespace octo
{
    template <class Domain>
    class Clock
    {
    public:
        static void setRate(unit::hertz<float> rate) { Clock::rate = rate; }
        static unit::hertz<float> getRate() { return rate; }
        
        static unit::discrete<uint64_t> tick() { return ++timestamp; }
        static unit::discrete<uint64_t> now() { return timestamp; }
        
    private:
        static unit::hertz<float> rate;
        static unit::discrete<uint64_t> timestamp;
    };
    
    template <class Domain>
    unit::hertz<float> Clock<Domain>::rate = 44100;
    
    template <class Domain>
    unit::discrete<uint64_t> Clock<Domain>::timestamp = 0;
}

#endif
