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

#ifndef OCTOPUS_GROUP_HPP
#define OCTOPUS_GROUP_HPP

#include <cstdint>
#include <set>
#include <string>
#include <unordered_map>

#include "SignalBase.hpp"
#include "Signal.hpp"

namespace octo
{
    //! Multiple signals in one monolith object
    /*! Groups contains one or more output signals, functioning as it were as a multi-output
        signal, without being Signals themselves. Patches are a good example, as well as Split,
        which contains a Sieve per channel, without Split itself being a signal. 
     
        When requesting outputs from a Group, it is up to the user to know the sample type
        of the output. */
    class Group
    {
    public:
        //! Virtual, because this is a polymorphic base class
        virtual ~Group() = default;
        
        //! Retrieve one of the outputs as a typeless SignalBase
        /*! If you know the sample type of a particalar output, it is easier to call the templated
            version of this function.
            @throw std::runtime_error: If no output with this name exists */
        SignalBase& getOutput(const std::string& name);
        
        //! Retrieve one of the outputs as a typed Signal
        /*! @throw std::runtime_error: If no output with this name exists */
        template <class T>
        Signal<T>& getOutput(const std::string& name) { return dynamic_cast<Signal<T>&>(getOutput(name)); }
        
        //! Retrieve the number of outputs
        std::size_t getOutputCount() const { return outputs.size(); }
        
        //! Retrieve the names of all outputs
        /*! @note The order is undefined and you should not depend on it! */
        std::set<std::string> getOutputNames() const;
        
    protected:
        //! Add a new output
        /*! Derivatives can call this function to add an output to the group */
        void addOutput(const std::string& name, SignalBase& signal);
        
        //! Remove one of the outputs
        /*! Derivatives can call this function to remove an output from the group */
        void removeOutput(const std::string& name);
        
    private:
        //! The output signals of the group (could be values, sieves in the case of a split, etc.)
        std::unordered_map<std::string, SignalBase*> outputs;
    };
}

#endif
