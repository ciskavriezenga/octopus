/*
 
 This file is a part of Octopus, a modern C++ library for embedding digital
 signal processing as a language inside your software. It transcends a single
 domain (audio, video, math, etc.), combining multiple clocks in one graph.
 
 Copyright (C) 2017 Dsperados <info@dsperados.com>
 
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
#include <mutex>
#include <set>

#include "sink.hpp"

namespace octo
{
    //! Base class VariableClock and InvariableClock
    class Clock
    {
    public:
        //! Virtual destructor, because this is a base class
        virtual ~Clock() = default;
        
        //! Return the rate at which the clock runs (in Hertz)
        virtual float getRate() const = 0;
        
        //! Return the delta between ticks (in seconds)
        virtual float getDelta() const { return 1.0f / getRate(); }
        
        //! Move the clock forward to its next frame
        /*! Ticking a clock means that all sinks/signals that are set to
            follow this clock will generate a new sample when pulled. */
        uint64_t tick();
        
        //! Return the clocks current time index
        virtual uint64_t getNow() const = 0;
        
        //! Add a signal as persistent
        void addPersistentSink(Sink& sink);
        
        //! Remove a signal as persistent
        void removePersistentSink(Sink& sink);
        
        //! Is a signal persistent for this clock?
        bool isSinkPersistent(const Sink& sink) const;
        
    public:
        //! Called when the clock was ticked
        std::function<void()> onTick;
        
    private:
        //! Move the clock to its next time index
        virtual void ticked() = 0;
        
    private:
        //! The sinks that will be updated with each tick
        std::set<Sink*> persistentSinks;
        
        //! The mutex for locking around changes to the persistent sinks
        std::mutex persistencyMutex;
    };
    
    //! A clock with an invariable, constant rate
    /*! Clocks are used for keeping time with signals. Each signal compares its internal state
     with the clock it was given. If it's not up to date, new sample data will be generated.
     
     Invariable clocks are the most simple clocks available. You set their rate once, and
     signals with an invariable clock attached can then request the rate whenever they need it.
     A good example of a domain with an invariable clock would be audio. Although audio samples
     are often not actuallt generated every 1/44100 second (they are generated in 'bursts' by the
     audio callback), audio signals should operate as if the rate between every sample remains constant.
     That is how they will be *played back* after all. */
    class InvariableClock : public Clock
    {
    public:
        //! Construct the clock
        /*! @param rateInHertz The rate at which the clocks runs (changes only with setRate). */
        InvariableClock(float rateInHertz);
        
        //! Set the sample rate of the clock (in Hertz)
        void setRate(float rateInHertz) { rate_ = rateInHertz; }
        
        //! Return the rate at which the clock runs (in Hertz)
        float getRate() const final { return rate_; }
        
        //! Return the clocks current time index
        uint64_t getNow() const final { return timestamp; }
        
    private:
        //! Move the clock to its next time index
        void ticked() final { ++timestamp; }
        
    private:
        //! The rate at which the clock runs
        float rate_ = 0;
        
        //! The current time index of the clock
        uint64_t timestamp = 0;
    };
    
    //! A clock with a variable sample rate
    /*! Clocks are used for keeping time with signals. Each signal compares its internal state
     with the clock it was given. If it's not up to date, new sample data will be generated.
     
     Variable clocks automatically change their rate every time tick() is called, depending
     on the elapsed time since the previous tick. A good example of a domain with a variable
     clock would be video. The FPS (rate) in video applications changes every frame (some ticks
     take longer than others). Signals attached to variable clocks can request the rate or
     delta every frame anew and make sure they update themselves according to how much time the
     last tick took. */
    class VariableClock : public Clock
    {
    public:
        //! Construct the clock
        /*! @param startingRateInHertz The rate at which the clocks starts running (will be influenced by subsequent ticks) */
        VariableClock(float maximalRate);
        
        //! Return the rate at which the clock runs (in Hertz)
        float getRate() const final { return rate_; }
        
        //! Set the maximal sample rate of the clock (in Hertz)
        void setMaximalRate(float rateInHertz) { maximalRate = rateInHertz; }
        
        //! Return the maximal rate at which the clock can run (in Hertz)
        float getMaximalRate() const { return maximalRate; }
        
        //! Return the clocks current time index
        uint64_t getNow() const final { return timestamp; }
        
    private:
        //! Move the clock to its next time index
        void ticked() final;
        
    private:
        //! The maximal rate at which the clock should run
        float maximalRate = 0;
        
        //! The rate at which the clock currently runs
        float rate_ = 0;
        
        //! The current time index of the clock
        uint64_t timestamp = 0;
        
        //! The time at the previous tick() call
        std::chrono::high_resolution_clock::time_point lastNow;
    };
}

#endif
