#ifndef OCTOPUS_FOLD_HPP
#define OCTOPUS_FOLD_HPP

#include <cstdint>
#include <memory>
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
        //! Construct an empty sum
        Fold(Clock& clock) : Signal<Out>(clock) { }
        
        //! Construct an empty sum
        Fold(Clock& clock, std::size_t size) :
            Signal<Out>(clock)
        {
            resize(size);
        }
        
        //! Construct a sum with two terms and a clock
        Fold(Clock& clock, Value<In> lhs, Value<In> rhs) :
            Signal<Out>(clock)
        {
            this->emplace(std::move(lhs));
            this->emplace(std::move(rhs));
        }
        
        //! Construct a sum with two terms
        Fold(Value<In> lhs, Value<In> rhs)
        {
            if (&lhs.getClock() != &rhs.getClock())
                throw std::runtime_error("clocks of lhs and rhs of fold don't match");
            
            this->emplace(std::move(lhs));
            this->emplace(std::move(rhs));
        }
        
        //! Add a new input to the fold
        void emplace(Value<In> input)
        {
            inputs.emplace_back(std::make_unique<Value<In>>(std::move(input)));
        }
        
        //! Resize the amount of inputs
        void resize(std::size_t size)
        {
            const auto oldSize = inputs.size();
            inputs.resize(size);
            
            for (auto i = oldSize; i < size; ++i)
                inputs[i] = std::make_unique<Value<In>>(this->getClock());
        }
        
        //! Retrieve one of the inputs of the fold
        Value<In>& getInput(std::size_t index) { return *inputs.at(index); }
        
        //! Return the number of inputs to the fold
        std::size_t getInputCount() const { return inputs.size(); }
        
    private:
        //! Generate a new sample
        void generateSample(Out& out) const final override
        {
            if (inputs.empty())
                out = {};
            else
                out = std::accumulate(inputs.begin(), inputs.end(), init(), [&](auto& a, auto& b){ return fold(a, (*b)[0]); });
        }
        
        //! Return the initial value of the fold
        virtual Out init() const = 0;
        
        //! Folds an input sample with some output value
        virtual Out fold(const Out& out, const In& in) const = 0;
        
    private:
        //! The inputs to the fold
        std::vector<std::unique_ptr<Value<In>>> inputs;
    };
}

#endif
