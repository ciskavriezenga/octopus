//
//  Sink.cpp
//  octopus
//
//  Created by Stijn Frishert on 08/02/17.
//
//

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
        
        bool persistent = isPersistent();
        if (persistent && this->clock)
            this->clock->removePersistentSink(*this);
        
        this->clock = clock;
        if (clock)
        {
            timestamp = clock->now();
        
            if (persistent)
                this->clock->addPersistentSink(*this);
        }
        
        clockChanged(clock);
    }
    
    void Sink::setPersistency(bool persistent)
    {
        if (!clock)
            return;
        
        if (persistent)
            clock->addPersistentSink(*this);
        else
            clock->removePersistentSink(*this);
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
