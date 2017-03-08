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

#include "signal_base.hpp"

using namespace std;

namespace octo
{
    SignalBase::SignalBase(Clock* clock) :
        Sink(clock)
    {
        
    }
    
    SignalBase::~SignalBase()
    {
        disconnectDependees();
    }
    
    void SignalBase::disconnectDependees()
    {
        auto cachedDependees = dependees;
        for (auto& dependee : cachedDependees)
            dependee->disconnectFromDependent(*this);
        
        if (!dependees.empty())
            throw runtime_error("not all dependees disconnected");
    }
}
