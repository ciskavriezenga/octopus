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

#ifndef OCTOPUS_CORE_NOT_EQUAL_HPP
#define OCTOPUS_CORE_NOT_EQUAL_HPP

#include <octopus/signal.hpp>
#include <octopus/value.hpp>

namespace octo
{
	//! Returns a value if left != right and another value if it's false
	template <typename T>
	class NotEqual : public Signal<T>
	{
	public:
		using Signal<T>::Signal;
        
    public:
        //! The left-side value of the not-equal operator
        Value<float> left = 0;
        
        //! The right-side value of the not-equal operator
        Value<float> right = 0;
        
        //! The value to be returned when true
        Value<float> trueValue = 1;
        
        //! The value to be returned when false
        Value<float> falseValue = 0;

	private:
		//! Generate the next sample
		void generateSample(T& y) final override
		{
            const auto& t = trueValue();
            const auto& f = falseValue();
            y = (left() != right()) ? t : f;
		}
	};
}

#endif
