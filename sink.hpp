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
        virtual ~Sink();
        
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
        float getRate() const;
        
        //! Return the current delta of the clock
        float getDelta() const;
        
    protected:
        //! The clock this sink runs at
        Clock* clock = nullptr;
        
        //! The timestamp of the last update
        uint64_t timestamp = 0;
      
    private:
        //! Called when the sink needs updating according to the clock
        virtual void onUpdate() = 0;
        
        //! The clock changed
        virtual void clockChanged(Clock* clock) { }
        
        //! The persistency changed
        virtual void persistencyChanged(bool persistent) { }
        
    private:
        //! Has the sink done its first update yet?
        bool started = false;
    };
    
    //! A listener for sink events
    class Sink::Listener
    {
        friend class Sink;
        
    public:
        //! Virtual destructor, because this is a polymorphic class
        virtual ~Listener() = default;
        
    protected:
        //! Let the listener know the clock of a sink changed
        virtual void clockChanged(Clock* clock) { }
        
        //! Let the listener know the persistency of a sink changed
        virtual void persistencyChanged(bool persistent) { }
    };
}

#endif /* OCTOPUS_SINK_HPP */
