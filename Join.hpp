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
    template <class Domain, class T>
    class Join : public Fold<Domain, T, std::vector<T>>
    {
    public:        
        //! Construct an empty join
        Join() = default;
        
        //! Construct a join with two channels
        Join(Value<Domain, T> lhs, Value<Domain, T> rhs)
        {
            this->emplace(std::move(lhs));
            this->emplace(std::move(rhs));
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
    template <class Domain, class T1, class T2>
    std::enable_if_t<std::is_convertible<T1, T2>::value, Join<Domain, T2>> operator&(const T1& lhs, Signal<Domain, T2>& rhs) { return {lhs, rhs}; }
    
    //! Combine a scalar and a signal into a join
    template <class Domain, class T1, class T2>
    std::enable_if_t<std::is_convertible<T1, T2>::value, Join<Domain, T2>> operator&(const T1& lhs, Signal<Domain, T2>&& rhs) { return {lhs, std::move(rhs).moveToHeap()}; }
    
    //! Combine a scalar and a signal into a join
    template <class Domain, class T1, class T2>
    std::enable_if_t<std::is_convertible<T2, T1>::value, Join<Domain, T1>> operator&(Signal<Domain, T1>& lhs, const T2& rhs) { return {lhs, rhs}; }
    
    //! Combine two signals into a join
    template <class Domain, class T1, class T2>
    Join<Domain, std::common_type_t<T1, T2>> operator&(Signal<Domain, T1>& lhs, Signal<Domain, T2>& rhs) { return {lhs, rhs}; }
    
    //! Combine two signals into a join
    template <class Domain, class T1, class T2>
    Join<Domain, std::common_type_t<T1, T2>> operator&(Signal<Domain, T1>& lhs, Signal<Domain, T2>&& rhs) { return {lhs, std::move(rhs).moveToHeap()}; }
    
    //! Combine a scalar and a signal into a join
    template <class Domain, class T1, class T2>
    std::enable_if_t<std::is_convertible<T2, T1>::value, Join<Domain, T1>> operator&(Signal<Domain, T1>&& lhs, const T2& rhs) { return {std::move(lhs).moveToHeap(), rhs}; }
    
    //! Combine two signals into a join
    template <class Domain, class T1, class T2>
    Join<Domain, std::common_type_t<T1, T2>> operator&(Signal<Domain, T1>&& lhs, Signal<Domain, T2>& rhs) { return {std::move(lhs).moveToHeap(), rhs}; }
    
    //! Combine two signals into a join
    template <class Domain, class T1, class T2>
    Join<Domain, std::common_type_t<T1, T2>> operator&(Signal<Domain, T1>&& lhs, Signal<Domain, T2>&& rhs) { return {std::move(lhs).moveToHeap(), std::move(rhs).moveToHeap()}; }
    
    //! Add another term to a join
    template <class Domain, class T1, class T2>
    Join<Domain, T1> operator&(Join<Domain, T1>&& lhs, T2&& rhs) { lhs.emplace(std::forward<T2&&>(rhs)); return std::move(lhs); }
    
    //! Add another term to a join
    template <class Domain, class T1, class T2>
    Join<Domain, T2> operator&(T1&& lhs, Join<Domain, T2>&& rhs) { rhs.emplace(std::forward<T1&&>(lhs)); return std::move(rhs); }
}

#endif
