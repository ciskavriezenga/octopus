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

#ifndef OCTOPUS_NODE_HPP
#define OCTOPUS_NODE_HPP

namespace octo
{
    class Group;
    class SignalBase;
    
    //! A node is either a SignalBase or a Group
    class Node
    {
    public:
        //! Construct the Node with a SignalBase
        Node(SignalBase& signalBase) : type(Type::SIGNAL_BASE), signalBase(&signalBase) { }
        
        //! Construct the Node with a Group
        Node(Group& group) : type(Type::GROUP), group(&group) { }
        
        //! Is this node a SignalBase?
        bool isSignalBase() const { return type == Type::SIGNAL_BASE; }
        
        //! Is this node a Group?
        bool isGroup() const { return type == Type::GROUP; }
        
        //! Retrieve the SignalBase referenced by this Node
        /*! Undefined behaviour if the Node doesn't refer to a SignalBase */
        SignalBase& asSignalBase() { return *signalBase; }
        
        //! Retrieve the SignalBase referenced by this Node
        /*! Undefined behaviour if the Node doesn't refer to a SignalBase */
        const SignalBase& asSignalBase() const { return *signalBase; }
        
        //! Retrieve the Group referenced by this Node
        /*! Undefined behaviour if the Node doesn't refer to a Group */
        Group& asGroup() { return *group; }
        
        //! Retrieve the Group referenced by this Node
        /*! Undefined behaviour if the Node doesn't refer to a Group */
        const Group& asGroup() const { return *group; }

    private:
        //! Type enum to tag the type of the Node
        enum class Type { SIGNAL_BASE, GROUP } type;
        
        union
        {
            SignalBase* signalBase; //!< Address of the SignalBase if this Node represents one
            Group* group; //!< Address of the Group if this Node represents one
        };
    };
    
    //! Compare two nodes for equality
    static inline bool operator==(const Node& lhs, const Node& rhs)
    {
        return (lhs.isSignalBase() && rhs.isSignalBase() && (&lhs.asSignalBase() == &rhs.asSignalBase())) ||
               (lhs.isGroup() && rhs.isGroup() && (&lhs.asGroup() == &rhs.asGroup()));
    }
    
    //! Compare two nodes for inequality
    static inline bool operator!=(const Node& lhs, const Node& rhs)
    {
        return !(lhs == rhs);
    }
}

#endif
