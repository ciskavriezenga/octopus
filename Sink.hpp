//
//  Sink.hpp
//  octopus
//
//  Created by Stijn Frishert on 08/02/17.
//
//

#ifndef OCTOPUS_SINK_HPP
#define OCTOPUS_SINK_HPP

#include <cstdint>

namespace octo
{
    class Clock;
    
    //! Anything that needs updating according to a clock
    class Sink
    {
    public:
        //! Construct the sink by specifying the clock to which it will listen
        Sink(Clock* clock);
        
        //! Virtual destructor, because this is a polymorphic base class
        virtual ~Sink() = default;
        
        //! Make sure the sink is up to date with the clock it was given
        void update();
        
        //! Change the clock
        void setClock(Clock* clock);
        
        //! Retrieve the clock this sink runs at
        Clock* getClock() const { return clock; }
        
        //! Make this sink persistent
        void setPersistency(bool persistent);
        
        //! Is this sink persistent?
        bool isPersistent() const;
        
    protected:
        //! Return the current rate of the clock
        float rate() const;
        
        //! Return the current delta of the clock
        float delta() const;
        
    protected:
        //! The clock this sink runs at
        Clock* clock = nullptr;
        
        //! The timestamp of the last update
        uint64_t timestamp = 0;
      
    private:
        //! Called when the sink needs updating according to the clock
        virtual void onUpdate() = 0;
        
        //! The clock changed
        virtual void clockChanged(Clock* clock) = 0;
        
    private:
        //! Has the sink done its first update yet?
        bool started = false;
    };
}

#endif /* OCTOPUS_SINK_HPP */
