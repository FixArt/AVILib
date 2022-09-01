#include <cmath>
#include <stdexcept>
#ifndef AVILIB_USED_NUMERIC
#pragma once 

//#include <cstdint>
#include <bitset>
#include <string>
#include <cstring>
#include <type_traits>
// #include <exception>
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
     * @brief Unsigned numeric type of any size of bits.
     * 
     * @tparam size Number of bits.
     */
    template<size_t size = 128>
    struct auint_t
    {
        private:

        std::bitset<size> itself;

        constexpr void invert() noexcept
        {
            for(std::size_t i = 0; i < size/2; ++i)
            {
                bool temp = itself[i];
                itself[i] = itself[size - i - 1];
                itself[size - i - 1] = temp;
            }
        }

        constexpr size_t highestSet() const noexcept
        {
            size_t i = 0;
            for(i = size; i > 0; --i)
            {
                if(itself.test(i - 1)) return i - 1;
            }
            return i;
        }

        constexpr inline static char digitToCharacter(const unsigned short& digit) noexcept
        {
            return '0' + digit;
        }

        constexpr inline static unsigned short characterToDigit(const char& character) noexcept
        {
            return (short)(character - '0');
        }

        public:
        auint_t() = default;

        constexpr size_t digits() const noexcept
        {
            if(*this == 0 || size < 4) return 1;
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
        constexpr unsigned short digitrl(const size_t& index) const noexcept
        {
            if(size < 4)
            {
                const unsigned short tested = itself.to_ulong();
                switch(tested)
                {
                    default: return 0;
                    case 1: return 1;
                    case 2: return 2;
                    case 3: return 3;
                    case 4: return 4;
                    case 5: return 5;
                    case 6: return 6;
                    case 7: return 7;
                }
            }
            auint_t<size> newNumber = *this;
            for(size_t i = 0; i < (index + 1); ++i)
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
        constexpr inline unsigned short digitlr(const size_t& index) const noexcept
        {
            return digitrl(digits() - 1 - index);
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

        // template<typename = typename std::enable_if<(size <= (sizeof(unsigned long long) * 8))>>
        // constexpr inline operator unsigned long long() const
        // {
        //     return itself.to_ullong();
        // }

        template<typename = typename std::enable_if<(size >= (sizeof(unsigned long long) * 8))>>
        constexpr inline auint_t(const unsigned long long& copied) noexcept : itself{copied}
        {
            //invert();
            //itself <<= (size - (sizeof(unsigned long long) * 8));
        }

        template<typename = typename std::enable_if<(size >= (sizeof(unsigned long long) * 8))>>
        constexpr auint_t& operator=(const unsigned long long& copied) noexcept
        {
            itself = {copied};
            //invert();
            //itself <<= (size - (sizeof(unsigned long long) * 8));
            return *this;
        }

        template<typename = typename std::enable_if<(size >= (sizeof(unsigned long) * 8) && size < (sizeof(unsigned long long) * 8))>>
        constexpr inline auint_t(const unsigned long& copied) noexcept : itself{copied}
        {
            //invert();
            //itself <<= (size - (sizeof(unsigned long long) * 8));
        }

        template<typename = typename std::enable_if<(size >= (sizeof(unsigned long) * 8) && size < (sizeof(unsigned long long) * 8))>>
        constexpr auint_t& operator=(const unsigned long& copied) noexcept
        {
            itself = {copied};
            //invert();
            //itself <<= (size - (sizeof(unsigned long long) * 8));
            return *this;
        }

        template<typename = typename std::enable_if<(size >= (sizeof(unsigned int) * 8) && size < (sizeof(unsigned long) * 8))>>
        constexpr inline auint_t(const unsigned int& copied) noexcept : itself{copied}
        {
            //invert();
            //itself <<= (size - (sizeof(unsigned long long) * 8));
        }

        template<typename = typename std::enable_if<(size >= (sizeof(unsigned int) * 8) && size < (sizeof(unsigned long) * 8))>>
        constexpr auint_t& operator=(const unsigned int& copied) noexcept
        {
            itself = {copied};
            //invert();
            //itself <<= (size - (sizeof(unsigned long long) * 8));
            return *this;
        }

        template<typename = typename std::enable_if<(size >= (sizeof(unsigned short) * 8) && size < (sizeof(unsigned short) * 8))>>
        constexpr inline auint_t(const unsigned short& copied) noexcept : itself{copied}
        {
            //invert();
            //itself <<= (size - (sizeof(unsigned long long) * 8));
        }

        template<typename = typename std::enable_if<(size >= (sizeof(unsigned short) * 8) && size < (sizeof(unsigned short) * 8))>>
        constexpr auint_t& operator=(const unsigned short& copied) noexcept
        {
            itself = {copied};
            //invert();
            //itself <<= (size - (sizeof(unsigned long long) * 8));
            return *this;
        }

        template<typename = typename std::enable_if<(size >= (sizeof(unsigned char) * 8) && size < (sizeof(unsigned char) * 8))>>
        constexpr inline auint_t(const unsigned char& copied) noexcept : itself{copied}
        {
            //invert();
            //itself <<= (size - (sizeof(unsigned long long) * 8));
        }

        template<typename = typename std::enable_if<(size >= (sizeof(unsigned char) * 8) && size < (sizeof(unsigned char) * 8))>>
        constexpr auint_t& operator=(const unsigned char& copied) noexcept
        {
            itself = {copied};
            //invert();
            //itself <<= (size - (sizeof(unsigned long long) * 8));
            return *this;
        }

        template<typename = typename std::enable_if<(size >= (sizeof(long long) * 8))>>
        constexpr inline auint_t(const long long& copied) noexcept : itself{(unsigned long long)((copied >= 0)?(copied):(-copied))}
        {
            //invert();
            //itself <<= (size - (sizeof(long long) * 8));
        }

        template<typename = typename std::enable_if<(size >= (sizeof(long long) * 8))>>
        constexpr auint_t& operator=(const long long& copied) noexcept
        {
            itself = {(unsigned long long)((copied >= 0)?(copied):(-copied))};
            //invert();
            //itself <<= (size - (sizeof(long long) * 8));
            return *this;
        }

        template<typename = typename std::enable_if<(size >= (sizeof(long) * 8) && size < (sizeof(long long) * 8))>>
        constexpr inline auint_t(const long& copied) noexcept : itself{(unsigned long)((copied >= 0)?(copied):(-copied))}
        {
            //invert();
            //itself <<= (size - (sizeof(long long) * 8));
        }

        template<typename = typename std::enable_if<(size >= (sizeof(long) * 8) && size < (sizeof(long long) * 8))>>
        constexpr auint_t& operator=(const long& copied) noexcept
        {
            itself = {(unsigned long)((copied >= 0)?(copied):(-copied))};
            //invert();
            //itself <<= (size - (sizeof(long long) * 8));
            return *this;
        }

        template<typename = typename std::enable_if<(size >= (sizeof(int) * 8) && size < (sizeof(long) * 8))>>
        constexpr inline auint_t(const int& copied) noexcept : itself{(unsigned int)((copied >= 0)?(copied):(-copied))}
        {
            //invert();
            //itself <<= (size - (sizeof(long long) * 8));
        }

        template<typename = typename std::enable_if<(size >= (sizeof(int) * 8) && size < (sizeof(long) * 8))>>
        constexpr auint_t& operator=(const int& copied) noexcept
        {
            itself = {(unsigned int)((copied >= 0)?(copied):(-copied))};
            //invert();
            //itself <<= (size - (sizeof(long long) * 8));
            return *this;
        }

        template<typename = typename std::enable_if<(size >= (sizeof(short) * 8) && size < (sizeof(short) * 8))>>
        constexpr inline auint_t(const short& copied) noexcept : itself{(unsigned short)((copied >= 0)?(copied):(-copied))}
        {
            //invert();
            //itself <<= (size - (sizeof(long long) * 8));
        }

        template<typename = typename std::enable_if<(size >= (sizeof(short) * 8) && size < (sizeof(short) * 8))>>
        constexpr auint_t& operator=(const short& copied) noexcept
        {
            itself = {(unsigned short)((copied >= 0)?(copied):(-copied))};
            //invert();
            //itself <<= (size - (sizeof(long long) * 8));
            return *this;
        }

        template<typename = typename std::enable_if<(size >= (sizeof(char) * 8) && size < (sizeof(char) * 8))>>
        constexpr inline auint_t(const char& copied) noexcept : itself{(unsigned char)((copied >= 0)?(copied):(-copied))}
        {
            //invert();
            //itself <<= (size - (sizeof(long long) * 8));
        }

        template<typename = typename std::enable_if<(size >= (sizeof(char) * 8) && size < (sizeof(char) * 8))>>
        constexpr auint_t& operator=(const char& copied) noexcept
        {
            itself = {(unsigned char)((copied >= 0)?(copied):(-copied))};
            //invert();
            //itself <<= (size - (sizeof(long long) * 8));
            return *this;
        }

        auint_t(const std::bitset<size>& copied) noexcept : itself{copied} {}

        template<size_t bits>
        constexpr auint_t(const auint_t<bits>& copied) noexcept
        {
            // itself.reset();
            // for(size_t i = 0; i < ((size > bits)?(bits):(size)); ++i)
            // {
            //     itself[i] = ((std::bitset<bits>)copied)[i];
            // }
            for(size_t i = 0; i < size; ++i)
            {
                if(i < bits) itself[i] = ((std::bitset<bits>)copied)[i];
                else itself.set(i, false);
            }
        }

        template<size_t bits>
        constexpr auint_t& operator=(const auint_t<bits>& copied) noexcept
        {
            // itself.reset();
            // for(size_t i = 0; i < ((size > bits)?(bits):(size)); ++i)
            // {
            //     itself[i] = ((std::bitset<bits>)copied)[i];
            // }
            for(size_t i = 0; i < size; ++i)
            {
                if(i < bits) itself[i] = ((std::bitset<bits>)copied)[i];
                else itself.set(i, false);
            }
            return *this;
        }

        auint_t(const auint_t&) = default;

        auint_t(auint_t&&) = default;

        auint_t& operator=(const auint_t&) = default;

        auint_t& operator=(auint_t&&) = default;

        constexpr bool operator==(const auint_t& compared) const noexcept
        {
            return itself == compared.itself;
        }

        constexpr bool operator!=(const auint_t& compared) const noexcept
        {
            return itself != compared.itself;
        }

        constexpr bool operator>(const auint_t& compared) const noexcept
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

        constexpr bool operator<(const auint_t& compared) const noexcept
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

        constexpr bool operator>=(const auint_t& compared) const noexcept
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

        constexpr bool operator<=(const auint_t& compared) const noexcept
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

        constexpr inline bool operator>(const intmax_t& compared) const noexcept
        {
            return (compared < 0)?(true):(*this > ((auint_t)compared));
        }

        constexpr inline bool operator<(const intmax_t& compared) const noexcept
        {
            return (compared < 0)?(false):(*this < ((auint_t)compared));
        }

        constexpr inline bool operator>=(const intmax_t& compared) const noexcept
        {
            return (compared < 0)?(true):(*this >= ((auint_t)compared));
        }

        constexpr inline bool operator<=(const intmax_t& compared) const noexcept
        {
            return (compared < 0)?(false):(*this <= ((auint_t)compared));
        }

        constexpr inline auint_t operator+() const noexcept
        {
            return {itself};
        }

        constexpr inline auint_t operator&(const auint_t& processed) const noexcept
        {
            return itself & processed.itself;
        }

        constexpr inline auint_t operator|(const auint_t& processed) const noexcept
        {
            return itself | processed.itself;
        }

        constexpr inline auint_t operator^(const auint_t& processed) const noexcept
        {
            return itself ^ processed.itself;
        }

        constexpr inline auint_t operator~() const noexcept
        {
            return ~itself;
        }

        constexpr inline auint_t operator&=(const auint_t& processed) noexcept
        {
            itself &= processed.itself;
            return *this;
        }

        constexpr inline auint_t operator|=(const auint_t& processed) noexcept
        {
            itself |= processed.itself;
            return *this;
        }

        constexpr inline auint_t operator^=(const auint_t& processed) noexcept
        {
            itself ^= processed.itself;
            return *this;
        }

        constexpr auint_t& operator+=(auint_t processed) noexcept
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

        constexpr auint_t& operator-=(auint_t processed) noexcept
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

        constexpr auint_t& operator*=(auint_t processed) noexcept
        {
            if(processed == 10)
            {
                *this = (*this << 3) + (*this << 1);
                return *this;
            }
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

        constexpr inline auint_t distance(auint_t number) const noexcept
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

        constexpr auint_t& operator/=(const auint_t& processed) noexcept
        {
            if(processed == 0)
            {
                // throw(std::overflow_error("Division by zero."));
                return *this = 0;
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

        constexpr auint_t& divideWithRemainder(const auint_t& processed, auint_t& remainder) noexcept
        {
            if(processed == 0)
            {
                // throw(std::overflow_error("Division by zero."));
                remainder = 0;
                return *this = 0;
            }
            auint_t quotient = 0;
            auint_t<size + 1> temporaryProcessed = processed;

            while(true)
            {
                auint_t temporaryQuotient = 1;
                if (temporaryProcessed == *this)
                {
                    remainder = 0;
                    quotient += 1;
                    break;
                }
                else if (*this < temporaryProcessed)
                {
                    remainder = *this;
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

        constexpr auint_t& operator%=(auint_t processed) noexcept
        {
            // auint_t<size> remainder;
            // for(remainder = itself; remainder >= processed; remainder -= processed) {}
            // itself = remainder.itself;
            const size_t first = highestSet();
            const size_t second = processed.highestSet();
            if(second > first) return *this;
            if(second == first && (*this < processed)) return *this;
            if(first > second) return *this = 0;

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

        constexpr inline auint_t<size> mod(const auint_t<size>& number) const noexcept
        {
            return *this % number;
        }

        constexpr inline auint_t<size> mod2() const noexcept
        {
            return (itself[0])?(1):(0);
        }

        constexpr inline auint_t<size> mod10() const noexcept
        {
            return *this % 10;
        }

        constexpr auint_t<size> log(const auint_t<size>& previous) const noexcept
        {
            // if(*this == 1)
            // {
            //     return (previous == 1)?(1):(0);
            // }
            // if(previous > *this)
            // {
            //     throw(EINVAL);
            // }
            // // auint_t<size> returned = 1;
            // auint_t<size> returned = 1;
            // bool beforeLast = (previous.pow(returned) > *this)?(true):(false);
            // bool lastOperation = beforeLast;
            // while(previous.pow(returned) != *this)
            // {
            //     if(previous.pow(returned) > *this)
            //     {
            //         --returned;
            //         if(lastOperation && !beforeLast) break;
            //         beforeLast = lastOperation;
            //         lastOperation = false;
            //     }
            //     else
            //     {
            //         ++returned;
            //         if(!lastOperation && beforeLast) break;
            //         beforeLast = lastOperation;
            //         lastOperation = true;
            //     }
            // }
            // return returned;
            if(*this == 1)
            {
                return (previous == 1)?(1):(0);
            }
            if(previous > *this)
            {
                return 0;
            }
            auint_t high, low;
            low = 1;
            high = *this >> 1;
            // high = sqrt(); // May be too slow.
            auint_t guess = (low + high) >> 1;
            while(previous.pow(guess) != *this)
            {
                if(previous.pow(guess) > *this)
                {
                    if(high == guess) break;
                    high = guess;
                }
                else
                {
                    if(low == guess) break;
                    low = guess;
                }
                guess = (low + high) >> 1;
            }
            return guess;
        }

        constexpr inline auint_t log2() const noexcept
        {
            return log(2);
        }

        constexpr inline auint_t log10() const noexcept
        {
            return log(10);
        }

        constexpr inline auint_t operator+(const auint_t& processed) const noexcept
        {
            auint_t<size> returned = *this;
            returned += processed;
            return returned;
        }

        constexpr inline auint_t operator-(const auint_t& processed) const noexcept
        {
            auint_t<size> returned = *this;
            returned -= processed;
            return returned;
        }

        constexpr inline auint_t operator*(const auint_t& processed) const noexcept
        {
            auint_t<size> returned = *this;
            returned *= processed;
            return returned;
        }

        constexpr inline auint_t operator/(const auint_t& processed) const noexcept
        {
            auint_t<size> returned = *this;
            returned /= processed;
            return returned;
        }

        constexpr auint_t pow(const auint_t& power) const noexcept
        {
            if(power == 0) return 1;
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

        constexpr auint_t sqrt() const noexcept
        {
            auint_t<size> temporary, returned, preturned;
            preturned.itself.set(0, (returned.itself[0])?(false):(true));
            returned = *this >> 1;
            temporary = 0;
            while(returned != temporary && preturned != returned)
            {
                temporary = returned;
                // auint_t calculation = (*this / temporary + temporary) >> 1;
                // if(calculation == temporary) break;
                // returned = calculation;
                // if(returned == (*this / temporary + temporary) / 2) break;
                if(returned.itself[0]) preturned = returned;
                returned = (*this / temporary + temporary) >> 1;
            }
            return returned;
        }

        constexpr auint_t root(auint_t<size> power) const noexcept
        {
            if(power == 2) return sqrt();
            // auint_t<size> low, high, plow, phigh;
            auint_t<size> low, high;
            low = (*this == 0)?(0):(1);
            high = (*this == 0)?(1):(*this);

            // // Any fast operation, which will make them differ. (initialization of all values is unneeded)
            // plow.itself.set(0, (low.itself[0])?(false):(true));
            // phigh.itself.set(0, (high.itself[0])?(false):(true));

            auint_t<size> guess = (low + high) >> 1;
            // while((guess.pow(power) - *this) > 0 and (high != phigh && low != plow))
            while((guess.pow(power) - *this) > 0)
            {
                if(guess.pow(power) > *this)
                {
                    // phigh = high;
                    if(high == guess) break;
                    high = guess;
                }
                else
                {
                    // plow = low;
                    if(low == guess) break;
                    low = guess;
                }
                guess = (low + high) >> 1;
            }

            return guess;
        }

        // /**
        //  * @brief Very broken exponent function.
        //  * 
        //  * @return constexpr auint_t Returned value.
        //  */
        // constexpr inline auint_t exp()
        // {
        //     auint_t<(size > 15)?(size):(16)> e = 27183;
        //     return (auint_t<size>)(e.pow((auint_t<(size > 15)?(size):(16)>)*this) / (((auint_t<(size > 15)?(size):(16)>)10000).pow((auint_t<(size > 15)?(size):(16)>)*this)));
        // }

        constexpr auint_t factorial() const noexcept
        {
            auint_t returned = 1;
            for(auint_t i = 2; i <= *this; ++i)
            {
                returned *= i;
            }
            return returned;
        }

        constexpr inline auint_t operator%(const auint_t& processed) const noexcept
        {
            auint_t<size> returned = *this;
            returned %= processed;
            return returned;
        }

        constexpr inline auint_t& operator++() noexcept
        {
            return *this += 1;
        }

        constexpr inline auint_t& operator--() noexcept
        {
            return *this -= 1;
        }

        constexpr inline auint_t operator++(int) noexcept
        {
            *this += 1;
            return *this + 1;
        }

        constexpr inline auint_t operator--(int) noexcept
        {
            *this -= 1;
            return *this - 1;
        }

        constexpr inline auint_t& operator>>=(size_t number) noexcept
        {
            itself >>= number;
            return *this;
        }

        constexpr inline auint_t& operator<<=(size_t number) noexcept
        {
            itself <<= number;
            return *this;
        }

        constexpr inline auint_t operator>>(size_t number) const noexcept
        {
            auint_t<size> returned = *this;
            returned >>= number;
            return returned;
        }

        constexpr inline auint_t operator<<(size_t number) const noexcept
        {
            auint_t<size> returned = *this;
            returned <<= number;
            return returned;
        }

        // operator std::bitset<size>() const
        // {
        //     return itself;
        // }

        constexpr inline explicit operator std::bitset<size>() noexcept
        {
            return itself;
        }

        constexpr inline explicit operator const std::bitset<size>&() const noexcept
        {
            return itself;
        }

        // template<size_t bits>
        // explicit operator std::bitset<bits>() const
        // {
        //     return itself;
        // }

        // explicit auint_t(const char* const copied)
        // {
        //     *this = 0;
        //     for(size_t i = 0; i < strlen(copied); ++i)
        //     {
        //         *this += (auint_t<size>{10}).pow(strlen(copied) - i - 1) * auint_t<size>{characterToDigit(copied[i])};
        //     }
        // }

        constexpr auint_t(const std::string& copied) noexcept
        {
            *this = 0;
            for(size_t i = 0; i < copied.size(); ++i)
            {
                *this += (auint_t<size>{10}).pow(copied.size() - i - 1) * auint_t<size>{characterToDigit(copied[i])};
            }
            // *this /= 2;
        }

        constexpr auint_t& operator=(const std::string& copied) noexcept
        {
            *this = 0;
            for(size_t i = 0; i < copied.size(); ++i)
            {
                *this += (auint_t<size>{10}).pow(copied.size() - i - 1) * auint_t<size>{characterToDigit(copied[i])};
            }
            // *this /= 2;
            return *this;
        }

        operator std::string() const noexcept
        {
            // if(*this == 0)
            // {
            //     return "0";
            // }
            // if(size < 4)
            // {
            //     return {digitToCharacter(digitrl(0))};
            // }
            // std::string returned; returned.resize(digits());
            // auint_t<size> worked = itself;
            // for(size_t i = 0; worked != 0; ++i)
            // {
            //     returned[i] = digitToCharacter(((worked % 10).itself.to_ulong()));
            //     worked /= 10;
            // }
            // return {returned.rbegin(), returned.rend()};
            if(*this == 0)
            {
                return "0";
            }
            if(size < 4)
            {
                return {digitToCharacter(digitrl(0))};
            }
            else if(size < 64)
            {
                std::string returned; returned.resize(digits());
                auint_t<size> worked = itself;
                for(size_t i = 0; worked != 0; ++i)
                {
                    returned[i] = digitToCharacter(((worked % 10).itself.to_ulong()));
                    worked /= 10;
                }
                return {returned.rbegin(), returned.rend()};
            }
            else
            {
                std::string returned; // returned.resize(numberSize);
                auint_t<size> worked = itself;
                for(size_t i = 0; worked != 0; i += 19)
                {
                    // returned.insert(0, std::to_string((worked % 10000000000000000000ULL).itself.to_ullong()));
                    // worked /= 10000000000000000000ULL;
                    auint_t<size> numbers;
                    worked.divideWithRemainder(10000000000000000000ULL, numbers);
                    returned.insert(0, std::to_string(numbers.itself.to_ullong()));
                }
                return returned;
            }
            }

        constexpr explicit inline operator bool() const noexcept
        {
            return !(itself.none());
        }

        ~auint_t() = default;

        constexpr inline bool isEven() const noexcept
        {
            return !itself[0];
        }

        constexpr inline bool isOdd() const noexcept
        {
            return itself[0];
        }

        constexpr bool isPrime() const noexcept
        {
            if(*this == 2) return true;
            if(*this < 2)
            {
                return false;
            }
            if(isEven()) return false;
            for(auint_t<size> i = 2; i <= sqrt(); ++i)
            {
                if(*this % i == 0)
                {
                    return false;
                }
            }
            return true;
        }

        constexpr bool isPowerOf2() const noexcept
        {
            bool found = false;
            for(bool checked : itself)
            {
                if(checked)
                {
                    if(found)
                    {
                        return false;
                    }
                    found = true;
                }
            }
            return found;
        }

        template<size_t bits>
        constexpr operator auint_t<bits>() const noexcept
        {
            std::bitset<bits> returned;
            for(size_t i = 0; i < ((size > bits)?(bits):(size)); ++i)
            {
                returned[i] = itself[i];
            }
            return {returned};
        }

        constexpr bool isAdditionOverflow(const auint_t& processed) const noexcept
        {
            return *this > (auint_t(std::bitset<size>{}.set()) - processed);
        }

        constexpr bool isSubtractionOverflow(const auint_t& processed) const noexcept
        {
            return *this < processed;
        }

        constexpr bool isMultiplicationOverflow(const auint_t& processed) const noexcept
        {
            return (processed > 1) && (*this > (auint_t(std::bitset<size>{}.set()) / processed));
        }

        constexpr bool isFactorialOverflow(const auint_t& processed) const noexcept
        {
            auint_t returned = 1;
            for(auint_t i = 2; i <= *this; ++i)
            {
                if(returned.isMultiplicationOverflow(i)) return true;
                returned *= i;
            }
            return false;
        }

        /**
         * @brief Fills number with digits of Euler's number.
         * 
         */
        constexpr auint_t& e() noexcept
        {
            // itself = 2;
            // auint_t currentFactorial = 2;
            // auint_t currentMultiplicator = 1;
            // for(auint_t i = 3; (!currentFactorial.isMultiplicationOverflow(i)) && (!isAdditionOverflow((currentMultiplicator / currentFactorial))); ++i)
            // {
            //     while(currentFactorial > currentMultiplicator)
            //     {
            //         currentMultiplicator *= 10;
            //         *this *= 10;
            //     }
            //     *this += (currentMultiplicator / currentFactorial);
            //     std::cout << " += " << (std::string)currentMultiplicator << "/" << (std::string)currentFactorial << " = " << (std::string)(currentMultiplicator / currentFactorial) << "\n";
            //     currentFactorial *= i;
            // }
            // return *this;
            auint_t currentMultiplicator = 1;
            for(itself = 2; !isMultiplicationOverflow(10); *this *= 10) { currentMultiplicator *= 10; }
            auint_t currentFactorial = 2;
            for(auint_t i = 3; (!currentFactorial.isMultiplicationOverflow(i)) && (!isAdditionOverflow((currentMultiplicator / currentFactorial))); ++i)
            {
                *this += (currentMultiplicator / currentFactorial);
                currentFactorial *= i;
            }
            return *this;
        }
    };

    /**
     * @brief Considered as zero.
     * 
     */
    template<> struct auint_t<0>
    {
        inline operator std::string() const noexcept
        {
            return "0";
        }
    };

    typedef auint_t<0> auint0_t;

    // typedef auint;

    template<size_t size>
    constexpr inline auint_t<size> pow(const auint_t<size>& base, const auint_t<size>& power)
    {
        return base.pow(power);
    }

    template<size_t size>
    constexpr auint_t<size> randomauint_t(unsigned seed = 1)
    {
        srand(seed);
        std::bitset<size> returned;
        for(size_t processed = 0; processed < size; processed += sizeof(int))
        {
            std::bitset<sizeof(unsigned int)> current{(unsigned int)rand()};
            // returned.set(processed, current);
            for(size_t index = 0; index < sizeof(unsigned int) && (index + processed) < size; ++index)
            {
                returned.set(processed + index, current[index]);
            }
        }
        // size_t processed = 0;
        return returned;
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
    std::string base2auint_t(const auint_t<size>& processed)
    {
        if(size == 0) return "0";
        std::string returned;
        returned.resize(size);
        for(size_t i = 0; i < size; ++i)
        {
            returned[i] = (((std::bitset<size>)processed)[size - i - 1])?('1'):('0');
        }
        return returned;
    }

    template<size_t size>
    std::string base4auint_t(const auint_t<size>& processed)
    {
        if(size == 0) return "0";
        std::bitset<size> current = (std::bitset<size>)processed;
        std::string returned;
        returned.resize(size / 2 + ((size % 2 != 0)?(1):(0)));
        size_t written = 0;
        for(size_t i = 0; i < size; i += 2)
        {
            std::bitset<3> checked;
            checked.set(0, current[i]);
            checked.set(1, (i + 1 < size)?(current[i + 1]):(false));

            // switch(checked.to_ulong())
            // {
            //     default: returned[written] = ' '; break;
            //     case 0: returned[written] = '0'; break;
            //     case 1: returned[written] = '1'; break;
            //     case 2: returned[written] = '2'; break;
            //     case 3: returned[written] = '3'; break;
            //     case 4: returned[written] = '4'; break;
            //     case 5: returned[written] = '5'; break;
            //     case 6: returned[written] = '6'; break;
            //     case 7: returned[written] = '7'; break;
            // }
            returned[written] = '0' + (char)checked.to_ullong();

            ++written;
        }
        returned = {returned.rbegin(), returned.rend()};
        while(returned[0] == '0')
        {
            returned.erase(0, 1);
        }
        return returned;
    }

    template<size_t size>
    std::string base8auint_t(const auint_t<size>& processed)
    {
        if(size == 0) return "0";
        std::bitset<size> current = (std::bitset<size>)processed;
        std::string returned;
        returned.resize(size / 3 + ((size % 3 != 0)?(1):(0)));
        size_t written = 0;
        for(size_t i = 0; i < size; i += 3)
        {
            std::bitset<3> checked;
            checked.set(0, current[i]);
            checked.set(1, (i + 1 < size)?(current[i + 1]):(false));
            checked.set(2, (i + 2 < size)?(current[i + 2]):(false));

            // switch(checked.to_ulong())
            // {
            //     default: returned[written] = ' '; break;
            //     case 0: returned[written] = '0'; break;
            //     case 1: returned[written] = '1'; break;
            //     case 2: returned[written] = '2'; break;
            //     case 3: returned[written] = '3'; break;
            //     case 4: returned[written] = '4'; break;
            //     case 5: returned[written] = '5'; break;
            //     case 6: returned[written] = '6'; break;
            //     case 7: returned[written] = '7'; break;
            // }
            returned[written] = '0' + (char)checked.to_ullong();

            ++written;
        }
        returned = {returned.rbegin(), returned.rend()};
        while(returned[0] == '0')
        {
            returned.erase(0, 1);
        }
        return returned;
    }

    template<size_t size>
    std::string base10auint_t(const auint_t<size>& processed)
    {
        return (std::string)processed;
    }

    template<size_t size>
    std::string base16auint_t(const auint_t<size>& processed)
    {
        if(size == 0) return "0";
        std::bitset<size> current = (std::bitset<size>)processed;
        std::string returned;
        returned.resize(size / 4 + ((size % 4 != 0)?(1):(0)));
        size_t written = 0;
        for(size_t i = 0; i < size; i += 4)
        {
            std::bitset<4> checked;
            checked.set(0, current[i]);
            checked.set(1, (i + 1 < size)?(current[i + 1]):(false));
            checked.set(2, (i + 2 < size)?(current[i + 2]):(false));
            checked.set(3, (i + 3 < size)?(current[i + 3]):(false));

            // switch(checked.to_ulong())
            // {
            //     default: returned[written] = ' '; break;
            //     case 0: returned[written] = '0'; break;
            //     case 1: returned[written] = '1'; break;
            //     case 2: returned[written] = '2'; break;
            //     case 3: returned[written] = '3'; break;
            //     case 4: returned[written] = '4'; break;
            //     case 5: returned[written] = '5'; break;
            //     case 6: returned[written] = '6'; break;
            //     case 7: returned[written] = '7'; break;
            // }
            // if(checked.to_ullong() < 10) returned[written] = '0' + (char)checked.to_ullong();
            // else switch(checked.to_ulong())
            // {
            //     default: returned[written] = ' '; break;
            //     case 10: returned[written] = 'A'; break;
            //     case 11: returned[written] = 'B'; break;
            //     case 12: returned[written] = 'C'; break;
            //     case 13: returned[written] = 'D'; break;
            //     case 14: returned[written] = 'E'; break;
            //     case 15: returned[written] = 'F'; break;
            // };
            const std::string symbols = "0123456789ABCDEF";
            returned[written] = symbols[checked.to_ulong()];

            ++written;
        }
        returned = {returned.rbegin(), returned.rend()};
        while(returned[0] == '0')
        {
            returned.erase(0, 1);
        }
        return returned;
    }

    template<size_t size>
    std::string base32auint_t(const auint_t<size>& processed)
    {
        if(size == 0) return "A";
        std::bitset<size> current = (std::bitset<size>)processed;
        std::string returned;
        returned.resize(size / 5 + ((size % 5 != 0)?(1):(0)));
        size_t written = 0;
        for(size_t i = 0; i < size; i += 5)
        {
            std::bitset<5> checked;
            checked.set(0, current[i]);
            checked.set(1, (i + 1 < size)?(current[i + 1]):(false));
            checked.set(2, (i + 2 < size)?(current[i + 2]):(false));
            checked.set(3, (i + 3 < size)?(current[i + 3]):(false));
            checked.set(4, (i + 4 < size)?(current[i + 4]):(false));
            
            const std::string symbols = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
            returned[written] = symbols[checked.to_ulong()];

            ++written;
        }
        returned = {returned.rbegin(), returned.rend()};
        while(returned[0] == 'A')
        {
            returned.erase(0, 1);
        }
        return returned;
    }

    template<size_t size>
    std::string zbase32auint_t(const auint_t<size>& processed)
    {
        if(size == 0) return "y";
        std::bitset<size> current = (std::bitset<size>)processed;
        std::string returned;
        returned.resize(size / 5 + ((size % 5 != 0)?(1):(0)));
        size_t written = 0;
        for(size_t i = 0; i < size; i += 5)
        {
            std::bitset<5> checked;
            checked.set(0, current[i]);
            checked.set(1, (i + 1 < size)?(current[i + 1]):(false));
            checked.set(2, (i + 2 < size)?(current[i + 2]):(false));
            checked.set(3, (i + 3 < size)?(current[i + 3]):(false));
            checked.set(4, (i + 4 < size)?(current[i + 4]):(false));
            
            const std::string symbols = "ybndrfg8ejkmcpqxot1uwisza345h769";
            returned[written] = symbols[checked.to_ulong()];

            ++written;
        }
        returned = {returned.rbegin(), returned.rend()};
        while(returned[0] == 'y')
        {
            returned.erase(0, 1);
        }
        return returned;
    }

    template<size_t size>
    std::string base32hexauint_t(const auint_t<size>& processed)
    {
        if(size == 0) return "0";
        std::bitset<size> current = (std::bitset<size>)processed;
        std::string returned;
        returned.resize(size / 5 + ((size % 5 != 0)?(1):(0)));
        size_t written = 0;
        for(size_t i = 0; i < size; i += 5)
        {
            std::bitset<5> checked;
            checked.set(0, current[i]);
            checked.set(1, (i + 1 < size)?(current[i + 1]):(false));
            checked.set(2, (i + 2 < size)?(current[i + 2]):(false));
            checked.set(3, (i + 3 < size)?(current[i + 3]):(false));
            checked.set(4, (i + 4 < size)?(current[i + 4]):(false));
            
            const std::string symbols = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
            returned[written] = symbols[checked.to_ulong()];

            ++written;
        }
        returned = {returned.rbegin(), returned.rend()};
        while(returned[0] == '0')
        {
            returned.erase(0, 1);
        }
        return returned;
    }

    template<size_t size>
    std::string geohashauint_t(const auint_t<size>& processed)
    {
        if(size == 0) return "0";
        std::bitset<size> current = (std::bitset<size>)processed;
        std::string returned;
        returned.resize(size / 5 + ((size % 5 != 0)?(1):(0)));
        size_t written = 0;
        for(size_t i = 0; i < size; i += 5)
        {
            std::bitset<5> checked;
            checked.set(0, current[i]);
            checked.set(1, (i + 1 < size)?(current[i + 1]):(false));
            checked.set(2, (i + 2 < size)?(current[i + 2]):(false));
            checked.set(3, (i + 3 < size)?(current[i + 3]):(false));
            checked.set(4, (i + 4 < size)?(current[i + 4]):(false));
            
            const std::string symbols = "0123456789bcdefghjkmnpqrstuvwxyz";
            returned[written] = symbols[checked.to_ulong()];

            ++written;
        }
        returned = {returned.rbegin(), returned.rend()};
        while(returned[0] == '0')
        {
            returned.erase(0, 1);
        }
        return returned;
    }

    /**
     * @brief Non base64 specification compliant conversion to base64 of number.
     * 
     * @tparam size Size of auint_t in bits.
     * @param processed Processed number.
     * @return std::string Returned string.
     */
    template<size_t size>
    std::string base64auint_t(const auint_t<size>& processed)
    {
        if(size == 0) return "A";
        std::bitset<size> current = (std::bitset<size>)processed;
        std::string returned;
        returned.resize(size / 6 + ((size % 6 != 0)?(1):(0)));
        size_t written = 0;
        for(size_t i = 0; i < size; i += 6)
        {
            std::bitset<6> checked;
            checked.set(0, current[i]);
            checked.set(1, (i + 1 < size)?(current[i + 1]):(false));
            checked.set(2, (i + 2 < size)?(current[i + 2]):(false));
            checked.set(3, (i + 3 < size)?(current[i + 3]):(false));
            checked.set(4, (i + 4 < size)?(current[i + 4]):(false));
            checked.set(5, (i + 5 < size)?(current[i + 5]):(false));

            const std::string symbols = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
            returned[written] = symbols[checked.to_ulong()];

            ++written;
        }
        returned = {returned.rbegin(), returned.rend()};
        while(returned[0] == 'A')
        {
            returned.erase(0, 1);
        }
        return returned;
    }

    template<size_t size>
    constexpr inline auint_t<size> powbase2(const size_t& power)
    {
        return ((auint_t<size>)2) << power;
    }
};
#define AVILIB_USED_NUMERIC 1
#endif
