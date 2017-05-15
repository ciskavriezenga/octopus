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

#ifndef OCTOPUS_BINARY_OPERATION_HPP
#define OCTOPUS_BINARY_OPERATION_HPP

#include <stdexcept>

#include "cached_signal.hpp"
#include "value.hpp"

namespace octo
{
    //! Applies a binary operation on two input signals
    template <class T>
    class BinaryOperation : public CachedSignal<T>
    {
    public:
        //! Construct an empty binary operation
        BinaryOperation(Clock* clock, const T& initialCache = T{}) :
            CachedSignal<T>(clock, initialCache)
        {
            
        }
        
        //! Construct the binary operation with two terms and its own clock
        BinaryOperation(Clock* clock, Value<T> left, Value<T> right) :
            Signal<T>(clock),
            left(std::move(left)),
            right(std::move(right))
        {
            
        }
        
    public:
        //! The left-hand side of the operation
        Value<T> left;
        
        //! The left-hand side of the operation
        Value<T> right;
        
    private:
        //! Generate a new sample
        void generateSample(T& out) final
        {
            combineSamples(left(), right(), out);
        }
        
        //! Combine two samples into a new one
        virtual void combineSamples(const T& left, const T& right, T& out) = 0;
    };
}

#endif
