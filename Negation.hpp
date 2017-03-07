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

#ifndef OCTOPUS_NEGATION_HPP
#define OCTOPUS_NEGATION_HPP

#include "UnaryOperation.hpp"

namespace octo
{
    //! Negate a signal
    template <class T>
    class Negation : public UnaryOperation<T>
    {
    public:
        // Use the constructor from UnaryOperation
        using UnaryOperation<T>::UnaryOperation;
        
        // Generate the moveToHeap() function
        GENERATE_MOVE(Negation)
        
    private:
        //! Generate a negative sample
        void convertSample(const T& in, T& out) final override
        {
            out = -in;
        }
    };
    
    //! Operator overload for negating signals
    template <class T>
    Negation<T> operator-(Signal<T>& signal)
    {
        return {signal.getClock(), signal};
    }
    
    //! Operator overload for negating signals
    template <class T>
    Negation<T> operator-(Signal<T>&& signal)
    {
        return {signal.getClock(), std::move(signal)};
    }
}

#endif
