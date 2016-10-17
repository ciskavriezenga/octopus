//
//  Product.hpp
//  octopus
//
//  Created by Stijn on 17/10/2016.
//
//

#ifndef OCTOPUS_PRODUCT_HPP
#define OCTOPUS_PRODUCT_HPP

#include <type_traits>

#include "Fold.hpp"

namespace octo
{
    //! Multiples an variadic amount of values into one
    template <class Domain, class T>
    class Product : public Fold<Domain, T>
    {
    public:
        //! Construct an empty product
        Product() = default;
        
        //! Construct a product with two terms
        Product(Value<Domain, T> lhs, Value<Domain, T> rhs)
        {
            this->emplace(std::move(lhs));
            this->emplace(std::move(rhs));
        }
        
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
    template <class Domain, class T1, class T2>
    std::enable_if_t<std::is_convertible<T1, T2>::value, Product<Domain, T2>> operator*(const T1& lhs, Signal<Domain, T2>& rhs) { return {lhs, rhs}; }
    
    //! Combine a scalar and a signal into a product
    template <class Domain, class T1, class T2>
    std::enable_if_t<std::is_convertible<T1, T2>::value, Product<Domain, T2>> operator*(const T1& lhs, Signal<Domain, T2>&& rhs) { return {lhs, std::move(rhs).moveToHeap()}; }
    
    //! Combine a scalar and a signal into a product
    template <class Domain, class T1, class T2>
    std::enable_if_t<std::is_convertible<T2, T1>::value, Product<Domain, T1>> operator*(Signal<Domain, T1>& lhs, const T2& rhs) { return {lhs, rhs}; }
    
    //! Combine two signals into a product
    template <class Domain, class T1, class T2>
    Product<Domain, std::common_type_t<T1, T2>> operator*(Signal<Domain, T1>& lhs, Signal<Domain, T2>& rhs) { return {lhs, rhs}; }
    
    //! Combine two signals into a product
    template <class Domain, class T1, class T2>
    Product<Domain, std::common_type_t<T1, T2>> operator*(Signal<Domain, T1>& lhs, Signal<Domain, T2>&& rhs) { return {lhs, std::move(rhs).moveToHeap()}; }
    
    //! Combine a scalar and a signal into a product
    template <class Domain, class T1, class T2>
    std::enable_if_t<std::is_convertible<T2, T1>::value, Product<Domain, T1>> operator*(Signal<Domain, T1>&& lhs, const T2& rhs) { return {std::move(lhs).moveToHeap(), rhs}; }
    
    //! Combine two signals into a product
    template <class Domain, class T1, class T2>
    Product<Domain, std::common_type_t<T1, T2>> operator*(Signal<Domain, T1>&& lhs, Signal<Domain, T2>& rhs) { return {std::move(lhs).moveToHeap(), rhs}; }
    
    //! Combine two signals into a product
    template <class Domain, class T1, class T2>
    Product<Domain, std::common_type_t<T1, T2>> operator*(Signal<Domain, T1>&& lhs, Signal<Domain, T2>&& rhs) { return {std::move(lhs).moveToHeap(), std::move(rhs).moveToHeap()}; }
    
    //! Add another term to a product
    template <class Domain, class T1, class T2>
    Product<Domain, T1> operator*(Product<Domain, T1>&& lhs, T2&& rhs) { lhs.emplace(std::forward<T2&&>(rhs)); return std::move(lhs); }
    
    //! Add another term to a product
    template <class Domain, class T1, class T2>
    Product<Domain, T2> operator*(T1&& lhs, Product<Domain, T2>&& rhs) { rhs.emplace(std::forward<T1&&>(lhs)); return std::move(rhs); }
}

#endif
