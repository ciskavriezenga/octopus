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

#ifndef OCTOPUS_SIGNAL_BASE_HPP
#define OCTOPUS_SIGNAL_BASE_HPP

#include <set>
#include <typeinfo>

#include "sink.hpp"

namespace octo
{
    //! Base class for all signals in Octopus, regardless of their output type
    class SignalBase : public Sink
    {
    public:
        //! Construct the signal base
        SignalBase(Clock* clock);
        
        //! Virtual destructor, because this is a polymorphic base class
        virtual ~SignalBase();
        
        //! Retrieve the type info of the output
        virtual const std::type_info& getTypeInfo() const = 0;
        
        //! Pull and retrieve the type-agnostic address of a sample
        /*! The address is invalidated next time someone pulls the signal */
        virtual const void* pullGeneric() = 0;
        
        //! Have all signals that depend on this one disconnect
        void disconnectDependees();
        
    public:
        //! The signals that depend on this signal
        std::set<SignalBase*> dependees;
        
    private:
        //! Called when a dependent asks not to depend on it anymore (e.g. it is being destroyed)
        virtual void disconnectFromDependent(SignalBase& dependent) { }
    };
}

#endif
