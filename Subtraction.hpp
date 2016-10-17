//
//  Subtraction.hpp
//  octopus
//
//  Created by Stijn on 17/10/2016.
//
//

#ifndef OCTOPUS_SUBTRACTION_HPP
#define OCTOPUS_SUBTRACTION_HPP

#include <type_traits>

#include "BinaryOperation.hpp"

namespace octo
{
    //! Subtracts a signal from another
    template <class Domain, class T>
    class Subtraction : public BinaryOperation<Domain, T>
    {
    public:
        // Reuse the constructor of BinaryOperation
        using BinaryOperation<Domain, T>::BinaryOperation;
        
        GENERATE_MOVE(Subtraction)
        
    private:
        //! Generate a new sample
        void combineSamples(const T& lhs, const T& rhs, T& out) final override
        {
            out = lhs - rhs;
        }
    };
    
    //! Combine a scalar and a signal into a subtraction
    template <class Domain, class T1, class T2>
    std::enable_if_t<std::is_convertible<T1, T2>::value, Subtraction<Domain, T2>> operator-(const T1& lhs, Signal<Domain, T2>& rhs) { return {lhs, rhs}; }
    
    //! Combine a scalar and a signal into a subtraction
    template <class Domain, class T1, class T2>
    std::enable_if_t<std::is_convertible<T1, T2>::value, Subtraction<Domain, T2>> operator-(const T1& lhs, Signal<Domain, T2>&& rhs) { return {lhs, std::move(rhs).moveToHeap()}; }
    
    //! Combine a scalar and a signal into a subtraction
    template <class Domain, class T1, class T2>
    std::enable_if_t<std::is_convertible<T2, T1>::value, Subtraction<Domain, T1>> operator-(Signal<Domain, T1>& lhs, const T2& rhs) { return {lhs, rhs}; }
    
    //! Combine two signals into a subtraction
    template <class Domain, class T1, class T2>
    Subtraction<Domain, std::common_type_t<T1, T2>> operator-(Signal<Domain, T1>& lhs, Signal<Domain, T2>& rhs) { return {lhs, rhs}; }
    
    //! Combine two signals into a subtraction
    template <class Domain, class T1, class T2>
    Subtraction<Domain, std::common_type_t<T1, T2>> operator-(Signal<Domain, T1>& lhs, Signal<Domain, T2>&& rhs) { return {lhs, std::move(rhs).moveToHeap()}; }
    
    //! Combine a scalar and a signal into a subtraction
    template <class Domain, class T1, class T2>
    std::enable_if_t<std::is_convertible<T2, T1>::value, Subtraction<Domain, T1>> operator-(Signal<Domain, T1>&& lhs, const T2& rhs) { return {std::move(lhs).moveToHeap(), rhs}; }
    
    //! Combine two signals into a subtraction
    template <class Domain, class T1, class T2>
    Subtraction<Domain, std::common_type_t<T1, T2>> operator-(Signal<Domain, T1>&& lhs, Signal<Domain, T2>& rhs) { return {std::move(lhs).moveToHeap(), rhs}; }
    
    //! Combine two signals into a subtraction
    template <class Domain, class T1, class T2>
    Subtraction<Domain, std::common_type_t<T1, T2>> operator-(Signal<Domain, T1>&& lhs, Signal<Domain, T2>&& rhs) { return {std::move(lhs).moveToHeap(), std::move(rhs).moveToHeap()}; }
}

#endif

