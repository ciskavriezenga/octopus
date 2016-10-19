#ifndef OCTOPUS_FOLD_HPP
#define OCTOPUS_FOLD_HPP

#include <numeric>
#include <stdexcept>
#include <vector>

#include "Signal.hpp"
#include "Value.hpp"

namespace octo
{
    //! Folds a variadic amount of signals into one
    template <class In, class Out = In>
    class Fold : public Signal<Out>
    {
    public:
        //! Construct the fold with a clock
        Fold(Clock& clock) : Signal<Out>(clock) { }
        
        //! Add a new input to the fold
        void emplace(Value<In> input)
        {
            inputs.emplace_back(std::move(input));
        }
        
    private:
        //! Generate a new sample
        void generateSample(Out& out) final override
        {
            if (inputs.empty())
                out = {};
            else
                out = std::accumulate(inputs.begin(), inputs.end(), init(), [&](auto& a, auto& b){ return fold(a, b[0]); });
        }
        
        //! Return the initial value of the fold
        virtual Out init() const = 0;
        
        //! Folds an input sample with some output value
        virtual Out fold(const Out& out, const In& in) const = 0;
        
    private:
        //! The inputs to the fold
        std::vector<Value<In>> inputs;
    };
}

#endif
