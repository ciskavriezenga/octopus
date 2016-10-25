//
//  Join.hpp
//  octopus
//
//  Created by Stijn on 17/10/2016.
//
//

#ifndef OCTOPUS_JOIN_HPP
#define OCTOPUS_JOIN_HPP

#include <vector>

#include "Fold.hpp"

namespace octo
{
    //! Joins multiple signals into one multi-channel signal
    template <class T>
    class Join : public Fold<T, std::vector<T>>
    {
    public:        
        using Fold<T, std::vector<T>>::Fold;
        
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
