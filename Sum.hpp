#ifndef OCTOPUS_SUM_HPP
#define OCTOPUS_SUM_HPP

#include <type_traits>

#include "Fold.hpp"

namespace octo
{
    //! Sums an variadic amount of values into one
    template <class Clock, class T>
    class Sum : public Fold<Clock, T>
    {
    public:
        //! Construct an empty sum
        Sum() = default;
        
        //! Construct a sum with two terms
        Sum(Value<Clock, T> lhs, Value<Clock, T> rhs)
        {
            this->emplace(std::move(lhs));
            this->emplace(std::move(rhs));
        }
        
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
    
    template <class Clock, class T1, class T2>
    std::enable_if_t<std::is_convertible<T1, T2>::value, Sum<Clock, T2>> operator+(const T1& lhs, Signal<Clock, T2>& rhs) { return {lhs, rhs}; }
    
    template <class T1, class T2>
    std::enable_if_t<std::is_convertible<T1, T2>::value, Sum<Clock, T2>> operator+(const T1& lhs, Signal<Clock, T2>&& rhs) { return {lhs, std::move(rhs).moveToHeap()}; }
    
    template <class Clock, class T1, class T2>
    std::enable_if_t<std::is_convertible<T2, T1>::value, Sum<Clock, T1>> operator+(Signal<Clock, T1>& lhs, const T2& rhs) { return {lhs, rhs}; }
    
    template <class Clock, class T1, class T2>
    Sum<Clock, std::common_type_t<T1, T2>> operator+(Signal<Clock, T1>& lhs, Signal<Clock, T2>& rhs) { return {lhs, rhs}; }
    
    template <class Clock, class T1, class T2>
    Sum<Clock, std::common_type_t<T1, T2>> operator+(Signal<Clock, T1>& lhs, Signal<Clock, T2>&& rhs) { return {lhs, std::move(rhs).moveToHeap()}; }
    
    template <class Clock, class T1, class T2>
    std::enable_if_t<std::is_convertible<T2, T1>::value, Sum<Clock, T1>> operator+(Signal<Clock, T1>&& lhs, const T2& rhs) { return {std::move(lhs).moveToHeap(), rhs}; }
    
    template <class Clock, class T1, class T2>
    Sum<Clock, std::common_type_t<T1, T2>> operator+(Signal<Clock, T1>&& lhs, Signal<Clock, T2>& rhs) { return {std::move(lhs).moveToHeap(), rhs}; }
    
    template <class Clock, class T1, class T2>
    Sum<Clock, std::common_type_t<T1, T2>> operator+(Signal<Clock, T1>&& lhs, Signal<Clock, T2>&& rhs) { return {std::move(lhs).moveToHeap(), std::move(rhs).moveToHeap()}; }
    
    template <class Clock, class T1, class T2>
    Sum<Clock, T1> operator+(Sum<Clock, T1>&& lhs, T2&& rhs) { lhs.emplace(std::forward<T2&&>(rhs)); return std::move(lhs); }
    
    template <class Clock, class T1, class T2>
    Sum<Clock, T2> operator+(T1&& lhs, Sum<Clock, T2>&& rhs) { rhs.emplace(std::forward<T1&&>(lhs)); return std::move(rhs); }
}

#endif
