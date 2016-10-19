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
        //! Construct an empty sum
        Sum(Clock& clock) : Fold<T>(clock) { }
        
        //! Construct a sum with two terms and a clock
        Product(Clock& clock, Value<T> lhs, Value<T> rhs) :
            Fold<T>(clock)
        {
            this->emplace(std::move(lhs));
            this->emplace(std::move(rhs));
        }
        
        //! Construct a sum with two terms
        Sum(Value<T> lhs, Value<T> rhs) :
        Sum(lhs.getClock(), std::move(lhs), std::move(rhs))
        {
            if (&lhs.getClock() != &rhs.getClock())
                throw std::runtime_error("clocks of lhs and rhs of sum don't match");
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
    
    //! Combine a scalar and a signal into a sum
    template <class T1, class T2>
    std::enable_if_t<std::is_convertible<T1, T2>::value, Sum<T2>> operator+(const T1& lhs, Signal<T2>& rhs) { return {lhs, rhs}; }
    
    //! Combine a scalar and a signal into a sum
    template <class T1, class T2>
    std::enable_if_t<std::is_convertible<T1, T2>::value, Sum<T2>> operator+(const T1& lhs, Signal<T2>&& rhs) { return {lhs, std::move(rhs).moveToHeap()}; }
    
    //! Combine a scalar and a signal into a sum
    template <class T1, class T2>
    std::enable_if_t<std::is_convertible<T2, T1>::value, Sum<T1>> operator+(Signal<T1>& lhs, const T2& rhs) { return {lhs, rhs}; }
    
    //! Combine two signals into a sum
    template <class T1, class T2>
    Sum<std::common_type_t<T1, T2>> operator+(Signal<T1>& lhs, Signal<T2>& rhs) { return {lhs, rhs}; }
    
    //! Combine two signals into a sum
    template <class T1, class T2>
    Sum<std::common_type_t<T1, T2>> operator+(Signal<T1>& lhs, Signal<T2>&& rhs) { return {lhs, std::move(rhs).moveToHeap()}; }
    
    //! Combine a scalar and a signal into a sum
    template <class T1, class T2>
    std::enable_if_t<std::is_convertible<T2, T1>::value, Sum<T1>> operator+(Signal<T1>&& lhs, const T2& rhs) { return {std::move(lhs).moveToHeap(), rhs}; }
    
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
