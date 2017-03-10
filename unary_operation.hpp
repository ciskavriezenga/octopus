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

#ifndef OCTOPUS_UNARY_OPERATION_HPP
#define OCTOPUS_UNARY_OPERATION_HPP

#include "signal.hpp"
#include "value.hpp"

namespace octo
{
    //! Applies a unary operation on a single input signal
    template <class In, class Out = In>
    class UnaryOperation : public Signal<Out>
    {
    public:
        //! Construct the unary operation
        UnaryOperation(Clock* clock, Value<In> input = In{}) :
            Signal<Out>(clock),
            input(std::move(input))
        {
            
        }
        
    public:
        //! The input to the operation
        Value<In> input;
        
    private:
        //! Convert the sample
        virtual void convertSample(const In& in, Out& out) = 0;
        
        //! Generate a new sample by transforming it from input to output
        void generateSample(Out& out) final override
        {
            convertSample(input(), out);
        }
    };
}

#endif
