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

#ifndef OCTOPUS_SUBTRACTION_HPP
#define OCTOPUS_SUBTRACTION_HPP

#include <type_traits>

#include "BinaryOperation.hpp"

namespace octo
{
    //! Subtract a signal from another
    template <class T>
    class Subtraction : public BinaryOperation<T>
    {
    public:
        // Reuse the constructor of BinaryOperation
        using BinaryOperation<T>::BinaryOperation;
        
        GENERATE_MOVE(Subtraction)
        
    private:
        //! Generate a new sample
        void combineSamples(const T& lhs, const T& rhs, T& out) final override
        {
            out = lhs - rhs;
        }
    };
    
    //! Combine a scalar and a signal into a subtraction
    template <class T1, class T2>
    std::enable_if_t<std::is_convertible<T1, T2>::value, Subtraction<T2>> operator-(const T1& lhs, Signal<T2>& rhs) { return {Value<T1>(rhs.getClock(), lhs), rhs}; }
    
    //! Combine a scalar and a signal into a subtraction
    template <class T1, class T2>
    std::enable_if_t<std::is_convertible<T1, T2>::value, Subtraction<T2>> operator-(const T1& lhs, Signal<T2>&& rhs) { return {Value<T1>(rhs.getClock(), lhs), std::move(rhs).moveToHeap()}; }
    
    //! Combine a scalar and a signal into a subtraction
    template <class T1, class T2>
    std::enable_if_t<std::is_convertible<T2, T1>::value, Subtraction<T1>> operator-(Signal<T1>& lhs, const T2& rhs) { return {lhs, Value<T2>(lhs.getClock(), rhs)}; }
    
    //! Combine two signals into a subtraction
    template <class T1, class T2>
    Subtraction<std::common_type_t<T1, T2>> operator-(Signal<T1>& lhs, Signal<T2>& rhs) { return {lhs, rhs}; }
    
    //! Combine two signals into a subtraction
    template <class T1, class T2>
    Subtraction<std::common_type_t<T1, T2>> operator-(Signal<T1>& lhs, Signal<T2>&& rhs) { return {lhs, std::move(rhs).moveToHeap()}; }
    
    //! Combine a scalar and a signal into a subtraction
    template <class T1, class T2>
    std::enable_if_t<std::is_convertible<T2, T1>::value, Subtraction<T1>> operator-(Signal<T1>&& lhs, const T2& rhs) { return {std::move(lhs).moveToHeap(), Value<T2>(lhs.getClock(), rhs)}; }
    
    //! Combine two signals into a subtraction
    template <class T1, class T2>
    Subtraction<std::common_type_t<T1, T2>> operator-(Signal<T1>&& lhs, Signal<T2>& rhs) { return {std::move(lhs).moveToHeap(), rhs}; }
    
    //! Combine two signals into a subtraction
    template <class T1, class T2>
    Subtraction<std::common_type_t<T1, T2>> operator-(Signal<T1>&& lhs, Signal<T2>&& rhs) { return {std::move(lhs).moveToHeap(), std::move(rhs).moveToHeap()}; }
}

#endif

