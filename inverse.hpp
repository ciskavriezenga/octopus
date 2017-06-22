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

#ifndef OCTOPUS_INVERSE_HPP
#define OCTOPUS_INVERSE_HPP

#include "unary_operation.hpp"

namespace octo
{
    //! Invert a signal
    template <class In, class Out = In>
    class Inverse : public UnaryOperation<In, Out>
    {
    public:
        // Use the constructor from UnaryOperation
        using UnaryOperation<In, Out>::UnaryOperation;
        
    private:
        //! Generate a negative sample
        void convertSample(const In& in, Out& out) final
        {
            if (in == In(0))
                out = Out(0);
            else
                out = std::common_type_t<In, Out>(1) / in;
        }
    };
}

#endif
