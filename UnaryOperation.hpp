/*
 
 This file is a part of Octopus, a modern C++ library for embedding digital
 signal processing as a language inside your software. It transcends a single
 domain (audio, video, math, etc.), combining multiple clocks in one graph.
 
 Copyright (C) 2016 Dsperados <info@dsperados.com>
 
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

#ifndef OCTOPUS_UNARY_OPERATION_HPP
#define OCTOPUS_UNARY_OPERATION_HPP

#include "Signal.hpp"
#include "Value.hpp"

namespace octo
{
    //! Applies a unary operation on a single input signal
    template <class In, class Out = In>
    class UnaryOperation : public Signal<Out>
    {
    public:
        //! Construct an empty unary operation
        UnaryOperation(Clock& clock) :
            Signal<Out>(clock),
            input(clock)
        {
            
        }
        
        //! Construct the unary operation with its input
        UnaryOperation(Clock& clock, Value<In> input) :
            Signal<Out>(clock),
            input(std::move(input))
        {
            
        }
        
        //! Construct the unary operation with its input
        UnaryOperation(Value<In> input) :
            UnaryOperation(input.getClock(), std::move(input))
        {
            
        }
        
    public:
        //! The input to the operation
        Value<In> input;
    };
}

#endif
