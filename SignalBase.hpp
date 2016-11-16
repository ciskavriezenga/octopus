#ifndef OCTOPUS_SIGNAL_BASE_HPP
#define OCTOPUS_SIGNAL_BASE_HPP

#include <cstdint>
#include <set>
#include <typeinfo>
#include <unit/discrete.hpp>

namespace octo
{
    //! Base class for all signals in Octopus, regardless of their output type
    class SignalBase
    {
    public:
        //! Virtual destructor, because this is a polymorphic base class
        virtual ~SignalBase();
        
        //! Retrieve the type info of the output
        virtual const std::type_info& getTypeInfo() const = 0;
        
        //! Pull and retrieve the type-agnostic address of a sample
        /*! The address is invalidated next time someone pulls the signal */
        virtual const void* pull() = 0;
        
    public:
        //! The dependencies of this signal
        std::set<SignalBase*> dependencies;
        
    private:
        //! Notify dependencies this signal will be destructed
        virtual void dependentWillBeDestructed(SignalBase& dependent) { }
    };
}

#endif
