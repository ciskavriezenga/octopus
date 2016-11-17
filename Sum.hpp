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

#ifndef OCTOPUS_SUM_HPP
#define OCTOPUS_SUM_HPP

#include <type_traits>

#include "Fold.hpp"

namespace octo
{
    //! Sums an variadic amount of values into one
    template <class T>
    class Sum : public Fold<T>
    {
    public:
        using Fold<T>::Fold;
        
        //! Add another term to the sum
        template <class U>
        Sum& operator+=(U&& rhs)
        {
            this->emplace(std::forward<U&&>(rhs));
            return *this;
        }
        
        GENERATE_MOVE(Sum)
        
    private:
        // Inherited from Fold
        T init() const final override { return 0; }
        T fold(const T& in, const T& out) const final override { return in + out; }
    };
    
    //! Combine a scalar and a signal into a sum
    template <class T1, class T2>
    std::enable_if_t<std::is_convertible<T1, T2>::value, Sum<T2>> operator+(const T1& lhs, Signal<T2>& rhs) { return {Value<T1>(rhs.getClock(), lhs), rhs}; }
    
    //! Combine a scalar and a signal into a sum
    template <class T1, class T2>
    std::enable_if_t<std::is_convertible<T1, T2>::value, Sum<T2>> operator+(const T1& lhs, Signal<T2>&& rhs) { return {Value<T1>(rhs.getClock(), lhs), std::move(rhs).moveToHeap()}; }
    
    //! Combine a scalar and a signal into a sum
    template <class T1, class T2>
    std::enable_if_t<std::is_convertible<T2, T1>::value, Sum<T1>> operator+(Signal<T1>& lhs, const T2& rhs) { return {lhs, Value<T2>(lhs.getClock(), rhs)}; }
    
    //! Combine two signals into a sum
    template <class T1, class T2>
    Sum<std::common_type_t<T1, T2>> operator+(Signal<T1>& lhs, Signal<T2>& rhs) { return {lhs, rhs}; }
    
    //! Combine two signals into a sum
    template <class T1, class T2>
    Sum<std::common_type_t<T1, T2>> operator+(Signal<T1>& lhs, Signal<T2>&& rhs) { return {lhs, std::move(rhs).moveToHeap()}; }
    
    //! Combine a scalar and a signal into a sum
    template <class T1, class T2>
    std::enable_if_t<std::is_convertible<T2, T1>::value, Sum<T1>> operator+(Signal<T1>&& lhs, const T2& rhs) { return {std::move(lhs).moveToHeap(), Value<T2>(lhs.getClock(), rhs)}; }
    
    //! Combine two signals into a sum
    template <class T1, class T2>
    Sum<std::common_type_t<T1, T2>> operator+(Signal<T1>&& lhs, Signal<T2>& rhs) { return {std::move(lhs).moveToHeap(), rhs}; }
    
    //! Combine two signals into a sum
    template <class T1, class T2>
    Sum<std::common_type_t<T1, T2>> operator+(Signal<T1>&& lhs, Signal<T2>&& rhs) { return {std::move(lhs).moveToHeap(), std::move(rhs).moveToHeap()}; }
    
    //! Add another term to a sum
    template <class T1, class T2>
    Sum<T1> operator+(Sum<T1>&& lhs, T2&& rhs) { lhs.emplace(std::forward<T2&&>(rhs)); return std::move(lhs); }
    
    //! Add another term to a sum
    template <class T1, class T2>
    Sum<T2> operator+(T1&& lhs, Sum<T2>&& rhs) { rhs.emplace(std::forward<T1&&>(lhs)); return std::move(rhs); }
}

#endif
