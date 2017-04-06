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

#include <stdexcept>

#include "clock.hpp"
#include "sink.hpp"

namespace octo
{
    Sink::Sink(Clock* clock) :
        clock(clock)
    {
        if (clock)
            timestamp = clock->now();
    }

    Sink::~Sink()
    {
        if (isPersistent())
            setPersistency(false);
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
        
        const auto listeners = sinkListeners;
        
        // If we've moved to a new clock (instead of no clock at all), set some data
        if (clock)
        {
            timestamp = clock->now();
            
            if (persistent)
                this->clock->addPersistentSink(*this);
        } else if (persistent) {
            // If we didn't move to a new clock, and lost persistency, let derivatives and listeners know
            persistencyChanged(false);
            for (auto& listener : listeners)
                listener->persistencyChanged(false);
        }
    
        // Let derivatives and listeners know we moved to a new clock
        clockChanged(clock);
        for (auto& listener : listeners)
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
        const auto listeners = sinkListeners;
        for (auto& listener : listeners)
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
