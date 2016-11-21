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
#ifndef OCTOPUS_CROSS_FADE_HPP
#define OCTOPUS_CROSS_FADE_HPP

#include <dsperados/math/ease.hpp>
#include <dsperados/math/utility.hpp>
#include <functional>

#include "BinaryOperation.hpp"

namespace octo
{
    //! Cross fade between two signals
    template <class T>
    class CrossFade : public BinaryOperation<T>
    {
    public:
        //! Construct the cross fade
        CrossFade(octo::Clock& clock) :
            BinaryOperation<T>(clock),
            fade(clock)
        {

        }
        
        GENERATE_MOVE(CrossFade)
        
    public:
        //! The fading value (0 = left, 1 = right)
        Value<float> fade;
        
        //! The function that will be used for easing between silence and full-volume
        std::function<float(float)> ease = [](float v){ return math::easeCosine(v); };
        
    private:
        // Inherited from BinaryOperation
        void combineSamples(const T& left, const T& right, T& out) final override
        {
            if (!ease)
            {
                out = T{};
            } else {
                const auto phase = math::clamp<float>(fade(), 0, 1);
                out = ease(1 - phase) * left + ease(phase) * right;
            }
        }
    };
}

#endif
