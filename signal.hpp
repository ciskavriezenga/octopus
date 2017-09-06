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

#ifndef OCTOPUS_SIGNAL_HPP
#define OCTOPUS_SIGNAL_HPP

#include <memory>
#include <stdexcept>
#include <vector>

#include "clock.hpp"
#include "signal_base.hpp"

namespace octo
{
    //! A discrete signal of _any_ given type.
    /*! This is the major class of all signal processing. Everything in octopus is a signal
        derivative. When constructing a signal, it asks for a Clock (see Clock.hpp for more
        information), which it will use for time keeping.
     
        @code{.cpp}
        InvariableClock clock;
        Sine<float> sine(clock);
        @endcode
     
        Requesting samples from a signal can be done by calling the signal. This will return
        the same value until the clock has been moved forward with tick(). Therefore, walking
        through a signal corresponds to repeatedly requesting a sample and moving the clock further.
     
        @code{cpp}
        cout << sine() << endl;
        clock.tick();
        cout << sine() << endl;
        @endcode
     
        Besides accessing individual signal elements using call-syntax, one can manipulate signals
        as monolithic entities, reminiscent of mathematical signal theory.
        formulas.
     
        @code{.cpp}
        // Vibrato
        sine.frequency = Sine<float>(clock, 0.1) * 30 + 440;
        @endcode */
    template <class T>
    class Signal : public SignalBase
    {
    public:
        using SignalBase::SignalBase;
        
        //! Virtual destructor, because this is a base class
        virtual ~Signal() = default;
        
        //! Retrieve a signal of the sample, relative to the its clock's current timestamp
        /*! @return A reference to the generated sample in cache. Copy and be done with it, this could change with each call */
        const T& operator()()
        {
            update(); // Update the signal as a sink
            return getOutput();
        }
        
        //! Retrieve a signal of the sample, relative to the its clock's current timestamp
        /*! @return A reference to the generated sample in cache. Copy and be done with it, this could change with each call */
        const T& pull() { return operator()(); }
        
        //! Return the current sample of the signal
        explicit operator T() { return (*this)(); }
        
        // Inherited from SignalBase
        const std::type_info& getTypeInfo() const final { return typeid(T); }
        const void* pullGeneric() final { return &(*this)(); }
        
        //! The type for template meta programming
        using type = T;
        
    private:
        //! Return the output value
        virtual const T& getOutput() = 0;
        
    private:
        //! A cache for previously generated samples
        T cache = T{};
    };
}

#endif
