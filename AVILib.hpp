#include <cmath>
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

            vector<type>(type selectedStandart)
            {
                standart = selectedStandart;
            }

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
                type* newArray = new type[size + 1];
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
                delete[] array;
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
                if(index >= size or size == 0) return;
                type* newArray = new type[size - 1];
                size_t j = 0;
                for(size_t i = 0; i < size; ++i)
                {
                    if(index == i)
                    {
                        continue;
                    }
                    newArray[j] = array[i];
                    ++j;
                }
                delete[] array;
                array = newArray;
                ++arraySize;
            }

            //Excludes list from list.
            void exclude(const size_t &from, const size_t &to)
            {
                if(from > to)
                {
                    if(to >= size) return;
                    type* newArray = new type[size - (from - to + 1)];
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
                    delete[] array;
                    array = newArray;
                }
                else
                {
                    if(from >= size) return;
                    type* newArray = new type[size - (to - from + 1)];
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
                    delete[] array;
                    array = newArray;
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
             * @param insertedVector 
             * @param index 
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
                    type* newArray = new type[index + 1];
                    for(size_t i = 0; i < index + 1; ++i)
                    {
                        if(i < size) newArray[i] = array[i];
                        else newArray[i] = standart;
                    }
                    delete[] array;
                    array = newArray;
                    arraySize = index + 1;
                    return array[index];
                }
                else
                {
                    return array[index];
                }
            }

            void clean()
            {
                delete[] array;
                array = nullptr;
                arraySize = 0;
            }

            void append(const type& newElement)
            {
                type* newArray = new type[size + 1];
                for(size_t i = 0; i < size + 1; ++i)
                {
                    if(i < size) newArray[i] = array[i];
                    else newArray[i] = newElement;
                }
                delete[] array;
                array = newArray;
                ++arraySize;
            }

            void append(const vector<type>& appendedVector)
            {
                insert(appendedVector, size);
            }

            vector<type>& operator=(const vector<type>& assignedVector)
            {
                clean();
                for(size_t i = 0; i < assignedVector.size; ++i)
                {
                    append(assignedVector[i]);
                }
                standart = assignedVector.standart;
                return *this;
            }

            vector<type>(const vector<type>& assignedVector)
            {
                clean();
                for(size_t i = 0; i < assignedVector.size; ++i)
                {
                    append(assignedVector[i]);
                }
                standart = assignedVector.standart;
                //return *this;
            }

            vector<type>(const type* const assignedArray, size_t assignedSize, type newStandart = 0)
            {
                clean();
                for(size_t i = 0; i < assignedSize; ++i)
                {
                    append(assignedArray[i]);
                }
                standart = newStandart;
                //return *this;
            }

            vector<type>& operator=(std::initializer_list<type> initializationArray)
            {
                clean();
                for(type newElement : initializationArray)
                {
                    append(newElement);
                }
                return *this;
            }

            vector<type>(std::initializer_list<type> initializationArray)
            {
                clean();
                for(type newElement : initializationArray)
                {
                    append(newElement);
                }
            }

            /**
             * @brief Makes arrays smaller by given size.
             * 
             * @param reducedSize Number of elements for reduction.
             */
            void reduce(const size_t &reducedSize = 1)
            {
                type* newArray = new type[arraySize - reducedSize];
                for(size_t i = 0; i < arraySize - reducedSize; ++i)
                {
                    newArray[i] = array[i];
                }
                delete[] array;
                array = newArray;
                arraySize = arraySize - reducedSize;
            }

            void offset(const size_t &choosedOffset)
            {
                if(choosedOffset == 0)
                {
                    return;
                }
                type* newArray = new type[arraySize + choosedOffset];
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
                delete[] array;
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
                    type* newArray = new type[arraySize + choosedOffset];
                    size_t writteni = 0;
                    for(size_t i = std::abs(choosedOffset); i < arraySize + choosedOffset; ++i)
                    {
                        newArray[writteni] = array[i];
                        ++writteni;
                    }
                    delete[] array;
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
                if(array != nullptr) delete[] array;
            }
    };
};