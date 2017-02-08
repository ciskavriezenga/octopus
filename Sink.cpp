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
    Sink::Sink(Clock& clock) :
        clock(&clock),
        timestamp(clock.now())
    {
        
    }
    
    void Sink::update()
    {
        const auto now = clock->now();
        
        // Do we need updating?
        if (timestamp >= now && started)
            return;
        
        started = true;
        timestamp = now;
        onUpdate();
    }
    
    void Sink::setClock(Clock& clock)
    {
        this->clock = &clock;
        timestamp = clock.now();
        
        clockChanged(clock);
    }
    
    void Sink::setPersistency(bool persistent)
    {
        if (persistent)
            clock->addPersistentSink(*this);
        else
            clock->removePersistentSink(*this);
    }
    
    bool Sink::isPersistent() const
    {
        return clock->isSinkPersistent(*this);
    }
}
