//
//  Group.hpp
//  octopus
//
//  Created by Stijn on 17/10/2016.
//
//

#ifndef OCTOPUS_GROUP_HPP
#define OCTOPUS_GROUP_HPP

#include <cstdint>
#include <set>
#include <string>
#include <unordered_map>

#include "Node.hpp"
#include "SignalBase.hpp"
#include "Signal.hpp"

namespace octo
{
    //! Multiple signals in one monolith object
    class Group : public Node
    {
    public:
        //! Virtual, because this is a polymorphic base class
        virtual ~Group() = default;
        
        //! Retrieve one of the outputs as a typeless signal base
        const SignalBase& getOutput(const std::string& name) const;
        
        //! Retrieve one of the outputs as a typed signal
        template <class T>
        const Signal<T>& getOutput(const std::string& name) const { return dynamic_cast<const Signal<T>&>(getOutput(name)); }
        
        //! Retrieve the number of outputs
        std::size_t getOutputCount() const { return outputs.size(); }
        
        //! Retrieve the names used as keys for the outputs
        /*! @note The order is undefined and you should not depend on it! */
        std::set<std::string> getOutputNames() const;
        
    protected:
        //! Add a new output
        void setOutput(const std::string& name, const SignalBase& signal);
        
        //! Remove one of the outputs
        void removeOutput(const std::string& name);
        
    private:
        //! The output signals of the group (could be values, sieves in the case of a split, etc.)
        std::unordered_map<std::string, const SignalBase*> outputs;
    };
}

#endif
