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

#include <thread>

#include "clock.hpp"

using namespace std::chrono;

namespace octo
{
	uint64_t Clock::tick()
    {
        ticked();
        
        {
            std::unique_lock<std::mutex> lock(persistencyMutex);
            for (auto& sink : persistentSinks)
                sink->update();
        }
        
        if (onTick)
            onTick();
        
        return now();
    }
    
    void Clock::addPersistentSink(Sink& sink)
    {
        std::unique_lock<std::mutex> lock(persistencyMutex);
        persistentSinks.emplace(&sink);
    }
    
    void Clock::removePersistentSink(Sink& sink)
    {
        std::unique_lock<std::mutex> lock(persistencyMutex);
        persistentSinks.erase(&sink);
    }
    
    bool Clock::isSinkPersistent(const Sink& sink) const
    {
        return persistentSinks.count(const_cast<Sink*>(&sink));
    }
    
// --- InvariableClock --- //

    InvariableClock::InvariableClock(float rateInHertz) :
        rate_(rateInHertz)
    {
        
    }
    
// --- VariableClock --- //

    VariableClock::VariableClock(float maximalRate) :
        maximalRate(maximalRate),
        rate_(maximalRate)
    {
        lastNow = std::chrono::high_resolution_clock::now();
    }

    void VariableClock::ticked()
    {
        auto now = high_resolution_clock::now();
        auto diff = duration_cast<std::chrono::duration<float>>(now - lastNow);
        
        if (diff.count() < 1.0 / maximalRate)
        {
            std::this_thread::sleep_for(diff);
            rate_ = maximalRate;
            lastNow += duration_cast<high_resolution_clock::duration>(duration<double>(1.0 / maximalRate));
        } else {
            rate_ = 1.0f / diff.count();
            lastNow = now;
        }

        ++timestamp;
    }
}
