#ifndef OCTOPUS_SUM_HPP
#define OCTOPUS_SUM_HPP

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
        
        //! Construct a sum with two terms
        Sum(Value<T> lhs, Value<T> rhs) :
            Fold<T>(lhs.getClock())
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
    
    template <class T, class U>
    Sum<T> operator+(const T& lhs, Signal<U>& rhs) { return {lhs, rhs}; }
    
    template <class T, class U>
    Sum<T> operator+(const T& lhs, Signal<U>&& rhs) { return {lhs, std::move(rhs).moveToHeap()}; }
    
    template <class T, class U>
    Sum<T> operator+(Signal<T>& lhs, const U& rhs) { auto& clock = lhs.getClock(); return {lhs, {clock, static_cast<T>(rhs)}}; }
    
    template <class T, class U>
    Sum<T> operator+(Signal<T>& lhs, Signal<U>& rhs) { return {lhs, rhs}; }
    
    template <class T, class U>
    Sum<T> operator+(Signal<T>& lhs, Signal<U>&& rhs) { return {lhs, std::move(rhs).moveToHeap()}; }
    
    template <class T, class U>
    Sum<T> operator+(Signal<T>&& lhs, const U& rhs) { auto& clock = lhs.getClock(); return {std::move(lhs).moveToHeap(), {clock, static_cast<T>(rhs)}}; }
    
    template <class T, class U>
    Sum<T> operator+(Signal<T>&& lhs, Signal<U>& rhs) { return {std::move(lhs).moveToHeap(), rhs}; }
    
    template <class T, class U>
    Sum<T> operator+(Signal<T>&& lhs, Signal<U>&& rhs) { return {std::move(lhs).moveToHeap(), std::move(rhs).moveToHeap()}; }
    
    template <class T, class U>
    Sum<T> operator+(Sum<T>&& lhs, U&& rhs) { lhs.emplace(std::forward<U&&>(rhs)); return std::move(lhs); }
    
    template <class T, class U>
    Sum<U> operator+(T&& lhs, Sum<U>&& rhs) { rhs.emplace(std::forward<T&&>(lhs)); return std::move(rhs); }
}

#endif
