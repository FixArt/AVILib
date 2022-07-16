#include <cmath>
#include <cstdlib>
#include <initializer_list>

/**
 * @brief Namespace of Alternative Vector Implementation Library.
 * 
 */
namespace AVIL
{
    /**
     * @brief Vector implementation.
     * 
     * @tparam type Type of array.
     */
    template<class type>
    struct vector
    {
        protected:
            type* array = nullptr;
            size_t arraySize = 0;
        public:

            vector<type>() = default;

            const size_t &size = arraySize;

            /**
             * @brief Default value for new vector elements.
             * 
             */
            type standart;

            vector<type>(const type& selectedStandart)
            {
                standart = selectedStandart;
            }

            vector<type>& operator=(std::initializer_list<type> initializationArray)
            {
                
                for(type newElement : initializationArray)
                {
                    append(newElement);
                }
                return *this;
            }

            // explicit vector<type>(std::initializer_list<type> initializationArray)
            // {
            //     
            //     for(type newElement : initializationArray)
            //     {
            //         append(newElement);
            //     }
            // }

            /**
             * @brief Checks whenever given index currently exists.
             * 
             * @param index Index to check.
             * @return true Yes, index exists.
             * @return false No, index wasn't allocated yet.
             */
            bool exists(const size_t& index) const
            {
                if(array == nullptr) return false;
                return index >= size;
            }

            /**
             * @brief Add element to vector by index.
             * 
             * @param newElement Element itself.
             * @param index Index at which it will be placed.
             */
            void push(const type &newElement, const size_t &index)
            {
                if(index > size) return;
                type* newArray = (type*)malloc((size + 1) * sizeof(type));
                size_t writteni = 0;
                for(size_t i = 0; i < size; ++i)
                {
                    if(writteni == index)
                    {
                        newArray[writteni] = newElement;
                        --i;
                    }
                    else
                    {
                        newArray[writteni] = array[i];
                    }
                    ++writteni;
                }
                free(array);
                array = newArray;
                ++arraySize;
            }

            /**
             * @brief Exclude element from array by index
             * 
             * @param index Index, at which element will be excluded from array.
             */
            void pop(const size_t &index)
            {
                if(size - 1 == 0)
                {
                    
                    return;
                }
                if(index >= size or size == 0) return;
                //type* newArray = (type*)malloc((size - 1) * sizeof(type));
                size_t j = 0;
                for(size_t i = 0; i < size; ++i)
                {
                    if(index == i)
                    {
                        continue;
                    }
                    array[j] = array[i];
                    ++j;
                }
                //free(array);
                //array = newArray;
                //--arraySize;
                reduce(1);
            }

            /**
             * @brief Remove all instances which equal to given.
             * 
             * @param checkedElement Element searched.
             */
            void remove(const type& checkedElement)
            {
                if(size == 0)
                {
                    return;
                }
                if(size - 1 == 0)
                {
                    
                    return;
                }
                //type* newArray = (type*)malloc((size - 1) * sizeof(type));
                size_t j = 0;
                size_t allFound = 0;
                for(size_t i = 0; i < size; ++i)
                {
                    if(checkedElement == array[i])
                    {
                        ++allFound;
                        continue;
                    }
                    array[j] = array[i];
                    ++j;
                }
                //free(array);
                //array = newArray;
                //--arraySize;
                reduce(allFound);
            }

            /**
             * @brief Remove all instances which marked by given function.
             * 
             * @param shouldRemove Function to check whenever element should be removed.
             */
            void remove(bool(shouldRemove)(const type&))
            {
                if(size == 0)
                {
                    return;
                }
                if(size - 1 == 0)
                {
                    
                    return;
                }
                //type* newArray = (type*)malloc((size - 1) * sizeof(type));
                size_t j = 0;
                size_t allFound = 0;
                for(size_t i = 0; i < size; ++i)
                {
                    if(shouldRemove(array[i]))
                    {
                        ++allFound;
                        continue;
                    }
                    array[j] = array[i];
                    ++j;
                }
                //free(array);
                //array = newArray;
                //--arraySize;
                reduce(allFound);
            }

            /**
             * @brief Count all instances which equal to given.
             * 
             * @param checkedElement Element counted.
             * @return size_t Number of occurences.
             */
            size_t count(const type& checkedElement) const
            {
                size_t allFound = 0;
                for(size_t i = 0; i < size; ++i)
                {
                    if(checkedElement == array[i])
                    {
                        ++allFound;
                    }
                }
                return allFound;
            }

            /**
             * @brief Count all instances which considered good by given function.
             * 
             * @param shouldCount Function which will check if element should be counted.
             * @return size_t Number of occurences.
             */
            size_t count(bool(shouldCount)(const type&)) const
            {
                size_t allFound = 0;
                for(size_t i = 0; i < size; ++i)
                {
                    if(shouldCount(array[i]))
                    {
                        ++allFound;
                    }
                }
                return allFound;
            }

            /**
             * @brief Allows you to find where is specific element.
             * 
             * @param checkedElement Searched element.
             * @return size_t Placement. Size of vector, if no such element found.
             */
            size_t where(const type& checkedElement) const
            {
                for(size_t i = 0; i < size; ++i)
                {
                    if(checkedElement == array[i])
                    {
                        return i;
                    }
                }
                return size;
            }

            /**
             * @brief Allows you to find where is element which corresponds to the function condition.
             * 
             * @param shouldCount Function which defines which element we searching.
             * @return size_t Placement. Size of vector, if no such element found.
             */
            size_t where(bool(shouldCount)(const type&)) const
            {
                for(size_t i = 0; i < size; ++i)
                {
                    if(shouldCount(array[i]))
                    {
                        return i;
                    }
                }
                return size;
            }

            /**
             * @brief Allows you to find where is specific vector.
             * 
             * @param comparedVector Searched vector.
             * @return size_t Placement. Size of vector, if no such element found.
             */
            size_t where(vector<type> comparedVector) const
            {
                size_t counter = 0;
                for(size_t i = 0; i < size; ++i)
                {
                    if(comparedVector[counter] == array[i])
                    {
                        ++counter;
                        if(counter == comparedVector.size)
                        {
                            return counter;
                        }
                    }
                }
                return size;
            }

            /**
             * @brief Composes new vector from all elements which flagged by given function.
             * 
             * @param shouldInclude Checks whenever element should be included.
             * @return vector<type> Returned vector.
             */
            vector<type> compose(bool(shouldInclude)(const type&)) const
            {
                vector<type> newVector{standart};
                for(size_t i = 0; i < size; ++i)
                {
                    if(shouldInclude(array[i]))
                    {
                        newVector.append(array[i]);
                    }
                }
                return newVector;
            }

            /**
             * @brief Composes new vector out of elements of old vector which were processed by specific function.
             * 
             * @param processFunction Function, which changes value.
             * @return vector<type> Returned vector.
             */
            vector<type> process(type(processFunction)(const type&))
            {
                vector<type> newVector{standart};
                for(size_t i = 0; i < size; ++i)
                {
                    newVector.append(processFunction(array[i]));
                }
                return newVector;
            }

            size_t max() const
            {
                if(array == nullptr) return 0;
                size_t maxPlacement = 0;
                type maxValue = array[0];
                for(size_t i = 0; i < size; ++i)
                {
                    if(array[i] > maxValue)
                    {
                        maxValue = array[i];
                        maxPlacement = i;
                    }
                }
                return maxPlacement;
            }

            size_t min() const
            {
                if(array == nullptr) return 0;
                size_t minPlacement = 0;
                type minValue = array[0];
                for(size_t i = 0; i < size; ++i)
                {
                    if(array[i] < minValue)
                    {
                        minValue = array[i];
                        minPlacement = i;
                    }
                }
                return minPlacement;
            }

            //Excludes list from list.
            void exclude(const size_t &from, const size_t &to)
            {
                if((size - (from - to + 1)) == 0)
                {
                    
                    return;
                }
                if(from > to)
                {
                    if(to >= size) return;
                    type* newArray = (type*)malloc((size - (from - to + 1)) * sizeof(type));
                    size_t readi = 0;
                    for(size_t i = 0; i < size - (from - to + 1); ++i)
                    {
                        if(readi >= to and readi <= from)
                        {
                            --i;
                        }
                        else
                        {
                            newArray[i] = array[readi];
                        }
                        ++readi;
                    }
                    free(array);
                    array = newArray;
                    arraySize -= (from - to + 1);
                }
                else
                {
                    if(from >= size) return;
                    type* newArray = (type*)malloc((size - (to - from + 1)) * sizeof(type));
                    size_t readi = 0;
                    for(size_t i = 0; i < size - (to - from + 1); ++i)
                    {
                        if(readi >= from and readi <= to)
                        {
                            --i;
                        }
                        else
                        {
                            newArray[i] = array[readi];
                        }
                        ++readi;
                    }
                    free(array);
                    array = newArray;
                    arraySize -= (to - from + 1);
                }
            }

            /**
             * @brief Create new vector from selected data. Works even if one of the points is out of vector bounds.
             * 
             * @param from From which index start.
             * @param to On which index end.
             * @return vector<type> Returned new vector.
             */
            vector<type> copy(const size_t &from, const size_t &to) const
            {
                vector<type> newVector{standart};
                if(array == nullptr) return newVector;
                if(from > to)
                {
                    if(to >= size) return newVector;
                    for(size_t i = to; i <= from; ++i)
                    {
                        if(i < size) newVector[i - to] = array[i];
                    }
                    return newVector;
                }
                else
                {
                    if(from >= size) return newVector;
                    for(size_t i = from; i <= to; ++i)
                    {
                        if(i < size) newVector[i - from] = array[i];
                    }
                    return newVector;
                }
            }

            /**
             * @brief Reverts vector.
             * 
             */
            void reverse()
            {
                type temporary;
                for(unsigned long long i = 0; i < size/2; ++i)
                {
                    temporary = array[size - 1 - i];
                    array[size - 1 - i] = array[i];
                    array[i] = temporary;
                }
            }

            /**
             * @brief Create new vector from data, which later will be excluded.
             * 
             * @param from From which index start.
             * @param to On which index end.
             * @return vector<type> Returned new vector.
             */
            vector<type> cut(const size_t &from, const size_t &to)
            {
                vector<type> newVector{standart};
                if(array == nullptr) return newVector;
                if(from > to and to >= size) return newVector;
                else if(from >= size) return newVector;
                newVector = copy(from, to);
                exclude(from, to);
                return newVector;
            }

            /**
             * @brief Function, which inserts vector at index.
             * 
             * @param insertedVector Vector which will be inserted.
             * @param index Index at which vector will be inserted.
             */
            void insert(vector<type> insertedVector, size_t index = 0)
            {
                if(index > size) return;
                for(size_t i = 0; i < size; ++i)
                {
                    push(insertedVector[i], i + index);
                }
            }

            /**
             * @brief Returns constant array from itself. Useful for strings printing.
             * 
             * @return type* Returned constant array.
             */
            operator const type*&() const
            {
                return (const type*&)array;
            }

            type &operator[](const size_t &index)
            {
                if(index >= arraySize or array == nullptr)
                {
                    type* newArray = (type*)malloc((index + 1) * sizeof(type));
                    for(size_t i = 0; i < index + 1; ++i)
                    {
                        if(i < size) newArray[i] = array[i];
                        else newArray[i] = standart;
                    }
                    free(array);
                    array = newArray;
                    arraySize = index + 1;
                    return array[index];
                }
                else
                {
                    return array[index];
                }
            }

            void clear()
            {
                if(array != nullptr) free(array);
                array = nullptr;
                arraySize = 0;
            }

            void append(const type& newElement)
            {
                type* newArray = (type*)malloc((size + 1) * sizeof(type));
                for(size_t i = 0; i < size + 1; ++i)
                {
                    if(i < size) newArray[i] = array[i];
                    else newArray[i] = newElement;
                }
                free(array);
                array = newArray;
                ++arraySize;
            }

            void append(const vector<type>& appendedVector)
            {
                insert(appendedVector, size);
            }

            vector<type>& operator=(const vector<type>& assignedVector)
            {
                
                for(size_t i = 0; i < assignedVector.size; ++i)
                {
                    append(assignedVector[i]);
                }
                standart = assignedVector.standart;
                return *this;
            }

            vector<type>(const vector<type>& assignedVector)
            {
                
                for(size_t i = 0; i < assignedVector.size; ++i)
                {
                    append(assignedVector[i]);
                }
                standart = assignedVector.standart;
                //return *this;
            }

            vector<type>(const type* const assignedArray, size_t assignedSize, type newStandart = 0)
            {
                
                for(size_t i = 0; i < assignedSize; ++i)
                {
                    append(assignedArray[i]);
                }
                standart = newStandart;
                //return *this;
            }

            /**
             * @brief Makes arrays smaller by given size.
             * 
             * @param reducedSize Number of elements for reduction.
             */
            void reduce(const size_t &reducedSize = 1)
            {
                //type* newArray = (type*)malloc((arraySize - reducedSize) * sizeof(type));
                type* newArray = (type*)reallocarray(array, sizeof(type), arraySize - reducedSize);
                if(newArray == nullptr)
                {
                    newArray = (type*)malloc((arraySize - reducedSize) * sizeof(type));
                    for(size_t i = 0; i < arraySize - reducedSize; ++i)
                    {
                        newArray[i] = array[i];
                    }
                    free(array);
                }
                array = newArray;
                arraySize = arraySize - reducedSize;
            }

            void offset(const size_t &choosedOffset)
            {
                if(choosedOffset == 0)
                {
                    return;
                }
                type* newArray = (type*)malloc((arraySize + choosedOffset) * sizeof(type));
                size_t readi = 0;
                for(size_t i = 0; i < arraySize + choosedOffset; ++i)
                {
                    if(i < choosedOffset)
                    {
                        newArray[i] = standart;
                    }
                    else
                    {
                        newArray[i] = array[readi];
                        ++readi;
                    }
                }
                free(array);
                array = newArray;
                arraySize = arraySize + choosedOffset;
            }

            void offset(const long long &choosedOffset)
            {
                if(choosedOffset >= 0)
                {
                    offset((size_t)choosedOffset);
                }
                else
                {
                    type* newArray = (type*)malloc((arraySize + choosedOffset) * sizeof(type));
                    size_t writteni = 0;
                    for(size_t i = std::abs(choosedOffset); i < arraySize + choosedOffset; ++i)
                    {
                        newArray[writteni] = array[i];
                        ++writteni;
                    }
                    free(array);
                    array = newArray;
                    arraySize = arraySize + choosedOffset;
                }
            }

            bool operator==(const vector& comparedVector) const
            {
                if(size != comparedVector.size) return false;
                for(size_t i = 0; i < size; ++i)
                {
                    if(array[i] != comparedVector[i])
                    {
                        return false;
                    }
                }
                return true;
            }

            ~vector()
            {
                if(array != nullptr) free(array);
            }

            vector<type> operator<(const type& comparedValue) const
            {
                return compose([comparedValue](const type& checkedValue){ return checkedValue < comparedValue; });
            }

            vector<type> operator>(const type& comparedValue) const
            {
                return compose([comparedValue](const type& checkedValue){ return checkedValue > comparedValue; });
            }

            vector<type> operator<=(const type& comparedValue) const
            {
                return compose([comparedValue](const type& checkedValue){ return checkedValue <= comparedValue; });
            }

            vector<type> operator>=(const type& comparedValue) const
            {
                return compose([comparedValue](const type& checkedValue){ return checkedValue >= comparedValue; });
            }

            vector<type> operator==(const type& comparedValue) const
            {
                return compose([comparedValue](const type& checkedValue){ return checkedValue == comparedValue; });
            }

            vector<type> operator!=(const type& comparedValue) const
            {
                return compose([comparedValue](const type& checkedValue){ return checkedValue != comparedValue; });
            }

            vector<type> operator+(const type& processedValue)
            {
                return process([processedValue](const type& checkedValue){ return checkedValue + processedValue; });
            }

            vector<type> operator-(const type& processedValue)
            {
                return process([processedValue](const type& checkedValue){ return checkedValue - processedValue; });
            }

            vector<type> operator*(const type& processedValue)
            {
                return process([processedValue](const type& checkedValue){ return checkedValue * processedValue; });
            }

            vector<type> operator/(const type& processedValue)
            {
                return process([processedValue](const type& checkedValue){ return checkedValue / processedValue; });
            }
    };

    template<class type>
    inline vector<type> range(type first, type last)
    {
        vector<type> newVector;
        for(type i = first; i < last; ++i) newVector.append(i);
        return newVector;
    }
};
