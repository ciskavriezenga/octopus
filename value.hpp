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

#ifndef OCTOPUS_VALUE_HPP
#define OCTOPUS_VALUE_HPP

#include <cassert>
#include <memory>
#include <mutex>
#include <set>
#include <stdexcept>

#include "signal.hpp"

namespace octo
{
    enum class ValueMode { CONSTANT, REFERENCE, INTERNAL };
    
    //! Signal that depends on an external factor to generate data
    /*! Values are without a doubt the most used signals in Octopus. While being signals themselves, they
        don't generate new sample data out of their own. Values can be set to refer to other signals, reusing
        their output as transparent pass-throughs. Values can also be given a constant value of T, which they
        will then output as a DC.
     
        The most common use for Value objects is as inputs of actual signals:
        @code{cpp}
        class Sine : public Signal<float>
        {
        public:
            Value<float> frequency;
     
        private:
            void generateSample(float& out)
            {
                ...
                // Request the frequency at the current sample
                auto f = frequency();
                ...
            }
        };
     
        Sine sine;
     
        // Plug the sine output into its frequency
        sine.frequency = sine;
        @endcode */
    template <class T>
    class Value : public Signal<T>
    {
    public:
        class Listener;
        
    public:
        //! Construct a value with constant value
        Value(const T& constant = T{}) :
            Signal<T>(nullptr, constant),
            mode(ValueMode::CONSTANT),
            constant(constant)
        {
            
        }
        
        //! Construct a value referencing another signal
        Value(Signal<T>& reference) :
            Signal<T>(reference.getClock(), reference()),
            mode(ValueMode::REFERENCE),
            reference(&reference)
        {
            reference.dependees.emplace(this);
        }
        
        //! Reference another Value
        /*! This overload is necessary, because otherwise the deleted copy constructor is selected */
        Value(Value& reference) : Value(dynamic_cast<Signal<T>&>(reference)) { }
        
        //! Construct a value owning an internal signal
        Value(Signal<T>&& internal) : Value(std::move(internal).moveToHeap()) { }
        
        //! Construct a value owning an internal signal
        Value(std::unique_ptr<Signal<T>> internal) :
            Signal<T>(internal->getClock(), internal->pull()),
            mode(ValueMode::INTERNAL),
            internal(std::move(internal))
        {
            if (!this->internal)
                throw std::invalid_argument("value cannot contain a nullptr internal signal");
        }
        
        //! Copying a Value is forbidden
        Value(const Value&) = delete;
        
        //! Moving from a value
        Value(Value&& rhs) :
            Signal<T>(nullptr, rhs())
        {
            if (&rhs == this)
                return;
            
            switch (mode = rhs.mode)
            {
                case ValueMode::CONSTANT:
                    new (&constant) T(rhs.constant);
                    break;
                case ValueMode::REFERENCE:
                    reference = rhs.reference;
                    setClock(reference->getClock());
                    break;
                case ValueMode::INTERNAL:
                    new (&internal) std::unique_ptr<Signal<T>>(std::move(rhs.internal));
                    setClock(internal->getClock());
                    break;
            }
                        
            rhs.reset();
        }
        
        //! Destruct the value and release any contained data
        ~Value() { reset(); assert(listeners.empty()); }
        
        //! Assign a new constant to the value
        Value& operator=(const T& constant)
        {
            {
                std::unique_lock<std::mutex> lock(mutex);
                deconstruct();
                
                mode = ValueMode::CONSTANT;
                new (&this->constant) T(constant);
                setClock(nullptr);
            }
            
            notifyConstantSet();
            
            return *this;
        }
        
        //! Have the value reference another signal
        Value& operator=(Signal<T>& reference)
        {
            if (isReference() && this->reference == &reference)
                return *this;
            
            {
                std::unique_lock<std::mutex> lock(mutex);
                deconstruct();
                
                mode = ValueMode::REFERENCE;
                new (&this->reference) Signal<T>*(&reference);
                
                reference.dependees.emplace(this);
                setClock(reference.getClock());
            }
            
            notifySignalSet();
            
            return *this;
        }
        
        //! Have the value contain another signal
        Value& operator=(Signal<T>&& internal)
        {
            return *this = std::move(internal).moveToHeap();
        }
        
        //! Have the value contain another signal
        Value& operator=(std::unique_ptr<Signal<T>> internal)
        {
            if (!internal)
                throw std::invalid_argument("nullptr given to Value");
            
            {
                std::unique_lock<std::mutex> lock(mutex);
                deconstruct();
                
                mode = ValueMode::INTERNAL;
                new (&this->internal) std::unique_ptr<Signal<T>>(std::move(internal));
                setClock(internal->getClock());
            }
            
            notifySignalSet();
            
            return *this;
        }
        
        //! Copying is forbidden
        Value& operator=(const Value&) = delete;
        
        //! Moving from another Value
        Value& operator=(Value&& rhs)
        {
            if (&rhs == this)
                return *this;
            
            if (mode != rhs.mode ||
                (isConstant() && constant != rhs.constant) ||
                (isReference() && reference != rhs.reference))
            {
                std::unique_lock<std::mutex> lock(mutex);
                deconstruct();
                switch ((mode = rhs.mode))
                {
                    case ValueMode::CONSTANT: new (&constant) T(rhs.constant); setClock(nullptr); break;
                    case ValueMode::REFERENCE: reference = rhs.reference; setClock(rhs.reference); break;
                    case ValueMode::INTERNAL: new (&internal) std::unique_ptr<Signal<T>>(std::move(rhs.internal)); setClock(internal->getClock()); break;
                }
            }
            
            // We're notifying in a different switch, so that we can unlock the mutex earlier
            switch (mode)
            {
                case ValueMode::CONSTANT: notifyConstantSet(); break;
                case ValueMode::REFERENCE: notifyConstantSet(); break;
                case ValueMode::INTERNAL: notifyConstantSet(); break;
            }
            
            rhs.reset();
            
            return *this;
        }
        
        //! Reference another Value
        /*! This overload is necessary, because otherwise the deleted copy assignment op is selected */
        Value& operator=(Value& reference) { return *this = dynamic_cast<Signal<T>&>(reference); }
        
        //! Reset a signal to its unconnected state
        void reset()
        {
            *this = T{};
        }
        
        //! Is this value a constant?
        bool isConstant() const noexcept { return mode == ValueMode::CONSTANT; }
        
        //! Is this value a reference?
        bool isReference() const noexcept { return mode == ValueMode::REFERENCE; }
        
        //! Is this value an internal signal?
        bool isInternal() const noexcept { return mode == ValueMode::INTERNAL; }
        
        //! Return the constant value this object will output
        /*! @throw std::runtime_error if the value is not a constant */
        const T& getConstant() const
        {
            if (!isConstant())
                throw std::runtime_error("value is not constant");
            
            return constant;
        }
        
        //! Return a reference to the contained/referenced signal
        /*! @throw std::runtime_error if the value is a constant */
        Signal<T>& getReference() const
        {
            switch (mode)
            {
                case ValueMode::CONSTANT: throw std::runtime_error("value is constant");
                case ValueMode::REFERENCE: return *reference;
                case ValueMode::INTERNAL: return *internal;
            }
        }
        
        GENERATE_MOVE(Value)
        
    public:
        //! A collection of listeners for Value events
        std::set<Listener*> listeners;
        
    private:
        using Sink::setClock;
                              
        //! Deconstructs what's in the union
        /*! Internal use only. A new construction should take place immediately afterwards */
        void deconstruct()
        {
            switch (mode)
            {
                case ValueMode::CONSTANT:
                    constant.~T();
                    break;
                case ValueMode::REFERENCE:
                    reference->dependees.erase(this);
                    reference = nullptr;
                    break;
                case ValueMode::INTERNAL:
                    internal.~unique_ptr();
                    break;
            }
        }
        
        //! Generate a new sample
        void generateSample(T& out) final override
        {
            std::unique_lock<std::mutex> lock(mutex);
            switch (mode)
            {
                case ValueMode::CONSTANT: out = constant; break;
                case ValueMode::REFERENCE: out = (*reference)(); break;
                case ValueMode::INTERNAL: out = (*internal)(); break;
            }
        }
        
        //! Reset the value, because the referenced signal will be destructed
        void disconnectFromDependent(SignalBase& dependent) final override
        {
            assert(reference == &dependent);
            reset();
        }
        
        void notifyConstantSet()
        {
            const auto temp = listeners;
            for (auto& listener : temp)
                listener->setToConstant(*this, constant);
        }
        
        void notifySignalSet()
        {
            const auto temp = listeners;
            for (auto& listener : temp)
                listener->setToSignal(*this, isReference() ? *reference : *internal);
        }
        
    private:
        //! The mode state which the value is in
        ValueMode mode = ValueMode::CONSTANT;
        
        //! The union that stores the value data
        union
        {
            //! Holds static value if mode == ValueMode::VALUE
            T constant;
            
            //! Holds non-owned signal pointer if mode == ValueMode::NON_OWNED
            Signal<T>* reference;
            
            //! Holds owned signal unique_ptr if mode == ValueMode::OWNED
            std::unique_ptr<Signal<T>> internal;
        };
        
        //! A mutex for updating the value reference
        std::mutex mutex;
    };
    
    //! Listener for events that happen to a Value
    template <class T>
    class Value<T>::Listener
    {
        friend class Value<T>;
        
    public:
        //! Virtual destructor, because this is a polymorphic base class
        virtual ~Listener() = default;
        
    protected:
        //! Called when the value is set to a constant
        virtual void setToConstant(Value<T>& value, const T& constant) = 0;
        
        //! Called when the value is set to a signal
        virtual void setToSignal(Value<T>& value, Signal<T>& signal) = 0;
    };
    
    //! Compare two values for equality
    template <class T>
    bool operator==(const Value<T>& lhs, const Value<T>& rhs)
    {
        if (lhs.isConstant() && rhs.isConstant())
            return lhs.getConstant() == rhs.getConstant();
        
        return &lhs.getReference() == rhs.getReference();
    }
    
    //! Compare two values for inequality
    template <class T>
    bool operator!=(const Value<T>& lhs, const Value<T>& rhs)
    {
        return !(lhs == rhs);
    }
}

#endif
