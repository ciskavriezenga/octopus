#ifndef OCTOPUS_VALUE_HPP
#define OCTOPUS_VALUE_HPP

#include <memory>
#include <stdexcept>

#include "Signal.hpp"

namespace octo
{
    enum class ValueMode { CONSTANT, REFERENCE, INTERNAL };
    
    //! Signal that depends on external factors to output its value
    /*! Value either takes its output from a constant variable, another referenced signal, or one it owns
        internally. It does not generate samples itself */
    template <class T>
    class Value : public Signal<T>
    {
    public:
        //! Construct a value with constant value
        Value(Clock& clock, const T& constant = {}) : Signal<T>(clock), mode(ValueMode::CONSTANT), constant(constant) { }
        
        //! Construct a value referencing another signal
        Value(Clock& clock, Signal<T>& reference) : Signal<T>(clock), mode(ValueMode::REFERENCE), reference(&reference) { }
        
        //! Construct a value referencing another signal
        Value(Signal<T>& reference) : Value(reference.getClock(), reference) { }
        
//        //! Reference another Value
//        /*! This overload is necessary, because otherwise the deleted copy constructor is selected */
//        Value(const Value& reference) : Value(dynamic_cast<Signal<T>&>(reference)) { }
        
        //! Construct a value owning an internal signal
        Value(Clock& clock, Signal<T>&& internal) : Signal<T>(clock), mode(ValueMode::INTERNAL), internal(std::move(internal).moveToHeap()) { }
        
        //! Construct a value owning an internal signal
        Value(Signal<T>&& internal) : Value(internal.getClock(), std::move(internal)) { }
        
        //! Construct a value owning an internal signal
        Value(Clock& clock, std::unique_ptr<Signal<T>> internal) : Signal<T>(clock), mode(ValueMode::INTERNAL), internal(std::move(internal)) { }
        
        //! Construct a value owning an internal signal
        Value(std::unique_ptr<Signal<T>> internal) : Value(internal->getClock(), std::move(internal)) { }
        
        //! Copying a Value is forbidden
        Value(const Value&) = delete;
        
        //! Moving from a value
        Value(Value&& rhs) : Signal<T>(rhs.getClock())
        {
            if (&rhs == this)
                return;
            
            if (mode != rhs.mode ||
                (isConstant() && constant != rhs.constant) ||
                (isReference() && reference != rhs.reference))
            {
                switch ((mode = rhs.mode))
                {
                    case ValueMode::CONSTANT:
                        new (&constant) T(rhs.constant);
                        if (onConstantSet)
                            onConstantSet(constant);
                        break;
                    case ValueMode::REFERENCE:
                        reference = rhs.reference;
                        if (onSignalSet)
                            onSignalSet(*reference);
                        break;
                    case ValueMode::INTERNAL:
                        new (&internal) std::unique_ptr<Signal<T>>(std::move(rhs.internal));
                        if (onSignalSet)
                            onSignalSet(*internal);
                        break;
                }
            }
            
            rhs.reset();
        }
        
        //! Destruct the value and release any contained data
        ~Value() { reset(); }
        
        //! Assign a new constant to the value
        Value& operator=(const T& constant)
        {
            if (isConstant() && this->constant == constant)
                return *this;
            
            deconstruct();
            
            mode = ValueMode::CONSTANT;
            new (&this->constant) T(constant);
            
            if (onConstantSet)
                onConstantSet(constant);
            
            return *this;
        }
        
        //! Have the value reference another signal
        Value& operator=(Signal<T>& reference)
        {
            if (isReference() && this->reference == &reference)
                return *this;
            
            deconstruct();
            
            mode = ValueMode::REFERENCE;
            new (&this->reference) Signal<T>*(&reference);
            
            reference.dependencies.emplace(this);
            
            if (onSignalSet)
                onSignalSet(*this->reference);
            
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
            
            deconstruct();
            
            mode = ValueMode::INTERNAL;
            new (&this->internal) std::unique_ptr<Signal<T>>(std::move(internal));
            
            if (onSignalSet)
                onSignalSet(*this->internal);
            
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
                deconstruct();
                switch ((mode = rhs.mode))
                {
                    case ValueMode::CONSTANT:
                        new (&constant) T(rhs.constant);
                        if (onConstantSet)
                            onConstantSet(constant);
                        break;
                    case ValueMode::REFERENCE:
                        reference = rhs.reference;
                        if (onSignalSet)
                            onSignalSet(*reference);
                        break;
                    case ValueMode::INTERNAL:
                        new (&internal) std::unique_ptr<Signal<T>>(std::move(rhs.internal));
                        if (onSignalSet)
                            onSignalSet(*internal);
                        break;
                }
            }
            
            rhs.reset();
            
            return *this;
        }
        
//        //! Reference another Value
//        /*! This overload is necessary, because otherwise the deleted copy assignment op is selected */
//        Value& operator=(Value& reference) { return *this = dynamic_cast<Signal<T>&>(reference); }
        
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
        std::function<void(const T&)> onConstantSet; //!< Called when the value is assigned a new constant
        std::function<void(Signal<T>&)> onSignalSet; //!< Called when the value is assigned a new signal
        
    private:
        void deconstruct()
        {
            switch (mode)
            {
                case ValueMode::CONSTANT:
                    constant.~T();
                    break;
                case ValueMode::REFERENCE:
                    reference->dependencies.erase(this);
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
            switch (mode)
            {
                case ValueMode::CONSTANT: out = constant; break;
                case ValueMode::REFERENCE: out = (*reference)[0]; break;
                case ValueMode::INTERNAL: out = (*internal)[0]; break;
            }
        }
        
        //! Reset the value, because the referenced signal will be destructed
        void dependentWillBeDestructed(Node& dependent) final override
        {
            assert(reference == &dependent);
            reset();
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
