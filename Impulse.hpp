//
//  Impulse.hpp
//  octopus
//
//  Created by Stijn Frishert on 08/11/16.
//
//

#ifndef OCTOPUS_IMPULSE_HPP
#define OCTOPUS_IMPULSE_HPP

#include "Signal.hpp"

namespace octo
{
    //! Generates a Dirac impulse signal
    template <class T>
    class Impulse : public Signal<T>
    {
    public:
        //! Construct and pass the clock
        Impulse(Clock& clock) :
            Signal<T>(clock)
        {
            
        }
        
        //! Reset, make the signal output another pulse
        void reset() { triggered = false; }
        
        // Inherited from Signal
        GENERATE_MOVE(Impulse)
        
    private:
        //! Generate a new sample
        void generateSample(T& out) final override
        {
            out = triggered ? 0 : 1;
            triggered = true;
        }
        
    private:
        //! Has the value 1 impulse already been fired?
        bool triggered = false;
    };
}

#endif
