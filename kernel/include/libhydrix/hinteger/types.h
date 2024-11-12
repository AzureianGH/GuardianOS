#pragma once
#include <stdint.h>
#include <libhydrix/libhydrix.h>
class IBaseInheritableObject
{
    public:
        /// @brief Constructor
        virtual void Ctor() = 0;
        /// @brief Destructor
        virtual void Dtor() = 0;
        /// @brief The size of the object
        virtual size_t Size() = 0;
        /// @brief The type of the object
        virtual const char* Type() = 0;
};

class IStringableObject
{
    public:
        /// @brief Convert the object to a string
        virtual const char* ToString() = 0;
};

class Int8 : public IBaseInheritableObject, public IStringableObject
{
    private:
        int8_t value;
    public:

        Int8(int8_t value)
        {
            this->value = value;
        }
        /// @brief Constructor
        void Ctor() override
        {
            value = 0;
        }
        /// @brief Destructor
        void Dtor()
        {
            value = '\0';
        }
        /// @brief The size of the object
        size_t Size()
        {
            return sizeof(value);
        }
        /// @brief The type of the object
        const char* Type()
        {
            return "Int8";
        }
        /// @brief Convert the object to a string
        const char* ToString()
        {
            return ToString(value);
        }
};