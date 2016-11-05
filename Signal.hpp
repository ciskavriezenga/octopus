#ifndef OCTOPUS_SIGNAL_HPP
#define OCTOPUS_SIGNAL_HPP

#include <dsperados/math/utility.hpp>
#include <memory>
#include <stdexcept>
#include <vector>

#include "Clock.hpp"
#include "SignalBase.hpp"

namespace octo
{
    //! A discrete signal of _any_ given type.
    /*! This is the major base class of all signal processing. Everything is a signal and
        individual samples can be accessed using the subscript operator, like so:
     
        @code{.cpp}
        Sine sine;
        cout << sine[0] << sine[1] << endl;
     
        AudioFile file;
        auto x = file[2] * file[2];
        @endcode
     
        Besides accessing individual signal elements using the math-like [] syntax, one can
        manipulate signals as monolithic entities, reminiscent of mathematical signal theory
        formulas.
     
        @code{.cpp}
        // Vibrato
        sine.frequency = Sine(0.1) * 30 + 440;
     
        // A very simple low-pass filter
        auto filter = (x + Delay(x, 1)) * 0.5;
        @endcode */
    template <class T>
    class Signal : public SignalBase
    {
    public:
        //! Construct the signal
        Signal(Clock& clock) :
            clock(&clock),
            cache(64),
            timestamp(clock.now())
        {
            
        }
        
        //! Virtual destructor, because this is a base class
        virtual ~Signal() = default;
        
        //! Retrieve a signal of the sample, relative to the its clock's current timestamp
        /*! @return A reference to the generated sample in cache. Copy and be done with it, this could change with each new [] call */
        const T& operator[](int z)
        {
            // If a sample before the beginning of clock time is asked, return the first one ever
            const auto now = clock->now();
            if (now < -z)
                z = -static_cast<int>(now);
            
            // Generate new samples up to the requested one
            const auto requestedTimestamp = now + z;
            while (timestamp <= requestedTimestamp)
            {
                ++timestamp;
                cacheIndex = math::wrap(cacheIndex + 1, 0, cache.size());
                generateSample(cache[cacheIndex]);
            }
            
            // If the requested sample is too far back for it to be in the cache, throw
            if (requestedTimestamp + cache.size() <= timestamp)
                throw std::runtime_error("sample out of cache");
            
            // Return the correct sample from the cache
            auto requestedIndex = (int64_t)cacheIndex + (int64_t)z;
            return cache[math::wrap(requestedIndex, 0, cache.size())];
        }
        
        //! Return the current sample of the signal
        explicit operator T() const { return (*this)[0]; }
        
        //! Move this signal to the heap
        /*! Signals need to implement this to support in-place creation of signals in expressions.
            @note This function can only be used on r-value signal objects. */
        virtual std::unique_ptr<Signal> moveToHeap() && = 0;
        
        //! Retrieve the clock this signal runs at
        Clock& getClock() const { return *clock; }
        
        //! Retrieve the type info of the output
        const std::type_info& getTypeInfo() const final override { return typeid(T); }
        
    private:
        //! Generate a new sample
        virtual void generateSample(T& out) = 0;
        
    private:
        //! The clock this signal runs at
        Clock* clock = nullptr;
        
        //! A cache for previously generated samples
        std::vector<T> cache;
        
        //! The read index of the cache
        unsigned int cacheIndex = 0;
        
        //! The timestamp of the next-to-be generated sample
        uint64_t timestamp = 0;
    };
    
    // Convenience macro for overriding Signal::move()
    #define GENERATE_MOVE(CLASS) \
    std::unique_ptr<octo::Signal<typename std::decay<decltype(std::declval<CLASS>()[unit::discrete<uint64_t>(0)])>::type>> moveToHeap() && override \
    { \
        return std::make_unique<CLASS>(std::move(*this)); \
    }
}

#endif
