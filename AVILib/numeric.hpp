#ifndef AVILIB_USED_NUMERIC
#pragma once 

//#include <cstdint>
#include <bitset>
#include <string>
#include <cstring>
#include <type_traits>
// #include <iostream>
// #include <utility>
// #include <type_traits>
// #include <errno.h>

/**
 * @brief Namespace of Alternative Vector (And pair... and shared_ptr... and other things?) Implementation Library.
 * 
 */
namespace AVIL
{
    /**
     * @brief Unsigned numeric type of any size of bits. Recommended to not use on numbers smaller than 9 bits.
     * 
     * @tparam size Number of bits.
     */
    template<size_t size = 128>
    struct auint_t
    {
        private:

        std::bitset<size> itself;

        constexpr void invert()
        {
            for(std::size_t i = 0; i < size/2; ++i)
            {
                bool temp = itself[i];
                itself[i] = itself[size - i - 1];
                itself[size - i - 1] = temp;
            }
        }

        constexpr size_t highestSet() const
        {
            size_t i = 0;
            for(i = size; i > 0; --i)
            {
                if(itself.test(i - 1)) return i - 1;
            }
            return i;
        }

        constexpr inline static char digitToCharacter(const unsigned short& digit)
        {
            return '0' + digit;
        }

        constexpr inline static unsigned short characterToDigit(const char& character)
        {
            return (short)(character - '0');
        }

        public:
        auint_t() = default;

        constexpr size_t digits() const
        {
            if(*this == 0) return 1;
            size_t count = 0;
            auint_t<size> checkedNumber = *this;
            while(checkedNumber != 0)
            {
                checkedNumber /= 10;
                ++count;
            }
            return count;
        }

        /**
         * @brief Get digit as if number was read from right to left.
         * 
         * @param index Placement of digit.
         * @return unsigned short Digit itself. 
         */
        constexpr unsigned short digitrl(const size_t& index) const
        {
            auint_t<size> newNumber = *this;
            for(size_t i = 0; i < index; ++i)
            {
                newNumber /= 10;
            }
            newNumber %= 10;
            return newNumber.itself.to_ulong();
        }

        /**
         * @brief Get digit as if number was read from left to right.
         * 
         * @param index Placement of digit.
         * @return unsigned short Digit itself. 
         */
        constexpr inline unsigned short digitlr(const size_t& index) const
        {
            return digitrl(digits() - index);
        }

        // std::bitset<size> max() const
        // {
        //     std::bitset<size> returned;
        //     returned.set(true);
        //     return returned;
        // }

        // std::bitset<size> min() const
        // {
        //     std::bitset<size> returned;
        //     returned.set(false);
        //     return returned;
        // }

        constexpr inline auint_t(const unsigned long long& copied) : itself{copied}
        {
            //invert();
            //itself <<= (size - (sizeof(unsigned long long) * 8));
        }

        constexpr auint_t& operator=(const unsigned long long& copied)
        {
            itself = {copied};
            //invert();
            //itself <<= (size - (sizeof(unsigned long long) * 8));
            return *this;
        }

        auint_t(const std::bitset<size>& copied) : itself{copied} {}

        template<size_t bits>
        constexpr auint_t(const auint_t<bits>& copied)
        {
            for(size_t i = 0; i < ((size > bits)?(bits):(size)); ++i)
            {
                itself[i] = ((std::bitset<bits>)copied)[i];
            }
        }

        template<size_t bits>
        constexpr auint_t& operator=(const auint_t<bits>& copied)
        {
            for(size_t i = 0; i < ((size > bits)?(bits):(size)); ++i)
            {
                itself[i] = ((std::bitset<bits>)copied)[i];
            }
            return *this;
        }

        auint_t(const auint_t&) = default;

        auint_t(auint_t&&) = default;

        auint_t& operator=(const auint_t&) = default;

        auint_t& operator=(auint_t&&) = default;

        constexpr bool operator==(const auint_t& compared) const
        {
            return itself == compared.itself;
        }

        constexpr bool operator!=(const auint_t& compared) const
        {
            return itself != compared.itself;
        }

        constexpr bool operator>(const auint_t& compared) const
        {
            //for(size_t i = 0; i < size; ++i)
            for(size_t i = size; i > 0; --i)
            {
                if(itself[i - 1] and (!compared.itself[i - 1]))
                {
                    return true;
                }
                if((!itself[i - 1]) and compared.itself[i - 1])
                {
                    return false;
                }
            }
            return false;
        }

        constexpr bool operator<(const auint_t& compared) const
        {
            // for(size_t i = 0; i < size; ++i)
            for(size_t i = size; i > 0; --i)
            {
                if((!itself[i - 1]) and compared.itself[i - 1])
                {
                    return true;
                }
                if(itself[i - 1] and (!compared.itself[i - 1]))
                {
                    return false;
                }
            }
            return false;
        }

        constexpr bool operator>=(const auint_t& compared) const
        {
            // for(size_t i = 0; i < size; ++i)
            for(size_t i = size; i > 0; --i)
            {
                if(itself[i - 1] and (!compared.itself[i - 1]))
                {
                    return true;
                }
                if((!itself[i - 1]) and compared.itself[i - 1])
                {
                    return false;
                }
            }
            return true;
        }

        constexpr bool operator<=(const auint_t& compared) const
        {
            // for(size_t i = 0; i < size; ++i)
            for(size_t i = size; i > 0; --i)
            {
                if((!itself[i - 1]) and compared.itself[i - 1])
                {
                    return true;
                }
                if(itself[i - 1] and (!compared.itself[i - 1]))
                {
                    return false;
                }
            }
            return true;
        }

        constexpr auint_t operator+()
        {
            return {itself};
        }

        constexpr auint_t operator&(const auint_t& processed)
        {
            return itself & processed.itself;
        }

        constexpr auint_t operator|(const auint_t& processed)
        {
            return itself | processed.itself;
        }

        constexpr auint_t operator^(const auint_t& processed)
        {
            return itself ^ processed.itself;
        }

        constexpr auint_t operator~()
        {
            return ~itself;
        }

        constexpr auint_t operator&=(const auint_t& processed)
        {
            itself &= processed.itself;
            return *this;
        }

        constexpr auint_t operator|=(const auint_t& processed)
        {
            itself |= processed.itself;
            return *this;
        }

        constexpr auint_t operator^=(const auint_t& processed)
        {
            itself ^= processed.itself;
            return *this;
        }

        constexpr auint_t& operator+=(auint_t processed)
        {
            size_t more = 0;
            // for(size_t i = size - 1; i > 0; --i)
            // bool carry = false;
            for(size_t i = 0; i < size; ++i)
            {
                if(more > 0)
                {
                    if(!itself[i]) --more;
                    itself[i] = !itself[i];
                }
                if(itself[i] && processed.itself[i]) ++more;
                itself[i] = itself[i] ^ processed.itself[i];
                // itself[i] = (bool)((itself[i] ^ processed.itself[i]) ^ carry);
                // carry = (((bool)itself[i] && processed.itself[i]) || ((bool)itself[i] && carry) || ((bool)processed.itself[i] && carry));
            }
            return *this;
        }

        constexpr auint_t& operator-=(auint_t processed)
        {
            size_t more = 0;
            //for(size_t i = size - 1; i > 0; --i)
            for(size_t i = 0; i < size; ++i)
            {
                if(more > 0)
                {
                    if(itself[i]) --more;
                    itself[i] = !itself[i];
                }
                if((!itself[i]) && processed.itself[i]) ++more;
                itself[i] = itself[i] ^ processed.itself[i];
            }
            return *this;
        }

        constexpr auint_t& operator*=(auint_t processed)
        {
            std::bitset<size> temporary = itself;
            itself.reset();
            if(temporary.count() < processed.itself.count())
            {
                for(size_t i = 0; i < size; ++i)
                {
                    if(temporary[i])
                    {
                        *this += (processed.itself << i);
                    }
                }
            }
            else
            {
                for(size_t i = 0; i < size; ++i)
                {
                    if(processed.itself[i])
                    {
                        *this += (temporary << i);
                    }
                }
            }
            // auint_t resolution = 0;
            // auint_t power = processed;
            // while(processed > 0)
            // {
            //     if((power % 2) != 0)
            //     {
            //         resolution += *this;
            //     }
            //     power >>= 2;
            //     *this <<= 2;
            // }
            // *this = resolution;
            return *this;
        }

        constexpr inline auint_t distance(auint_t number)
        {
            // if(*this > number)
            // {
            //     return *this - number;
            // }
            // else
            // {
            //     return number - *this;
            // }
            return (*this > number)?(*this - number):(number - *this);
        }

        // auint_t& operator/=(const auint_t& processed)
        // {
        //     if(processed == 0)
        //     {
        //         throw(EINVAL);
        //     }
        //     auint_t quotient = 0;
        //     auint_t temporaryProcessed = processed;

        //     while(true)
        //     {
        //         auint_t temporaryQuotient = 1;
        //         if (temporaryProcessed == *this)
        //         {
        //             quotient += 1;
        //             break;
        //         }
        //         else if (*this < temporaryProcessed)
        //         {
        //             break;
        //         }
        //         while ((temporaryProcessed << 1) <= *this)
        //         {
        //             temporaryProcessed <<= 1;
        //             temporaryQuotient <<= 1;
        //         }

        //         *this -= temporaryProcessed;
        //         temporaryProcessed = processed;
        //         quotient += temporaryQuotient;
        //     }

        //     itself = quotient.itself;
            
        //     return *this;
        // }

        constexpr auint_t& operator/=(const auint_t& processed)
        {
            if(processed == 0)
            {
                throw(EINVAL);
            }
            auint_t quotient = 0;
            auint_t<size + 1> temporaryProcessed = processed;

            while(true)
            {
                auint_t temporaryQuotient = 1;
                if (temporaryProcessed == *this)
                {
                    quotient += 1;
                    break;
                }
                else if (*this < temporaryProcessed)
                {
                    break;
                }
                while ((temporaryProcessed << 1) <= *this)
                {
                    temporaryProcessed <<= 1;
                    temporaryQuotient <<= 1;
                }

                *this -= temporaryProcessed;
                temporaryProcessed = processed;
                quotient += temporaryQuotient;
            }

            itself = quotient.itself;
            // itself = (std::bitset<size>)((auint_t<size>)quotient);
            
            return *this;
        }

        constexpr auint_t& operator%=(auint_t processed)
        {
            // auint_t<size> remainder;
            // for(remainder = itself; remainder >= processed; remainder -= processed) {}
            // itself = remainder.itself;
            const size_t first = highestSet();
            const size_t second = processed.highestSet();
            if(second > first) return *this;
            if(second == first && (*this < processed)) return *this;

            const size_t shift = first - second;
            processed <<= shift;

            for(size_t i = 0; i <= shift; ++i)
            {
                if(*this == processed)
                {
                    itself = 0;
                    return *this;
                }
                if(*this > processed)
                {
                    *this -= processed;
                }
                processed >>= 1;
            }

            return *this;
        }

        constexpr inline auint_t operator+(const auint_t& processed)
        {
            auint_t<size> returned = *this;
            returned += processed;
            return returned;
        }

        constexpr inline auint_t operator-(const auint_t& processed)
        {
            auint_t<size> returned = *this;
            returned -= processed;
            return returned;
        }

        constexpr inline auint_t operator*(const auint_t& processed)
        {
            auint_t<size> returned = *this;
            returned *= processed;
            return returned;
        }

        constexpr inline auint_t operator/(const auint_t& processed)
        {
            auint_t<size> returned = *this;
            returned /= processed;
            return returned;
        }

        constexpr auint_t pow(const auint_t& power)
        {
            auint_t<size> additional = *this;
            auint_t<size> resolution = 1;
            auint_t copiedPower = power;
            while(copiedPower > 0)
            {
                if((copiedPower & 1) == 1)
                //if((copiedPower % 2) != 0)
                {
                    resolution *= additional;
                }
                additional *= additional;
                copiedPower >>= 1;
                // copiedPower /= 2;
            }
            return resolution;
        }

        constexpr inline auint_t operator%(const auint_t& processed)
        {
            auint_t<size> returned = *this;
            returned %= processed;
            return returned;
        }

        constexpr inline auint_t& operator++()
        {
            return *this += 1;
        }

        constexpr inline auint_t& operator--()
        {
            return *this -= 1;
        }

        constexpr inline auint_t operator++(int)
        {
            *this += 1;
            return *this + 1;
        }

        constexpr inline auint_t operator--(int)
        {
            *this -= 1;
            return *this - 1;
        }

        constexpr inline auint_t& operator>>=(size_t number)
        {
            itself >>= number;
            return *this;
        }

        constexpr inline auint_t& operator<<=(size_t number)
        {
            itself <<= number;
            return *this;
        }

        constexpr inline auint_t operator>>(size_t number)
        {
            auint_t<size> returned = *this;
            returned >>= number;
            return returned;
        }

        constexpr inline auint_t operator<<(size_t number)
        {
            auint_t<size> returned = *this;
            returned <<= number;
            return returned;
        }

        // operator std::bitset<size>() const
        // {
        //     return itself;
        // }

        constexpr inline explicit operator std::bitset<size>&()
        {
            return itself;
        }

        // operator unsigned long long()
        // {
        //     return itself.to_ullong();
        // }

        // template<size_t bits>
        // explicit operator std::bitset<bits>() const
        // {
        //     return itself;
        // }

        constexpr inline explicit operator const std::bitset<size>&() const
        {
            return itself;
        }

        // explicit auint_t(const char* const copied)
        // {
        //     *this = 0;
        //     for(size_t i = 0; i < strlen(copied); ++i)
        //     {
        //         *this += (auint_t<size>{10}).pow(strlen(copied) - i - 1) * auint_t<size>{characterToDigit(copied[i])};
        //     }
        // }

        constexpr auint_t(const std::string& copied)
        {
            *this = 0;
            for(size_t i = 0; i < copied.size(); ++i)
            {
                *this += (auint_t<size>{10}).pow(copied.size() - i - 1) * auint_t<size>{characterToDigit(copied[i])};
            }
            // *this /= 2;
        }

        constexpr auint_t& operator=(const std::string& copied)
        {
            *this = 0;
            for(size_t i = 0; i < copied.size(); ++i)
            {
                *this += (auint_t<size>{10}).pow(copied.size() - i - 1) * auint_t<size>{characterToDigit(copied[i])};
            }
            // *this /= 2;
            return *this;
        }

        operator std::string() const
        {
            if(*this == 0)
            {
                return "0";
            }
            std::string returned; returned.resize(digits());
            auint_t<size> worked = itself;
            for(size_t i = 0; worked != 0; ++i)
            {
                returned[i] = digitToCharacter(((worked % 10).itself.to_ulong()));
                worked /= 10;
            }
            // return returned;
            return {returned.rbegin(), returned.rend()};
        }

        constexpr explicit inline operator bool()
        {
            return !(itself.none());
        }

        ~auint_t() = default;

        constexpr inline bool isEven() const
        {
            return !itself[0];
        }

        constexpr inline bool isOdd() const
        {
            return itself[0];
        }

        template<size_t bits>
        constexpr operator auint_t<bits>()
        {
            std::bitset<bits> returned;
            for(size_t i = 0; i < ((size > bits)?(bits):(size)); ++i)
            {
                returned[i] = itself[i];
            }
            return {returned};
        }
    };

    template<size_t size>
    constexpr inline auint_t<size> pow(const auint_t<size>& base, const auint_t<size>& power)
    {
        return base.pow(power);
    }

    template<size_t size>
    constexpr auint_t<size> maxauint_t()
    {
        std::bitset<size> returned;
        // returned.set();
        for(size_t i = 0; i < size; ++i) returned.set(i, true); // returned[i] = true;
        return {returned};
    }
    
    template<size_t size>
    constexpr auint_t<size> minauint_t()
    {
        std::bitset<size> returned;
        // returned.reset();
        for(size_t i = 0; i < size; ++i) returned.set(i, false); // returned[i] = false;
        return {returned};
    }

    template<size_t size>
    std::string bitauint_t(const auint_t<size>& processed)
    {
        std::string returned;
        returned.resize(size);
        for(size_t i = 0; i < size; ++i)
        {
            returned[i] = (((std::bitset<size>)processed)[size - i - 1])?('1'):('0');
            
        }
        return returned;
    }
};
#define AVILIB_USED_NUMERIC 1
#endif
