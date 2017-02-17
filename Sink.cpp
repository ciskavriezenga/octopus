//
//  Sink.cpp
//  octopus
//
//  Created by Stijn Frishert on 08/02/17.
//
//

#include <stdexcept>

#include "Clock.hpp"
#include "Sink.hpp"

namespace octo
{
    Sink::Sink(Clock* clock) :
        clock(clock)
    {
        if (clock)
            timestamp = clock->now();
    }
    
    void Sink::update()
    {
        if (!clock)
            return onUpdate();
        
        const auto now = clock->now();
        
        // Do we need updating?
        if (timestamp >= now && started)
            return;
        
        started = true;
        timestamp = now;
        onUpdate();
    }
    
    void Sink::setClock(Clock* clock)
    {
        if (clock == this->clock)
            return;
        
        // If we're persistent with the current clock, make the sink non-persistent
        bool persistent = isPersistent();
        if (persistent && this->clock)
            this->clock->removePersistentSink(*this);
        
        // Change the clock
        this->clock = clock;
        
        // If we've moved to a new clock (instead of no clock at all), set some data
        if (clock)
        {
            timestamp = clock->now();
            
            if (persistent)
                this->clock->addPersistentSink(*this);
        } else if (persistent) {
            // If we didn't move to a new clock, and lost persistency, let derivatives and listeners know
            persistencyChanged(false);
            for (auto& listener : sinkListeners)
                listener->persistencyChanged(false);
        }
        
        // Let derivatives and listeners know we moved to a new clock
        clockChanged(clock);
        for (auto& listener : sinkListeners)
            listener->clockChanged(clock);
    }
    
    void Sink::setPersistency(bool persistent)
    {
        if (!clock)
            throw std::runtime_error("sink without clocks can't have their persistency set");
        
        if (persistent == isPersistent())
            return;
        
        if (persistent)
            clock->addPersistentSink(*this);
        else
            clock->removePersistentSink(*this);
        
        // Let derivatives and listeners know
        persistencyChanged(persistent);
        for (auto& listener : sinkListeners)
            listener->persistencyChanged(persistent);
    }
    
    bool Sink::isPersistent() const
    {
        return clock ? clock->isSinkPersistent(*this) : false;
    }
    
    float Sink::rate() const
    {
        return clock ? clock->rate() : 0;
    }
    
    float Sink::delta() const
    {
        return clock ? clock->delta() : 0;
    }
}
