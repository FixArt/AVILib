#include <cmath>
// #include <cstdint>
// #include <cstdlib>
// #include <cstring>
#include <cstring>
#include <initializer_list>
#include <functional>
#include <algorithm>
#include <type_traits>

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

            //Function for generating random numbers between min and max.
            template<class randomType>
            static randomType random(const randomType& min, const randomType& max)
            {
                return min + rand() % (max - min + 1);
            }

            //Function for generating random numbers between min and max.
            template<class randomType>
            static randomType random(const randomType& min, const randomType& max, randomType(randomProvider)())
            {
                return min + rand() % (max - min + 1);
            }

            void heapify(size_t lookedStart, size_t lookedSize)
            {
                type largest = lookedStart;
                size_t left = 2 * lookedStart + 1;
                size_t right = 2 * lookedStart + 2;
                if (left < lookedSize && array[left] > array[largest])
                    largest = left;
                if (right < lookedSize && array[right] > array[largest])
                    largest = right;
                if (largest != lookedStart)
                {
                    std::swap(array[lookedStart], array[largest]);
                    heapify(largest, lookedSize);
                }
            }

            void heapify(size_t lookedStart, size_t lookedSize, bool(comparer)(const type&, const type&))
            {
                type largest = lookedStart;
                size_t left = 2 * lookedStart + 1;
                size_t right = 2 * lookedStart + 2;
                if (left < lookedSize && comparer(array[left], array[largest]))
                    largest = left;
                if (right < lookedSize && comparer(array[right], array[largest]))
                    largest = right;
                if (largest != lookedStart)
                {
                    std::swap(array[lookedStart], array[largest]);
                    heapify(largest, lookedSize);
                }
            }
        public:

            vector<type>()
            {
                array = nullptr;
                arraySize = 0;
            }

            const size_t &size = arraySize;

            vector<type>& operator=(std::initializer_list<type> initializationArray)
            {
                // for(type newElement : initializationArray)
                // {
                //     append(newElement);
                // }
                resize(initializationArray.size());
                std::copy(initializationArray.begin(), initializationArray.end(), (*this).begin());
                return *this;
            }

            vector<type>(std::initializer_list<type> initializationArray)
            {
                // for(type newElement : initializationArray)
                // {
                //     append(newElement);
                // }
                resize(initializationArray.size());
                std::copy(initializationArray.begin(), initializationArray.end(), (*this).begin());
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
                //if(array == nullptr) return false; //Not needed.
                return index < size;
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
                if(index == size){ (*this)[index] = newElement; return; }
                type* newArray = (type*)malloc((size + 1) * sizeof(type));
                if(newArray == nullptr) { throw(ENOMEM); }
                size_t writteni = 0;
                for(size_t i = 0; i < size; ++i)
                {
                    if(writteni == index)
                    {
                        new (&newArray[writteni]) type(newElement);
                        --i;
                    }
                    else
                    {
                        new (&newArray[writteni]) type(array[i]);
                    }
                    ++writteni;
                }
                for(size_t i = 0; i < arraySize; ++i) { array[i].~type(); }
                free(array);
                array = newArray;
                ++arraySize;
            }

            template<class... arguments>
            void push(const type &newElement, const size_t &index, arguments... newElements)
            {
                push(newElement, index);
                push(newElements...);
            }

            /**
             * @brief Sums all elements.
             * 
             * @return type Sum of all elements.
             */
            type sum() const
            {
                type allElements = 0;
                for(size_t i = 0; i < size; ++i)
                {
                    allElements += array[i];
                }
                return allElements;
            }

            short compare(const vector<type>& comparedVector, bool includeSizes = false) const
            {
                if(size > comparedVector.size)
                {
                    for(size_t i = 0; i < comparedVector.size; ++i)
                    {
                        if(array[i] > comparedVector[i])
                        {
                            return 1;
                        }
                        else if(array[i] < comparedVector[i])
                        {
                            return -1;
                        }
                    }
                    if(includeSizes) return 1;
                }
                else
                {
                    for(size_t i = 0; i < size; ++i)
                    {
                        if(array[i] > comparedVector[i])
                        {
                            return 1;
                        }
                        else if(array[i] < comparedVector[i])
                        {
                            return -1;
                        }
                    }
                    if(includeSizes) return (size == comparedVector.size)?(0):(-1);
                }
                return 0;
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

            void pop(vector<size_t> poppedIndexes)
            {
                for(size_t index : poppedIndexes)
                {
                    pop(index);
                }
            }

            template<class... arguments>
            void pop(const size_t &index, arguments... removedIndexes)
            {
                pop(index);
                pop(removedIndexes...);
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

            template<class... arguments>
            void remove(const type& checkedElement, arguments... checkedElements)
            {
                remove(checkedElement);
                remove(checkedElements...);
            }

            void remove(vector<type> checkedElements)
            {
                for(type checkedElement : checkedElements)
                {
                    remove(checkedElement);
                }
            }

            template<class... arguments>
            void remove(bool(shouldRemove)(const type&), arguments... removalFunctions)
            {
                remove(shouldRemove);
                remove(removalFunctions...);
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
             * @param searchedVector Searched vector.
             * @return size_t Placement. Size of vector, if no such element found.
             */
            size_t where(vector<type> searchedVector) const
            {
                if(searchedVector.size == 0)
                {
                    return size;
                }
                size_t counter = 0;
                for(size_t i = 0; i < size; ++i)
                {
                    if(searchedVector[counter] == array[i])
                    {
                        ++counter;
                        if(counter == searchedVector.size)
                        {
                            return i - counter;
                        }
                    }
                }
                return size;
            }

            bool contains(const type& checkedElement) const
            {
                for(size_t i = 0; i < size; ++i)
                {
                    if(checkedElement == array[i])
                    {
                        return true;
                    }
                }
                return false;
            }

            bool contains(bool(shouldCount)(const type&)) const
            {
                for(size_t i = 0; i < size; ++i)
                {
                    if(shouldCount(array[i]))
                    {
                        return true;
                    }
                }
                return false;
            }

            bool contains(vector<type> searchedVector) const
            {
                if(searchedVector.size == 0)
                {
                    return size;
                }
                size_t counter = 0;
                for(size_t i = 0; i < size; ++i)
                {
                    if(searchedVector[counter] == array[i])
                    {
                        ++counter;
                        if(counter == searchedVector.size)
                        {
                            return true;
                        }
                    }
                }
                return false;
            }

            /**
             * @brief Composes new vector from all elements which flagged by given function.
             * 
             * @param shouldInclude Checks whenever element should be included.
             * @return vector<type> Returned vector.
             */
            vector<type> compose(bool(shouldInclude)(const type&)) const
            {
                vector<type> newVector;
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
             * @brief Composes new vector from all elements which flagged by given function.
             * 
             * @param shouldInclude Checks whenever element should be included.
             * @return vector<type> Returned vector.
             */
            vector<type> compose(std::function<bool(const type&)> shouldInclude) const
            {
                vector<type> newVector;
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
             * @brief Composes new vector from all elements which flagged by given function.
             * 
             * @param shouldInclude Checks whenever element should be included.
             * @return vector<type> Returned vector.
             */
            vector<type> compose(bool(shouldInclude)(const type&, size_t)) const
            {
                vector<type> newVector;
                for(size_t i = 0; i < size; ++i)
                {
                    if(shouldInclude(array[i], i))
                    {
                        newVector.append(array[i]);
                    }
                }
                return newVector;
            }

            /**
             * @brief Composes new vector from all elements which flagged by given function.
             * 
             * @param shouldInclude Checks whenever element should be included.
             * @return vector<type> Returned vector.
             */
            vector<type> compose(std::function<bool(const type&, size_t)> shouldInclude) const
            {
                vector<type> newVector;
                for(size_t i = 0; i < size; ++i)
                {
                    if(shouldInclude(array[i], i))
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
                vector<type> newVector;
                for(size_t i = 0; i < size; ++i)
                {
                    newVector.append(processFunction(array[i]));
                }
                return newVector;
            }

            /**
             * @brief Composes new vector out of elements of old vector which were processed by specific function.
             * 
             * @param processFunction Function, which changes value.
             * @return vector<type> Returned vector.
             */
            vector<type> process(std::function<type(const type&)> processFunction)
            {
                vector<type> newVector;
                for(size_t i = 0; i < size; ++i)
                {
                    newVector.append(processFunction(array[i]));
                }
                return newVector;
            }

            /**
             * @brief Composes new vector out of elements of old vector which were processed by specific function.
             *
             * @param processFunction Function, which changes value.
             * @return vector<type> Returned vector.
             */
            vector<type> process(type(processFunction)(const type&, size_t))
            {
                vector<type> newVector;
                for(size_t i = 0; i < size; ++i)
                {
                    newVector.append(processFunction(array[i], i));
                }
                return newVector;
            }

            /**
             * @brief Composes new vector out of elements of old vector which were processed by specific function.
             * 
             * @param processFunction Function, which changes value.
             * @return vector<type> Returned vector.
             */
            vector<type> process(std::function<type(const type&, size_t)> processFunction)
            {
                vector<type> newVector;
                for(size_t i = 0; i < size; ++i)
                {
                    newVector.append(processFunction(array[i], i));
                }
                return newVector;
            }

            /**
             * @brief Fills list with value.
             * 
             * @param fillValue Value with which vector will be filled.
             */
            void fill(const type& fillValue)
            {
                for(size_t i = 0; i < size; ++i)
                {
                    array[i] = fillValue;
                }
            }

            /**
             * @brief Fills vector through given function.
             * 
             * @param processFunction Function which gets position as argument.
             */
            void fill(type(processFunction)(const size_t&))
            {
                for(size_t i = 0; i < size; ++i)
                {
                    array[i] = processFunction(i);
                }
            }

            /**
             * @brief Fills vector through given function.
             * 
             * @param processFunction Function which gets position as argument.
             */
            void fill(std::function<type(const size_t&)> processFunction)
            {
                for(size_t i = 0; i < size; ++i)
                {
                    array[i] = processFunction(i);
                }
            }

            /**
             * @brief Fills vector through given function.
             * 
             * @param processFunction Given function.
             */
            void fill(type(processFunction)())
            {
                for(size_t i = 0; i < size; ++i)
                {
                    array[i] = processFunction();
                }
            }

            /**
             * @brief Fills vector through given function.
             * 
             * @param processFunction Given function.
             */
            void fill(std::function<type()> processFunction)
            {
                for(size_t i = 0; i < size; ++i)
                {
                    array[i] = processFunction();
                }
            }

            vector<type> order(const vector<size_t>& map) const
            {
                vector<type> newVector;
                if(map.size != size) return newVector;
                size_t i = 0;
                for(size_t placement : map)
                {
                    newVector[placement] = array[i];
                    ++i;
                }
                return newVector;
            }

            vector<type> order(size_t(processFunction)(const size_t&)) const
            {
                vector<type> newVector;
                for(size_t i = 0; i < size; ++i)
                {
                    newVector[processFunction(i)] = array[i];
                }
                return newVector;
            }

            vector<type> order(size_t(processFunction)(const size_t&, const type&)) const
            {
                vector<type> newVector;
                for(size_t i = 0; i < size; ++i)
                {
                    newVector[processFunction(i, array[i])] = array[i];
                }
                return newVector;
            }

            vector<type> order(std::function<size_t(const size_t&)> processFunction) const
            {
                vector<type> newVector;
                for(size_t i = 0; i < size; ++i)
                {
                    newVector[processFunction(i)] = array[i];
                }
                return newVector;
            }

            vector<type> order(std::function<size_t(const size_t&, const type&)> processFunction) const
            {
                vector<type> newVector;
                for(size_t i = 0; i < size; ++i)
                {
                    newVector[processFunction(i, array[i])] = array[i];
                }
                return newVector;
            }

            bool isComposed(const vector<type>& checkedVector, bool(shouldInclude)(const type&)) const
            {
                return checkedVector == compose(shouldInclude);
            }

            bool isComposed(const vector<type>& checkedVector, std::function<bool(const type&)> shouldInclude) const
            {
                return checkedVector == compose(shouldInclude);
            }

            bool isComposed(const vector<type>& checkedVector, bool(shouldInclude)(const type&, size_t)) const
            {
                return checkedVector == compose(shouldInclude);
            }

            bool isComposed(const vector<type>& checkedVector, std::function<bool(const type&, size_t)> shouldInclude) const
            {
                return checkedVector == compose(shouldInclude);
            }

            bool isProcessed(const vector<type>& checkedVector, bool(processFunction)(const type&)) const
            {
                return checkedVector == process(processFunction);
            }

            bool isProcessed(const vector<type>& checkedVector, std::function<bool(const type&)> processFunction) const
            {
                return checkedVector == process(processFunction);
            }

            bool isProcessed(const vector<type>& checkedVector, bool(processFunction)(const type&, size_t)) const
            {
                return checkedVector == process(processFunction);
            }

            bool isProcessed(const vector<type>& checkedVector, std::function<bool(const type&, size_t)> processFunction) const
            {
                return checkedVector == process(processFunction);
            }

            bool isFilled(const type& fillValue) const
            {
                vector<type> newVector;
                newVector.resize(size);
                return *this == newVector.fill(fillValue);
            }

            bool isFilled(bool(processFunction)(const type&)) const
            {
                vector<type> newVector;
                newVector.resize(size);
                return *this == newVector.fill(processFunction);
            }

            bool isFilled(std::function<bool(const type&)> processFunction) const
            {
                vector<type> newVector;
                newVector.resize(size);
                return *this == newVector.fill(processFunction);
            }

            bool isOrdered(const vector<type>& checkedVector, const vector<size_t>& map) const
            {
                return checkedVector == order(map);
            }

            bool isOrdered(const vector<type>& checkedVector, size_t(processFunction)(const size_t&)) const
            {
                return checkedVector == order(processFunction);
            }

            bool isOrdered(const vector<type>& checkedVector, size_t(processFunction)(const size_t&, const type&)) const
            {
                return checkedVector == order(processFunction);
            }

            bool isOrdered(const vector<type>& checkedVector, std::function<size_t(const size_t&)> processFunction) const
            {
                return checkedVector == order(processFunction);
            }

            bool isOrdered(const vector<type>& checkedVector, std::function<size_t(const size_t&, const type&)> processFunction) const
            {
                return checkedVector == order(processFunction);
            }

            void shuffle()
            {
                for(size_t i = 0; i < size; ++i)
                {
                    std::swap(array[i], array[random<size_t>(0, size - 1)]);
                }
            }

            void shuffle(size_t(provider)(size_t, size_t))
            {
                for(size_t i = 0; i < size; ++i)
                {
                    std::swap(array[i], array[provider(0, size - 1)]);
                }
            }

            void shuffle(size_t(provider)(size_t, size_t, size_t))
            {
                for(size_t i = 0; i < size; ++i)
                {
                    std::swap(array[i], array[provider(0, size - 1, i)]);
                }
            }

            void shuffle(size_t(provider)())
            {
                for(size_t i = 0; i < size; ++i)
                {
                    std::swap(array[i], array[random<size_t>(0, size - 1, provider)]);
                }
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
                    if(newArray == nullptr) { throw(ENOMEM); }
                    size_t readi = 0;
                    for(size_t i = 0; i < size - (from - to + 1); ++i)
                    {
                        if(readi >= to and readi <= from)
                        {
                            --i;
                        }
                        else
                        {
                            new (&newArray[i]) type(array[readi]);
                        }
                        ++readi;
                    }
                    for(size_t i = 0; i < arraySize; ++i) { array[i].~type(); }
                    free(array);
                    array = newArray;
                    arraySize -= (from - to + 1);
                }
                else
                {
                    if(from >= size) return;
                    type* newArray = (type*)malloc((size - (to - from + 1)) * sizeof(type));
                    if(newArray == nullptr) { throw(ENOMEM); }
                    size_t readi = 0;
                    for(size_t i = 0; i < size - (to - from + 1); ++i)
                    {
                        if(readi >= from and readi <= to)
                        {
                            --i;
                        }
                        else
                        {
                            new (&newArray[i]) type(array[readi]);
                        }
                        ++readi;
                    }
                    for(size_t i = 0; i < arraySize; ++i) { array[i].~type(); }
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
                vector<type> newVector;
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
                for(size_t i = 0; i < size/2; ++i)
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
                vector<type> newVector;
                if(array == nullptr) return newVector;
                if(from > to and to >= size) return newVector;
                else if(from >= size) return newVector;
                newVector = copy(from, to);
                exclude(from, to);
                return newVector;
            }

            /**
             * @brief Function, which places vector at index.
             * 
             * @param pushedVector Vector which will be placed at index.
             * @param index Index at which vector will be placed.
             */
            void push(vector<type> pushedVector, size_t index = 0)
            {
                if(index > size) return;
                for(size_t i = 0; i < pushedVector.size; ++i)
                {
                    push(pushedVector[i], i + index);
                }
            }

            /**
             * @brief Returns constant array from itself. Useful for strings printing.
             * 
             * @return type* Returned constant array.
             */
            operator const type*() const
            {
                if(array == nullptr) throw(EFAULT);
                return (const type*)array;
            }

            /**
             * @brief Returns constant array from itself. Useful for strings printing.
             * 
             * @return type& Returned constant array.
             */
            operator const type&() const
            {
                if(array == nullptr) throw(EFAULT);
                return (const type&)array;
            }

            type &operator[](const size_t &index)
            {
                if(!exists(index))
                {
                    resize(index + 1);
                }
                return array[index];
            }

            const type& operator[](const size_t &index) const
            {
                if(!exists(index))
                {
                    throw(EFAULT);
                }
                return array[index];
            }

            void clear()
            {
                for(size_t i = 0; i < arraySize; ++i) { array[i].~type(); }
                if(array != nullptr) free(array);
                array = nullptr;
                arraySize = 0;
            }

            /**
             * @brief Appends variable.
             * 
             * @param newElement Variable to append.
             */
            void append(const type& newElement)
            {
                type* newArray = (type*)malloc((size + 1) * sizeof(type));
                if(newArray == nullptr) { throw(ENOMEM); }
                for(size_t i = 0; i < size + 1; ++i)
                {
                    if(i < size) new (&newArray[i]) type(array[i]);
                    else new (&newArray[i]) type(newElement);
                }
                for(size_t i = 0; i < arraySize; ++i) { array[i].~type(); }
                free(array);
                array = newArray;
                ++arraySize;
            }

            /**
             * @brief Appends vector.
             * 
             * @param appendedVector Vector to append.
             */
            void append(const vector<type>& appendedVector)
            {
                push(appendedVector, size);
            }

            template<class... arguments>
            void append(const type& newElement, arguments... appendedElements)
            {
                append(newElement);
                append(appendedElements...);
            }

            template<class... arguments>
            void append(const vector<type>& newElement, arguments... appendedElements)
            {
                append(newElement);
                append(appendedElements...);
            }

            vector<type>& operator=(const vector<type>& assignedVector)
            {
                // for(size_t i = 0; i < assignedVector.size; ++i)
                // {
                //     append(assignedVector[i]);
                // }
                // memcpy(array, assignedVector, assignedVector.size * sizeof(type));
                resize(assignedVector.size);
                std::copy(assignedVector.begin(), assignedVector.end(), (*this).begin());
                return *this;
            }

            vector<type>(const vector<type>& assignedVector)
            {
                // for(size_t i = 0; i < assignedVector.size; ++i)
                // {
                //     append(assignedVector[i]);
                // }
                // memcpy(array, assignedVector, assignedVector.size * sizeof(type));
                resize(assignedVector.size);
                std::copy(assignedVector.begin(), assignedVector.end(), (*this).begin());
                //return *this;
            }

            vector<type>(const type* const assignedArray, size_t assignedSize, type newStandart = 0)
            {
                for(size_t i = 0; i < assignedSize; ++i)
                {
                    append(assignedArray[i]);
                }
                // memcpy(array, assignedArray, assignedSize * sizeof(type));
                resize(assignedSize);
                std::copy(assignedArray[0], assignedArray[assignedSize], array);
            }

            void resize(const size_t &newSize)
            {
                if(newSize == 0)
                {
                    clear();
                    return;
                }
                if(newSize == arraySize) return;
                if(array != nullptr)
                {
                    if(newSize < arraySize)
                    {
                        for(size_t i = newSize - 1; i < arraySize; ++i)
                        {
                            array[i].~type();
                        }
                    }
                    type* newArray = (type*)realloc(array, sizeof(type) * newSize);
                    if(newArray == nullptr)
                    {
                        newArray = (type*)malloc((newSize) * sizeof(type));
                        if(newArray == nullptr) { throw(ENOMEM); }
                        for(size_t i = 0; i < newSize; ++i)
                        {
                            if(i < size) new (&newArray[i]) type(array[i]);
                        }
                        for(size_t i = 0; i < ((arraySize > newSize)?(newSize):(arraySize)); ++i)
                        {
                            array[i].~type();
                        }
                        free(array);
                    }
                    else
                    {
                        if(newSize > arraySize)
                        {
                            for(size_t i = arraySize - 1; i < newSize; ++i)
                            {
                                new (&newArray[i]) type;
                            }
                        }
                    }
                    array = newArray;
                    arraySize = newSize;
                }
                else
                {
                    array = (type*)malloc((newSize) * sizeof(type));
                    if(array == nullptr) { throw(ENOMEM); }
                    for(size_t i = 0; i < newSize; ++i)
                    {
                        new (&array[i]) type;
                    }
                    // for(size_t i = 0; i < newSize; ++i)
                    // {
                    //     // type nulled{};
                    //     array[i] = {};
                    //     // memcpy(&array[i], &nulled, sizeof(type));
                    //     // std::copy(&nulled, (&nulled + sizeof(type)), &array[i]);
                    // }
                    // memset(array, 0, (newSize) * sizeof(type));
                    arraySize = newSize;
                }
            }

            /**
             * @brief Makes arrays smaller by given size.
             * 
             * @param reducedSize Number of elements for reduction.
             */
            void reduce(const size_t &reducedSize = 1)
            {
                if(reducedSize >= arraySize)
                {
                    clear();
                    return;
                }
                for(size_t i = arraySize - reducedSize - 1; i < arraySize; ++i)
                {
                    array[i].~type();
                }
                type* newArray = (type*)realloc(array, sizeof(type) * (arraySize - reducedSize));
                if(newArray == nullptr)
                {
                    newArray = (type*)malloc((arraySize - reducedSize) * sizeof(type));
                    if(newArray == nullptr) { throw(ENOMEM); }
                    for(size_t i = 0; i < arraySize - reducedSize; ++i)
                    {
                        new (&newArray[i]) type(array[i]);
                    }
                    // newArray = (type*)memcpy(newArray, array, (arraySize - reducedSize) * sizeof(type));
                    // std::copy(array[0], array[(arraySize - reducedSize) * sizeof(type)], newArray);
                    for(size_t i = 0; i < arraySize - reducedSize; ++i)
                    {
                        array[i].~type();
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
                size_t readi = 0;
                resize(arraySize + choosedOffset);
                for(size_t i = 0; i < arraySize + choosedOffset; ++i)
                {
                    if(i >= choosedOffset)
                    {
                        array[i] = array[readi];
                        array[readi] = {};
                        ++readi;
                    }
                }
            }

            void offset(const intmax_t &choosedOffset)
            {
                if(choosedOffset >= 0)
                {
                    offset((size_t)choosedOffset);
                }
                else
                {
                    size_t writteni = 0;
                    for(size_t i = std::abs(choosedOffset); i < arraySize - std::abs(choosedOffset); ++i)
                    {
                        array[writteni] = array[i];
                        ++writteni;
                    }
                    resize(arraySize - std::abs(choosedOffset));
                }
            }
            
            void bubbleSort()
            {
                for (size_t checkedSize = 0; checkedSize < size - 1; ++checkedSize)
                {
                    for(size_t i = 0; i<(size-checkedSize-1); ++i)
                    {
                        if(array[i] > array[i+1])
                        {
                            swap(array[i], array[i + 1]);
                            // type swap = array[i];
                            // array[i] = array[i+1];
                            // array[i+1] = swap;
                        }
                    }
                }
            }

            void bubbleSort(bool(comparer)(const type&, const type&))
            {
                for (size_t checkedSize = 0; checkedSize < size - 1; ++checkedSize)
                {
                    for(size_t i = 0; i < (size - checkedSize - 1); ++i)
                    {
                        if(comparer(array[i], array[i+1]))
                        {
                            swap(array[i], array[i + 1]);
                            // type swap = array[i];
                            // array[i] = array[i+1];
                            // array[i+1] = swap;
                        }
                    }
                }
            }

            void insertionSort()
            {
                for (size_t currentPoint = 0; currentPoint < size; ++currentPoint)
                {
                    for(size_t i = 0; i < currentPoint; ++i)
                    {
                        if(array[currentPoint] < array[i])
                        {
                            int value = array[currentPoint];
                            for(int j = currentPoint - 1; j >= i; --j)
                            {
                                array[j+1] = array[j];
                            }
                            array[i] = value;
                        }
                    }
                }
            }

            void insertionSort(bool(comparer)(const type&, const type&))
            {
                for (size_t currentPoint = 0; currentPoint < size; ++currentPoint)
                {
                    for(size_t i = 0; i < currentPoint; ++i)
                    {
                        if(comparer(array[i], array[currentPoint]))
                        {
                            int value = array[currentPoint];
                            for(int j = currentPoint - 1; j >= i; --j)
                            {
                                array[j+1] = array[j];
                            }
                            array[i] = value;
                        }
                    }
                }
            }

            void selectionSort()
            {
                for(size_t i = 0; i < size - 1; ++i)
                {
                    size_t minimalIndex = i;
                    for (size_t j = i + 1; j < size; ++j)
                    {
                        if (array[j] < array[minimalIndex])
                        {
                            minimalIndex = j;
                        }
                    }
                    if (minimalIndex != i)
                    {
                        std::swap(array[i], array[minimalIndex]);
                    }
                }
            }

            void selectionSort(bool(comparer)(const type&, const type&))
            {
                for(size_t i = 0; i < size - 1; ++i)
                {
                    size_t minimalIndex = i;
                    for (size_t j = i + 1; j < size; ++j)
                    {
                        if (comparer(array[j], array[minimalIndex]))
                        {
                            minimalIndex = j;
                        }
                    }
                    if (minimalIndex != i)
                    {
                        std::swap(array[i], array[minimalIndex]);
                    }
                }
            }

            void heapSort()
            {
                for (size_t i = size / 2 - 1; i != 0; i--) heapify(i, size);
                for (size_t i = size - 1; i > 0; i--)
                {
                    std::swap(array[0], array[i]);
                    heapify(0, i);
                }
            }
            
            void heapSort(bool(comparer)(const type&, const type&))
            {
                for (size_t i = size / 2 - 1; i != 0; i--) heapify(i, size, comparer);
                for (size_t i = size - 1; i > 0; i--)
                {
                    std::swap(array[0], array[i]);
                    heapify(0, i, comparer);
                }
            }

            void sort()
            {
                std::sort(begin(), end());
            }

            void sort(bool(comparer)(const type&, const type&))
            {
                std::sort(begin(), end(), comparer);
            }
            
            void stableSort()
            {
                std::stable_sort(begin(), end());
            }

            void stableSort(bool(comparer)(const type&, const type&))
            {
                std::stable_sort(begin(), end(), comparer);
            }

            /**
             * @brief Checks whenever vector is sorted.
             * 
             * @return true Vector is sorted.
             * @return false Vector isn't sorted.
             */
            bool sorted() const
            {
                if(size == 0)
                {
                    return true;
                }
                for(size_t i = 0; i < size - 1; ++i)
                {
                    if(array[i] > array[i+1])
                    {
                        return false;
                    }
                }
                return true;
            }

            /**
             * @brief Checks whenever vector is sorted with given comparison function.
             * 
             * @param comparer Given comparison function.
             * @return true Vector is sorted.
             * @return false Vector isn't sorted.
             */
            bool sorted(bool(comparer)(const type&, const type&)) const
            {
                if(size == 0)
                {
                    return true;
                }
                for(size_t i = 0; i < size - 1; ++i)
                {
                    if(comparer(array[i], array[i+1]))
                    {
                        return false;
                    }
                }
                return true;
            }

            ~vector()
            {
                for(size_t i = 0; i < arraySize; ++i) { array[i].~type(); }
                if(array != nullptr) free(array);
            }

            void exchange(vector<type>& exchangedWith)
            {
                vector<type> newVector = exchangedWith;
                exchangedWith.resize(size);
                exchangedWith = *this;
                resize(newVector.size);
                *this = newVector;
            }

            /*Vectors operations*/

            bool operator==(const vector<type>& comparedVector) const
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

            bool operator!=(const vector<type>& comparedVector) const
            {
                if(size != comparedVector.size) return false;
                for(size_t i = 0; i < size; ++i)
                {
                    if(array[i] != comparedVector[i])
                    {
                        return true;
                    }
                }
                return false;
            }

            vector<type> operator+(const vector<type>& appendedVector) const
            {
                vector<type> newVector;
                newVector.append(appendedVector);
                return newVector;
            }

            vector<type> operator+=(const vector<type>& appendedVector)
            {
                append(appendedVector);
                return *this;
            }

            bool operator>(const vector<type>& comparedVector) const
            {
                return compare(comparedVector, true) == 1;
            }

            bool operator<(const vector<type>& comparedVector) const
            {
                return compare(comparedVector, true) == -1;
            }

            bool operator>=(const vector<type>& comparedVector) const
            {
                return compare(comparedVector, true) != -1;
            }

            bool operator<=(const vector<type>& comparedVector) const
            {
                return compare(comparedVector, true) != 1;
            }

            #if __cplusplus == 202002L
            short operator<=>(const vector<type>& comparedVector) const
            {
                return compare(comparedVector, true);
            }
            #endif

            /*Variables comparisons*/

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

            vector<type> operator%(const type& processedValue)
            {
                return process([processedValue](const type& checkedValue){ return checkedValue % processedValue; });
            }

            vector<type> operator++()
            {
                process([](const type& checkedValue){ return checkedValue + 1; });
                return *this;
            }

            vector<type> operator--()
            {
                process([](const type& checkedValue){ return checkedValue - 1; });
                return *this;
            }

            vector<type> operator++(int)
            {
                process([](const type& checkedValue){ return checkedValue + 1; });
                return *process([](const type& checkedValue){ return checkedValue - 1; });;
            }

            vector<type> operator--(int)
            {
                process([](const type& checkedValue){ return checkedValue - 1; });
                return *process([](const type& checkedValue){ return checkedValue + 1; });;
            }

            vector<type> operator=(const type& processedValue)
            {
                fill(processedValue);
                return *this;
            }

            vector<type> operator+=(const type& processedValue)
            {
                *this = process([processedValue](const type& checkedValue){ return checkedValue + processedValue; });
                return *this;
            }

            vector<type> operator-=(const type& processedValue)
            {
                *this = process([processedValue](const type& checkedValue){ return checkedValue - processedValue; });
                return *this;
            }

            vector<type> operator*=(const type& processedValue)
            {
                *this = process([processedValue](const type& checkedValue){ return checkedValue * processedValue; });
                return *this;
            }

            vector<type> operator/=(const type& processedValue)
            {
                *this = process([processedValue](const type& checkedValue){ return checkedValue / processedValue; });
                return *this;
            }

            vector<type> operator%=(const type& processedValue)
            {
                *this = process([processedValue](const type& checkedValue){ return checkedValue % processedValue; });
                return *this;
            }

            vector<type> operator&&(const type& processedValue)
            {
                return process([processedValue](const type& checkedValue){ return checkedValue && processedValue; });
            }

            vector<type> operator||(const type& processedValue)
            {
                return process([processedValue](const type& checkedValue){ return checkedValue || processedValue; });
            }

            vector<type> operator!()
            {
                return process([](const type& checkedValue){ return !checkedValue; });
            }

            vector<type> operator&(const type& processedValue)
            {
                return process([processedValue](const type& checkedValue){ return checkedValue & processedValue; });
            }

            vector<type> operator|(const type& processedValue)
            {
                return process([processedValue](const type& checkedValue){ return checkedValue | processedValue; });
            }

            vector<type> operator^(const type& processedValue)
            {
                return process([processedValue](const type& checkedValue){ return checkedValue ^ processedValue; });
            }

            vector<type> operator<<(const type& processedValue)
            {
                return process([processedValue](const type& checkedValue){ return checkedValue << processedValue; });
            }

            vector<type> operator>>(const type& processedValue)
            {
                return process([processedValue](const type& checkedValue){ return checkedValue >> processedValue; });
            }

            vector<type> operator<<(const intmax_t& processedValue)
            {
                return process([processedValue](const type& checkedValue){ return checkedValue << processedValue; });
            }

            vector<type> operator>>(const intmax_t& processedValue)
            {
                return process([processedValue](const type& checkedValue){ return checkedValue >> processedValue; });
            }

            // const type* const begin() const
            // {
            //     return (const type &)&array[0];
            // }

            // const type* const end() const
            // {
            //     return (const type &)&array[arraySize];
            // }
            
            const type* begin() const
            {
                return &array[0];
            }

            const type* end() const
            {
                return &array[arraySize];
            }

            type* begin()
            {
                return &array[0];
            }

            type* end()
            {
                return &array[arraySize];
            }
    };

    template<class type>
    inline vector<type> range(type first, type last)
    {
        vector<type> newVector;
        for(type i = first; i < last; ++i) newVector.append(i);
        return newVector;
    }

    // /**
    //  * @brief Variable without specified type.
    //  * 
    //  */
    // struct untypized
    // {
    //     private:
    //         void* variable = nullptr;
    //         size_t capacity = 0;

    //         void resize(const size_t& newCapacity)
    //         {
    //             if(newCapacity == capacity) return;
    //             if(newCapacity == 0)
    //             {
    //                 free(variable);
    //                 variable = nullptr;
    //                 capacity = 0;
    //                 return;
    //             }

    //             if(variable != nullptr)
    //             {
    //                 void* newVariable = realloc(variable, newCapacity);
    //                 if(newVariable == nullptr)
    //                 {
    //                     // free(variable);
    //                     variable = malloc(newCapacity);
    //                 }
    //                 else
    //                 {
    //                     variable = newVariable;
    //                 }
    //             }
    //             else
    //             {
    //                 // free(variable);
    //                 variable = malloc(newCapacity);
    //             }
    //             capacity = newCapacity;
    //         }
    //     public:

    //         const size_t& size = capacity;
            
    //         untypized()
    //         {
    //             capacity = 0;
    //             variable = nullptr;
    //             resize(0);
    //         }

    //         template<class type>
    //         untypized(const type& value)
    //         {
    //             resize(sizeof(value));
    //             *((type*)variable) = (type&)value;
    //         }

    //         untypized(const untypized& value)
    //         {
    //             resize(value.size);
    //             if(value.size != 0) *((char*)variable) = (char&)value;
    //         }

    //         untypized& operator=(const untypized& value)
    //         {
    //             if(&value == this) return *this;
    //             resize(value.size);
    //             if(value.size != 0) *((char*)variable) = (char&)value;
    //             return *this;
    //         }

    //         untypized(const untypized&& value)
    //         {
    //             resize(value.size);
    //             if(value.size != 0) *((char*)variable) = (char&)value;
    //         }

    //         untypized& operator=(const untypized&& value)
    //         {
    //             if(&value == this) return *this;
    //             resize(value.size);
    //             if(value.size != 0) *((char*)variable) = (char&)value;
    //             return *this;
    //         }

    //         template<class type>
    //         untypized& operator=(const type& value)
    //         {
    //             resize(sizeof(type));
    //             *((type*)variable) = value;
    //             return *this;
    //         }

    //         template<class type>
    //         operator type&() const
    //         {
    //             return *((type*)variable);
    //         }

    //         // template<class type>
    //         // operator type()
    //         // {
    //         //     return *((type*)variable);
    //         // }

    //         ~untypized()
    //         {
    //             if(variable != nullptr) free(variable);
    //         }
    // };
};
