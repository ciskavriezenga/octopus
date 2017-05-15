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

#ifndef OCTOPUS_SUM_HPP
#define OCTOPUS_SUM_HPP

#include <type_traits>

#include "fold.hpp"

namespace octo
{
    //! Sums an variadic amount of signals into one
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
        
    private:
        // Inherited from Fold
        T init() const final { return 0; }
        T fold(const T& in, const T& out) const final { return in + out; }
    };
    
    // scalar + &
    template <typename Lhs, typename Rhs, typename = typename std::enable_if_t<
        !std::is_base_of<Sink, typename std::decay_t<Lhs>>::value &&
        std::is_base_of<Sink, typename std::decay_t<Rhs>>::value>>
    Sum<std::common_type_t<Lhs, typename Rhs::type>>
    operator+(const Lhs& lhs, Rhs& rhs)
    {
        return {rhs.getClock(), lhs, rhs};
    }
    
    // scalar + &&
    template <typename Lhs, typename Rhs, typename = typename std::enable_if_t<
        !std::is_base_of<Sink, typename std::decay_t<Lhs>>::value &&
        std::is_base_of<Sink, typename std::decay_t<Rhs>>::value>>
    Sum<std::common_type_t<Lhs, typename Rhs::type>>
    operator+(const Lhs& lhs, Rhs&& rhs)
    {
        return {rhs.getClock(), lhs, std::move(rhs)};
    }
    
    // & + scalar
    template <typename Lhs, typename Rhs, typename = typename std::enable_if_t<
        std::is_base_of<Sink, typename std::decay_t<Lhs>>::value &&
        !std::is_base_of<Sink, typename std::decay_t<Rhs>>::value>>
    Sum<std::common_type_t<typename Lhs::type, Rhs>>
    operator+(Lhs& lhs, const Rhs& rhs)
    {
        return {lhs.getClock(), lhs, rhs};
    }
    
    // & + &
    template <typename Lhs, typename Rhs, typename = typename std::enable_if_t<
        std::is_base_of<Sink, typename std::decay_t<Lhs>>::value &&
        std::is_base_of<Sink, typename std::decay_t<Rhs>>::value>>
    Sum<std::common_type_t<typename Lhs::type, typename Rhs::type>>
    operator+(Lhs& lhs, Rhs& rhs)
    {
        if (lhs.getClock() != rhs.getClock())
            throw std::runtime_error("can't deduce clock in sum of two sinks");
        
        return {lhs.getClock(), lhs, rhs};
    }
    
    // & + &&
    template <typename Lhs, typename Rhs, typename = typename std::enable_if_t<
        std::is_base_of<Sink, typename std::decay_t<Lhs>>::value &&
        std::is_base_of<Sink, typename std::decay_t<Rhs>>::value>>
    Sum<std::common_type_t<typename Lhs::type, typename Rhs::type>>
    operator+(Lhs& lhs, Rhs&& rhs)
    {
        if (lhs.getClock() != rhs.getClock())
            throw std::runtime_error("can't deduce clock in sum of two sinks");
        
        return {lhs.getClock(), lhs, std::move(rhs)};
    }
    
    // && + scalar
    template <typename Lhs, typename Rhs, typename = typename std::enable_if_t<
        std::is_base_of<Sink, typename std::decay_t<Lhs>>::value &&
        !std::is_base_of<Sink, typename std::decay_t<Rhs>>::value>>
    Sum<std::common_type_t<typename Lhs::type, Rhs>>
    operator+(Lhs&& lhs, const Rhs& rhs)
    {
        return {lhs.getClock(), std::move(lhs), rhs};
    }
    
    // && + &
    template <typename Lhs, typename Rhs, typename = typename std::enable_if_t<
        std::is_base_of<Sink, typename std::decay_t<Lhs>>::value &&
        std::is_base_of<Sink, typename std::decay_t<Rhs>>::value>>
    Sum<std::common_type_t<typename Lhs::type, typename Rhs::type>>
    operator+(Lhs&& lhs, Rhs& rhs)
    {
        if (lhs.getClock() != rhs.getClock())
            throw std::runtime_error("can't deduce clock in sum of two sinks");
        
        return {lhs.getClock(), std::move(lhs), rhs};
    }
    
    // && + &&
    template <typename Lhs, typename Rhs, typename = typename std::enable_if_t<
        std::is_base_of<Sink, typename std::decay_t<Lhs>>::value &&
        std::is_base_of<Sink, typename std::decay_t<Rhs>>::value>>
    Sum<std::common_type_t<typename Lhs::type, typename Rhs::type>>
    operator+(Lhs&& lhs, Rhs&& rhs)
    {
        if (lhs.getClock() != rhs.getClock())
            throw std::runtime_error("can't deduce clock in sum of two sinks");
        
        return {lhs.getClock(), std::move(lhs), std::move(rhs)};
    }
}

#endif
