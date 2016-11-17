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

#ifndef OCTOPUS_SIGNAL_HPP
#define OCTOPUS_SIGNAL_HPP

#include <dsperados/math/utility.hpp>
#include <memory>
#include <stdexcept>
#include <vector>

#include "Clock.hpp"
#include "SignalBase.hpp"

namespace octo
{
    //! A discrete signal of _any_ given type.
    /*! This is the major base class of all signal processing. Everything is a signal and
        individual samples can be accessed using the subscript operator, like so:
     
        @code{.cpp}
        Sine sine;
        cout << sine[0] << sine[1] << endl;
     
        AudioFile file;
        auto x = file[2] * file[2];
        @endcode
     
        Besides accessing individual signal elements using the math-like [] syntax, one can
        manipulate signals as monolithic entities, reminiscent of mathematical signal theory
        formulas.
     
        @code{.cpp}
        // Vibrato
        sine.frequency = Sine(0.1) * 30 + 440;
     
        // A very simple low-pass filter
        auto filter = (x + Delay(x, 1)) * 0.5;
        @endcode */
    template <class T>
    class Signal : public SignalBase
    {
    public:
        //! Construct the signal
        Signal(Clock& clock) :
            clock(&clock),
            timestamp(clock.now())
        {
            
        }
        
        //! Virtual destructor, because this is a base class
        virtual ~Signal() = default;
        
        //! Retrieve a signal of the sample, relative to the its clock's current timestamp
        /*! @return A reference to the generated sample in cache. Copy and be done with it, this could change with each new [] call */
        const T& operator()()
        {
            const auto now = clock->now();
            
            if (timestamp <= now)
            {
                timestamp = now + 1;
                generateSample(cache);
            }
            
            return cache;
        }
        
        //! Return the current sample of the signal
        explicit operator T() { return (*this)(); }
        
        //! Move this signal to the heap
        /*! Signals need to implement this to support in-place creation of signals in expressions.
            @note This function can only be used on r-value signal objects. */
        virtual std::unique_ptr<Signal> moveToHeap() && = 0;
        
        //! Retrieve the clock this signal runs at
        Clock& getClock() const { return *clock; }
        
        // Inherited from SignalBase
        const std::type_info& getTypeInfo() const final override { return typeid(T); }
        const void* pull() final override { return &(*this)(); }
        
    private:
        //! Generate a new sample
        virtual void generateSample(T& out) = 0;
        
    private:
        //! The clock this signal runs at
        Clock* clock = nullptr;
        
        //! A cache for previously generated samples
        T cache = T{};
        
        //! The timestamp of the next-to-be generated sample
        uint64_t timestamp = 0;
    };
    
    // Convenience macro for overriding Signal::move()
    #define GENERATE_MOVE(CLASS) \
    auto moveToHeap() && -> std::unique_ptr<octo::Signal<typename std::decay<decltype(std::declval<CLASS>().operator()())>::type>> override \
    { \
        return std::make_unique<CLASS>(std::move(*this)); \
    }
}

#endif
