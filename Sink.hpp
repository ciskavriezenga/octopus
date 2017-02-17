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
#include <set>

namespace octo
{
    class Clock;
    
    //! Anything that needs updating according to a clock
    class Sink
    {
    public:
        class Listener;
        
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
        
    public:
        //! Listeners for changes to this sink
        std::set<Listener*> sinkListeners;
        
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
        virtual void clockChanged(const Clock* clock) { }
        
        //! The persistency changed
        virtual void persistencyChanged(bool persistent) { }
        
    private:
        //! Has the sink done its first update yet?
        bool started = false;
    };
    
    //! A listener for sink events
    class Sink::Listener
    {
    public:
        //! Virtual destructor, because this is a polymorphic class
        virtual ~Listener() = default;
        
        //! Let the listener know the clock of a sink changed
        virtual void clockChanged(const Clock* clock) { }
        
        //! Let the listener know the persistency of a sink changed
        virtual void persistencyChanged(bool persistent) { }
    };
}

#endif /* OCTOPUS_SINK_HPP */
