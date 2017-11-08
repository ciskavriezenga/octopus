/*
 
 This file is a part of Octopus, a modern C++ library for embedding digital
 signal processing as a language inside your software. It transcends a single
 domain (audio, video, math, etc.), combining multiple clocks in one graph.
 
 Copyright (C) 2017 Dsperados <info@dsperados.com>
 
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

#ifndef OCTOPUS_MODULO_HPP
#define OCTOPUS_MODULO_HPP

#include <cmath>

#include "binary_operation.hpp"

namespace octo
{
    // Modulo
    template <typename T>
    class Modulo : public BinaryOperation<T>
    {
    public:
        Modulo(Clock* clock) :
            BinaryOperation<T>(clock, 0, 1)
        {
            
        }
        
    private:
        /// Inherited from BinaryOperation
        void combineSamples(const T& left, const T& right, T& out) final
        {
            if (right == 0)
            {
                out = 0;
                return;
            }
            
            out = std::fmod(left, right);
        }
    };
}

#endif
