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

#ifndef OCTOPUS_LIST_HPP
#define OCTOPUS_LIST_HPP

#include <vector>

#include "dirty.hpp"

namespace octo
{
    //! Template specialization of Dirty for std::vectors
    template <typename T>
    class Dirty<std::vector<T>>
    {
    public:
        Dirty& operator=(const std::vector<T>& rhs)
        {
            data = rhs;
            dirty = true;
            return *this;
        }
        
        Dirty& operator=(std::vector<T>&& rhs)
        {
            data = std::move(rhs);
            dirty = true;
            return *this;
        }
        
        T& operator[](std::size_t index) { return data[index % data.size()]; }
        const T& operator[](std::size_t index) const { return data[index % data.size()]; }
        
        void clear()
        {
            if (data.empty())
                return;
            
            data.clear();
            dirty = true;
        }
        
        bool empty() const { return data.empty(); }
        std::size_t size() const { return data.size(); }
        
        auto begin() { return data.begin(); }
        auto begin() const { return data.begin(); }
        auto cbegin() const { return data.cbegin(); }
        auto end() { return data.end(); }
        auto end() const { return data.end(); }
        auto cend() const { return data.cend(); }
        
    public:
        std::vector<T> data;
        bool dirty = true;
    };
    
    //! A Dirty of std::vector is also called a List
    template <typename T>
    using List = Dirty<std::vector<T>>;
}

#endif /* OCTOPUS_LIST_HPP */
