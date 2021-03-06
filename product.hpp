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

#ifndef OCTOPUS_PRODUCT_HPP
#define OCTOPUS_PRODUCT_HPP

#include <type_traits>

#include "fold.hpp"

namespace octo
{
    //! Multiplies an variadic amount of signals with each other
    template <class T>
    class Product : public Fold<T>
    {
    public:
        using Fold<T>::Fold;
        
        //! Add another term to the product
        template <class U>
        Product& operator*=(U&& rhs)
        {
            this->emplace(std::forward<U&&>(rhs));
            return *this;
        }
        
        GENERATE_MOVE(Product)
        
    private:
        // Inherited from Fold
        T init() const final override { return 1; }
        T fold(const T& in, const T& out) const final override { return in * out; }
    };
    
    //! Combine a scalar and a signal into a product
    template <class T1, class T2>
    std::enable_if_t<std::is_convertible<T1, T2>::value, Product<T2>> operator*(const T1& lhs, Signal<T2>& rhs)
    {
        return {rhs.getClock(), lhs, rhs};
    }
    
    //! Combine a scalar and a signal into a product
    template <class T1, class T2>
    std::enable_if_t<std::is_convertible<T1, T2>::value, Product<T2>> operator*(const T1& lhs, Signal<T2>&& rhs)
    {
        return {rhs.getClock(), lhs, std::move(rhs)};
    }
    
    //! Combine a scalar and a signal into a product
    template <class T1, class T2>
    std::enable_if_t<std::is_convertible<T2, T1>::value, Product<T1>> operator*(Signal<T1>& lhs, const T2& rhs)
    {
        return {lhs.getClock(), lhs, rhs};
    }
    
    //! Combine two signals into a product
    template <class T1, class T2>
    Product<std::common_type_t<T1, T2>> operator*(Signal<T1>& lhs, Signal<T2>& rhs)
    {
        if (lhs.getClock() != rhs.getClock())
            throw std::invalid_argument("cannot multiply two signals that do not use the same clock");
        
        return {lhs.getClock(), lhs, rhs};
    }
    
    //! Combine two signals into a product
    template <class T1, class T2>
    Product<std::common_type_t<T1, T2>> operator*(Signal<T1>& lhs, Signal<T2>&& rhs)
    {
        if (lhs.getClock() != rhs.getClock())
            throw std::invalid_argument("cannot multiply two signals that do not use the same clock");
        
        return {lhs.getClock(), lhs, std::move(rhs)};
    }
    
    //! Combine a scalar and a signal into a product
    template <class T1, class T2>
    std::enable_if_t<std::is_convertible<T2, T1>::value, Product<T1>> operator*(Signal<T1>&& lhs, const T2& rhs)
    {
        return {lhs.getClock(), std::move(lhs), rhs};
    }
    
    //! Combine two signals into a product
    template <class T1, class T2>
    Product<std::common_type_t<T1, T2>> operator*(Signal<T1>&& lhs, Signal<T2>& rhs)
    {
        if (lhs.getClock() != rhs.getClock())
            throw std::invalid_argument("cannot multiply two signals that do not use the same clock");
        
        return {lhs.getClock(), std::move(lhs), rhs};
    }
    
    //! Combine two signals into a product
    template <class T1, class T2>
    Product<std::common_type_t<T1, T2>> operator*(Signal<T1>&& lhs, Signal<T2>&& rhs)
    {
        if (lhs.getClock() != rhs.getClock())
            throw std::invalid_argument("cannot multiply two signals that do not use the same clock");
        
        return {lhs.getClock(), std::move(lhs), std::move(rhs)};
    }
    
    //! Add another factor to a product
    template <class T1, class T2>
    Product<T1> operator*(Product<T1>&& lhs, T2&& rhs)
    {
        lhs.emplace(std::forward<T2&&>(rhs));
        return std::move(lhs);
    }
    
    //! Add another factor to a product
    template <class T1, class T2>
    Product<T2> operator*(T1&& lhs, Product<T2>&& rhs)
    {
        rhs.emplace(std::forward<T1&&>(lhs));
        return std::move(rhs);
    }
}

#endif
