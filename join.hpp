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

#ifndef OCTOPUS_JOIN_HPP
#define OCTOPUS_JOIN_HPP

#include <vector>

#include "fold.hpp"

namespace octo
{
    //! Join multiple signals into one multi-channel signal
    /*! Combines multiple signals of type T into a signal of type std::vector<T>.
        Signals with a vector type are the common idiom within Octopus for representing
        multi-channel signals.
     
        Signals can be easily joined using the & syntax
        @code{cpp}
        Sine<float> sine1(clock), sine2(clock);
        auto join = sine1 & sine2;
        @endcode*/
    template <class T>
    class Join : public Fold<T, std::vector<T>>
    {
    public:        
        using Fold<T, std::vector<T>>::Fold;
        
        //! Add another channel to the join
        template <class U>
        Join& operator&=(U&& rhs)
        {
            this->emplace(std::forward<U&&>(rhs));
            return *this;
        }
        
        // Generate the move function
        GENERATE_MOVE(Join)
        
    private:
        //! Return the monoid identity
        std::vector<T> init() const final override { return {}; }
        
        //! The monoid binary operation
        std::vector<T> fold(const std::vector<T>& acc, const T& x) const final override
        {
            auto y = acc;
            y.emplace_back(x);
            return y;
        }
    };
    
    //! Combine a scalar and a signal into a join
    template <class T1, class T2>
    std::enable_if_t<std::is_convertible<T1, T2>::value, Join<T2>> operator&(const T1& lhs, Signal<T2>& rhs) { return {lhs, rhs}; }
    
    //! Combine a scalar and a signal into a join
    template <class T1, class T2>
    std::enable_if_t<std::is_convertible<T1, T2>::value, Join<T2>> operator&(const T1& lhs, Signal<T2>&& rhs) { return {lhs, std::move(rhs).moveToHeap()}; }
    
    //! Combine a scalar and a signal into a join
    template <class T1, class T2>
    std::enable_if_t<std::is_convertible<T2, T1>::value, Join<T1>> operator&(Signal<T1>& lhs, const T2& rhs) { return {lhs, rhs}; }
    
    //! Combine two signals into a join
    template <class T1, class T2>
    Join<std::common_type_t<T1, T2>> operator&(Signal<T1>& lhs, Signal<T2>& rhs) { return {lhs, rhs}; }
    
    //! Combine two signals into a join
    template <class T1, class T2>
    Join<std::common_type_t<T1, T2>> operator&(Signal<T1>& lhs, Signal<T2>&& rhs) { return {lhs, std::move(rhs).moveToHeap()}; }
    
    //! Combine a scalar and a signal into a join
    template <class T1, class T2>
    std::enable_if_t<std::is_convertible<T2, T1>::value, Join<T1>> operator&(Signal<T1>&& lhs, const T2& rhs) { return {std::move(lhs).moveToHeap(), rhs}; }
    
    //! Combine two signals into a join
    template <class T1, class T2>
    Join<std::common_type_t<T1, T2>> operator&(Signal<T1>&& lhs, Signal<T2>& rhs) { return {std::move(lhs).moveToHeap(), rhs}; }
    
    //! Combine two signals into a join
    template <class T1, class T2>
    Join<std::common_type_t<T1, T2>> operator&(Signal<T1>&& lhs, Signal<T2>&& rhs) { return {std::move(lhs).moveToHeap(), std::move(rhs).moveToHeap()}; }
    
    //! Add another term to a join
    template <class T1, class T2>
    Join<T1> operator&(Join<T1>&& lhs, T2&& rhs) { lhs.emplace(std::forward<T2&&>(rhs)); return std::move(lhs); }
    
    //! Add another term to a join
    template <class T1, class T2>
    Join<T2> operator&(T1&& lhs, Join<T2>&& rhs) { rhs.emplace(std::forward<T1&&>(lhs)); return std::move(rhs); }
}

#endif
