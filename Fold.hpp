/*
 
 This file is a part of Octopus, a modern C++ library for embedding digital
 signal processing as a language inside your software. It transcends a single
 domain (audio, video, math, etc.), combining multiple clocks in one graph.
 
 Copyright (C) 2016 Dsperados <info@dsperados.com>
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>
 
 --------------------------------------------------------------------
 
 If you would like to use Octopus for commercial or closed-source
 purposes, please contact us for a commercial license.
 
 */

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
    /*! Folds (a term coming from functional programming) form a big category of signals. They
        take multiple signals of the same type and fold them into a single output signal. Sum and
        Product are good examples (they fold by applying + or * on each input signal). */
    template <class In, class Out = In>
    class Fold : public Signal<Out>
    {
    public:
        //! Construct an empty fold
        Fold(Clock* clock, const Out& initialCache = Out{}) : Signal<Out>(clock, initialCache) { }
        
        //! Construct an empty fold
        Fold(Clock* clock, std::size_t size) :
            Signal<Out>(clock)
        {
            resize(size);
        }
        
        //! Construct a fold with two terms and a clock
        Fold(Clock* clock, Value<In> lhs, Value<In> rhs) :
            Signal<Out>(clock)
        {
            this->emplace(std::move(lhs));
            this->emplace(std::move(rhs));
        }
        
        //! Construct a fold with two terms
        /*! @throw std::runtime_error: If the terms do not share the same clock */
        Fold(Value<In>&& lhs, Value<In>&& rhs) :
            Signal<Out>(lhs.getClock())
        {
            if (&lhs.getClock() != &rhs.getClock())
                throw std::runtime_error("clocks of lhs and rhs of fold don't match");
            
            this->emplace(std::move(lhs));
            this->emplace(std::move(rhs));
        }
        
        Fold(const Fold&) = delete;
        Fold(Fold&&) = default;
        
        //! Virtual destructor, because this is a polymorphic base class
        virtual ~Fold() = default;
        
        //! Add a new input to the fold
        void emplace(Value<In> input)
        {
            inputs.emplace_back(std::make_unique<Value<In>>(std::move(input)));
        }
        
        //! Change the amount of inputs
        void resize(std::size_t size)
        {
            const auto oldSize = inputs.size();
            inputs.resize(size);
            
            for (auto i = oldSize; i < size; ++i)
                inputs[i] = std::make_unique<Value<In>>();
        }
        
        //! Retrieve one of the inputs
        Value<In>& getInput(std::size_t index) { return *inputs.at(index); }
        
        //! Return the number of inputs
        std::size_t getInputCount() const { return inputs.size(); }
        
    private:
        //! Generate a new sample
        void generateSample(Out& out) final override
        {
            if (inputs.empty())
                out = {};
            else
                out = std::accumulate(inputs.begin(), inputs.end(), init(), [&](auto& a, auto& b){ return fold(a, (*b)()); });
        }
        
        //! Return the initial value of the fold
        /*! The initial value with which input samples will be combined.
            For an addition fold this would be 0, because 0 + x = x.
            For a multiplication fold this would be 1, because 1 * x = x */
        virtual Out init() const = 0;
        
        //! Combine an input sample with an output state sample, effectively folding it in
        /*! This funtion is called on each input, where the output is given as the first argument to the next call.
            It 'accumulates' all input samples in a single sample of type Out. */
        virtual Out fold(const Out& out, const In& in) const = 0;
        
    private:
        //! The inputs to the fold
        std::vector<std::unique_ptr<Value<In>>> inputs;
    };
}

#endif
