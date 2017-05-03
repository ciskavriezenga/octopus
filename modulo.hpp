#ifndef OCTOPUS_CORE_MODULO_HPP
#define OCTOPUS_CORE_MODULO_HPP

#include <cmath>
#include <octopus/binary_operation.hpp>
#include <octopus/value.hpp>

namespace octo
{
    // Modulo
    template <typename T>
    class Modulo : public BinaryOperation<T>
    {
    public:
        using BinaryOperation<T>::BinaryOperation;
        
    private:
        /// Inherited from BinaryOperation
        void combineSamples(const T& left, const T& right, T& out) final override
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
