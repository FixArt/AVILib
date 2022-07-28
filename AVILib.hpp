#include <cmath>
#include <initializer_list>
#include <functional>
#include <algorithm>
#include <type_traits>

/**
 * @brief Namespace of Alternative Vector (And pair... and shared_ptr... and other things?) Implementation Library.
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

            template<class unwrapped, class... arguments>
            vector<unwrapped> unwrap(arguments... readArguments)
            {
                vector<unwrapped> checked;
                checked.append(readArguments...);
                return checked;
            }
        public:

            vector<type>() : array{nullptr}, arraySize{0} {}

            const size_t &size = arraySize;

            vector<type>& operator=(std::initializer_list<type> initializationArray)
            {
                // for(type newElement : initializationArray)
                // {
                //     append(newElement);
                // }
                resize(initializationArray.size());
                std::copy(initializationArray.begin(), initializationArray.end(), begin());
                return *this;
            }

            vector<type>(std::initializer_list<type> initializationArray)
            {
                // for(type newElement : initializationArray)
                // {
                //     append(newElement);
                // }
                resize(initializationArray.size());
                std::copy(initializationArray.begin(), initializationArray.end(), begin());
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
             * @brief Add element to vector by index without preserving order.
             * 
             * @param newElement Element itself.
             * @param index Index at which it will be placed.
             */
            void fpush(const type &newElement, const size_t &index)
            {
                if(index > size) return;
                resize(size + 1);
                swap(array[index], array[arraySize - 1]);
                array[index] = newElement;
            }

            template<class... arguments>
            void fpush(const type &newElement, const size_t &index, arguments... newElements)
            {
                fpush(newElement, index);
                fpush(newElements...);
            }

            /**
             * @brief Function, which places vector at index without preserving order.
             * 
             * @param pushedVector Vector which will be placed at index.
             * @param index Index at which vector will be placed.
             */
            void fpush(vector<type> pushedVector, size_t index = 0)
            {
                if(index > size) return;
                resize(arraySize + pushedVector.size);
                for(size_t i = 0; i < pushedVector.size; ++i)
                {
                    // fpush(pushedVector[i], i + index);
                    swap(array[index + i], array[arraySize - pushedVector.size + i]);
                    array[index + i] = pushedVector[i];
                }
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
             * @brief Exclude element from array by index
             * 
             * @param index Index, at which element will be excluded from array.
             */
            void fpop(const size_t &index)
            {
                if(index >= size or size == 0) return;
                swap(array[index], array[size - 1]);
                reduce(1);
            }

            void pop(const vector<size_t>& poppedIndexes)
            {
                // for(size_t index : poppedIndexes)
                // {
                //     // pop(index);
                // }
                size_t found = 0;
                for(size_t index : poppedIndexes)
                {
                    size_t j = 0;
                    for(size_t i = 0; i < size; ++i)
                    {
                        if(index == i)
                        {
                            ++found;
                            continue;
                        }
                        array[j] = array[i];
                        ++j;
                    }
                }
                reduce(found);
            }

            void fpop(vector<size_t> poppedIndexes)
            {
                size_t found = 0;
                poppedIndexes.sort();
                for(size_t index : poppedIndexes)
                {
                    if(index - found >= size - 1 - found) break;
                    swap(array[index - found], array[size - 1 - found]);
                    ++found;
                }
                reduce(found);
            }

            void ufpop(vector<size_t> poppedIndexes)
            {
                size_t found = 0;
                for(size_t index : poppedIndexes)
                {
                    swap(array[index - found], array[size - 1 - found]);
                    ++found;
                }
                reduce(found);
            }

            template<class... arguments>
            void pop(arguments... removedIndexes)
            {
                vector<size_t> unwrapped = unwrap<size_t>(removedIndexes...);
                unwrapped.sort();
                size_t found = 0;
                for(size_t index : unwrapped)
                {
                    pop(index - found);
                    ++found;
                }
            }

            template<class... arguments>
            void fpop(arguments... removedIndexes)
            {
                // pop(removedIndexes...);
                // pop(index);
                vector<size_t> unwrapped = unwrap<size_t>(removedIndexes...);
                unwrapped.sort();
                size_t found = 0;
                for(size_t index : unwrapped)
                {
                    if(index - found >= size - 1 - found) break;
                    swap(array[index - found], array[size - 1 - found]);
                    ++found;
                }
                reduce(found);
            }

            template<class... arguments>
            void ufpop(arguments... removedIndexes)
            {
                vector<size_t> unwrapped = unwrap<size_t>(removedIndexes...);
                size_t found = 0;
                for(size_t index : unwrapped)
                {
                    swap(array[index - found], array[size - 1 - found]);
                    ++found;
                }
                reduce(found);
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
                // if(size - 1 == 0)
                // {
                //     return;
                // }
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
                // if(size - 1 == 0)
                // {
                //     return;
                // }
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
             * @brief Remove all instances which equal to given.
             * 
             * @param checkedElement Element searched.
             */
            void fremove(const type& checkedElement)
            {
                if(size == 0)
                {
                    return;
                }
                size_t counted = 0;
                while(where(checkedElement) != size)
                {
                    //fpop(where(checkedElement));
                    swap(array[where(checkedElement)], array[size - 1 - counted]);
                    ++counted;
                }
                reduce(counted);
            }

            /**
             * @brief Remove all instances which marked by given function without preserving ordering.
             * 
             * @param shouldRemove Function to check whenever element should be removed.
             */
            void fremove(bool(shouldRemove)(const type&))
            {
                if(size == 0)
                {
                    return;
                }
                size_t counted = 0;
                while(where(shouldRemove) != size)
                {
                    //fpop(where(shouldRemove));
                    swap(array[where(shouldRemove)], array[size - 1 - counted]);
                    ++counted;
                }
                reduce(counted);
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

            // /**
            //  * @brief Composes new vector from all elements which flagged by given function.
            //  * 
            //  * @param shouldInclude Checks whenever element should be included.
            //  * @return vector<type> Returned vector.
            //  */
            // vector<type> compose(bool(shouldInclude)(const type&)) const
            // {
            //     vector<type> newVector;
            //     for(size_t i = 0; i < size; ++i)
            //     {
            //         if(shouldInclude(array[i]))
            //         {
            //             newVector.append(array[i]);
            //         }
            //     }
            //     return newVector;
            // }

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

            // /**
            //  * @brief Composes new vector out of elements of old vector which were processed by specific function.
            //  *
            //  * @param processFunction Function, which changes value.
            //  * @return vector<type> Returned vector.
            //  */
            // vector<type> process(type(processFunction)(const type&))
            // {
            //     vector<type> newVector;
            //     for(size_t i = 0; i < size; ++i)
            //     {
            //         newVector.append(processFunction(array[i]));
            //     }
            //     return newVector;
            // }

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

            // /**
            //  * @brief Fills vector through given function.
            //  * 
            //  * @param processFunction Function which gets position as argument.
            //  */
            // void fill(type(processFunction)(const size_t&))
            // {
            //     for(size_t i = 0; i < size; ++i)
            //     {
            //         array[i] = processFunction(i);
            //     }
            // }

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

            // /**
            //  * @brief Fills vector through given function.
            //  * 
            //  * @param processFunction Given function.
            //  */
            // void fill(type(processFunction)())
            // {
            //     for(size_t i = 0; i < size; ++i)
            //     {
            //         array[i] = processFunction();
            //     }
            // }

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

            // vector<type> order(size_t(processFunction)(const size_t&)) const
            // {
            //     vector<type> newVector;
            //     for(size_t i = 0; i < size; ++i)
            //     {
            //         newVector[processFunction(i)] = array[i];
            //     }
            //     return newVector;
            // }

            // vector<type> order(size_t(processFunction)(const size_t&, const type&)) const
            // {
            //     vector<type> newVector;
            //     for(size_t i = 0; i < size; ++i)
            //     {
            //         newVector[processFunction(i, array[i])] = array[i];
            //     }
            //     return newVector;
            // }

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

            // bool isComposed(const vector<type>& checkedVector, bool(shouldInclude)(const type&)) const
            // {
            //     return checkedVector == compose(shouldInclude);
            // }

            bool isComposed(const vector<type>& checkedVector, std::function<bool(const type&)> shouldInclude) const
            {
                return checkedVector == compose(shouldInclude);
            }

            // bool isComposed(const vector<type>& checkedVector, bool(shouldInclude)(const type&, size_t)) const
            // {
            //     return checkedVector == compose(shouldInclude);
            // }

            bool isComposed(const vector<type>& checkedVector, std::function<bool(const type&, size_t)> shouldInclude) const
            {
                return checkedVector == compose(shouldInclude);
            }

            // bool isProcessed(const vector<type>& checkedVector, bool(processFunction)(const type&)) const
            // {
            //     return checkedVector == process(processFunction);
            // }

            bool isProcessed(const vector<type>& checkedVector, std::function<bool(const type&)> processFunction) const
            {
                return checkedVector == process(processFunction);
            }

            // bool isProcessed(const vector<type>& checkedVector, bool(processFunction)(const type&, size_t)) const
            // {
            //     return checkedVector == process(processFunction);
            // }

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

            // bool isFilled(bool(processFunction)(const type&)) const
            // {
            //     vector<type> newVector;
            //     newVector.resize(size);
            //     return *this == newVector.fill(processFunction);
            // }

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

            // bool isOrdered(const vector<type>& checkedVector, size_t(processFunction)(const size_t&)) const
            // {
            //     return checkedVector == order(processFunction);
            // }

            // bool isOrdered(const vector<type>& checkedVector, size_t(processFunction)(const size_t&, const type&)) const
            // {
            //     return checkedVector == order(processFunction);
            // }

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
             * @brief Returns copy of a vector. Same operation as if the vector would be just assigned to another vector.
             * 
             * @return vector<type> New vector.
             */
            vector<type> copy()
            {
                vector<type> newVector;
                newVector = *this;
                return newVector;
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
             * @brief Returns constant array from itself. Useful for strings printing.
             * 
             * @return type* Returned constant array.
             */
            operator const type* const() const
            {
                if(array == nullptr) throw(EFAULT);
                return (const type* const)array;
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

            /**
             * @brief Returns constant array from itself. Useful for strings printing.
             * 
             * @return type* Returned constant array.
             */
            operator type*()
            {
                if(array == nullptr) throw(EFAULT);
                return (type*)array;
            }

            operator type&()
            {
                if(array == nullptr) throw(EFAULT);
                return (type&)array;
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

            /**
             * @brief Appends vector.
             * 
             * @param appendedVector Vector to append.
             */
            void fappend(const vector<type>& appendedVector)
            {
                fpush(appendedVector, size);
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

            template<class... arguments>
            void fappend(const vector<type>& newElement, arguments... appendedElements)
            {
                fappend(newElement);
                fappend(appendedElements...);
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

            vector<type>(vector<type>&& assignedVector)
            {
                // for(size_t i = 0; i < assignedVector.size; ++i)
                // {
                //     append(assignedVector[i]);
                // }
                // memcpy(array, assignedVector, assignedVector.size * sizeof(type));
                //resize(assignedVector.size);
                //std::move(assignedVector.begin(), assignedVector.end(), (*this).begin());
                //assignedVector.clear();
                array = assignedVector.array;
                arraySize = assignedVector.arraySize;
                assignedVector.array = nullptr;
                assignedVector.arraySize = 0;
                //return *this;
            }

            vector<type>(const type* const assignedArray, size_t assignedSize)
            {
                // for(size_t i = 0; i < assignedSize; ++i)
                // {
                //     append(assignedArray[i]);
                // }
                // memcpy(array, assignedArray, assignedSize * sizeof(type));
                resize(assignedSize);
                std::copy(&(assignedArray[0]), &(assignedArray[assignedSize]), begin());
            }

            vector<type>& createFrom(const type* const assignedArray, size_t assignedSize)
            {
                // for(size_t i = 0; i < assignedSize; ++i)
                // {
                //     append(assignedArray[i]);
                // }
                // memcpy(array, assignedArray, assignedSize * sizeof(type));
                resize(assignedSize);
                std::copy(&(assignedArray[0]), &(assignedArray[assignedSize]), begin());
                return *this;
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

            size_t unsorted()
            {
                return std::is_sorted_until(begin(), end());
            }

            size_t unsorted(bool(comparer)(const type&, const type&))
            {
                return std::is_sorted_until(begin(), end(), comparer);
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

    /**
     * Counter for pointers, which only can be sent by reference. Non explicit copying disallowed.
     */
    template<class countType = size_t>
    class counter
    {
        private:
            countType count = 0;

        public:
            // counter() : count(0) {}

            counter(countType settedCount = 0) : count(settedCount) {}

            explicit counter<countType>(const counter<countType>& copied)
            {
                count = copied.count;
            }

            counter& operator+=(const countType& number)
            {
                count += number;
                return *this;
            }

            counter& operator-=(const countType& number)
            {
                if(std::is_unsigned<countType>() and number >= count) count = 0;
                else count -= number;
                count -= number;
                return *this;
            }

            counter& operator*=(const countType& number)
            {
                count *= number;
                return *this;
            }

            counter& operator/=(const countType& number)
            {
                count /= number;
                return *this;
            }

            counter& operator=(const countType& number)
            {
                count = number;
                return *this;
            }

            bool operator==(const countType& number)
            {
                return count == number;
            }

            counter& operator++()
            {
                ++count;
                return *this;
            }

            counter& operator--()
            {
                if(!std::is_unsigned<countType>() or count != 0)
                {
                    --count;
                }
                return *this;
            }

            // operator countType()
            // {
            //     return count;
            // }

            operator const countType&()
            {
                return count;
            }
    };

    template<class type>
    struct unique_ptr
    {
        private:

            type* pointed;

        public:

            unique_ptr(type* pointer = nullptr) : pointed{pointer} {}

            unique_ptr(unique_ptr& copied)
            {
                pointed = copied.pointed;
                copied.pointed = nullptr;
            }

            unique_ptr(unique_ptr&& copied)
            {
                pointed = copied.pointed;
                copied.pointed = nullptr;
            }

            // const type& operator[](size_t index)
            // {
            //     return pointed[index];
            // }

            type& operator*()
            {
                return *pointed;
            }

            type* operator->()
            {
                return pointed;
            }

            const type& operator*() const
            {
                return *pointed;
            }

            const type* const operator->() const
            {
                return pointed;
            }

            operator bool() const
            {
                return pointed != nullptr;
            }

            operator const type* const() const
            {
                return pointed;
            }

            operator type*()
            {
                return pointed;
            }

            ~unique_ptr()
            {
                if(pointed != nullptr) delete pointed;
            }
    };

    template<class type>
    struct shared_ptr
    {
        private:

            type* pointed;

            counter<size_t>* point;

        public:

            // shared_ptr() : pointed{nullptr}, point{new counter<size_t>{1}} {}

            shared_ptr(type* pointer = nullptr) : pointed{pointer}, point{new counter<size_t>[2]} { ++(point[0]); }

            shared_ptr(type* pointer, counter<size_t>* otherPoint) : pointed{pointer}, point{otherPoint} { ++(point[0]); }

            shared_ptr<type>(const shared_ptr<type>& copied)
            {
                pointed = copied.pointed;
                point = copied.point;
                ++(point[0]);
            }

            shared_ptr<type>(shared_ptr<type>&& copied)
            {
                pointed = copied.pointed;
                point = copied.point;
                copied.pointed = nullptr;
                copied.point = new counter<size_t>[2];
                ++(copied.point[0]);
            }

            shared_ptr& operator=(shared_ptr& copied)
            {
                if(copied == *this) return *this;
                --(point[0]);
                if((point[0]) == (size_t)0)
                {
                    if(pointed != nullptr) delete pointed;
                    if((point[1]) == (size_t)0) delete[] point;
                }
                pointed = copied.pointed;
                point = copied.point;
                ++(point[0]);
                return *this;
            }

            bool unique()
            {
                return use_count() == 1;
            }

            // const type& operator[](size_t index)
            // {
            //     return pointed[index];
            // }

            shared_ptr& operator=(type* copied)
            {
                --(point[0]);
                if((point[0]) == (size_t)0)
                {
                    if(pointed != nullptr) delete pointed;
                    if((point[1]) == (size_t)0) delete[] point;
                }
                pointed = copied;
                point = new counter<size_t>[2];
                ++(point[0]);
                return *this;
            }

            operator bool() const
            {
                return pointed != nullptr;
            }

            type& operator*()
            {
                return *pointed;
            }

            type* operator->()
            {
                return pointed;
            }

            size_t use_count()
            {
                return (size_t)(point[0]);
            }

            counter<size_t>* getCounter()
            {
                return point;
            }

            const type& operator*() const
            {
                return *pointed;
            }

            // const type* const operator->() const
            // {
            //     return pointed;
            // }

            operator const type* const() const
            {
                return pointed;
            }

            operator type*()
            {
                return pointed;
            }

            ~shared_ptr()
            {
                --(point[0]);
                if((point[0]) == (size_t)0)
                {
                    if(pointed != nullptr) delete pointed;
                    if((point[1]) == (size_t)0) delete[] point;
                }
            }
    };

    template<class type>
    struct weak_ptr
    {
        private:
            type* stored;

            counter<size_t>* point;

        public:
            weak_ptr(shared_ptr<type>& store) : stored{(type*)store}, point{store.getCounter()} { ++(point[1]); }

            weak_ptr(shared_ptr<type>* store) : stored{(type*)(*store)}, point{store->getCounter()} { ++(point[1]); }

            weak_ptr(weak_ptr<type>& store) : stored{store.stored}, point{store.point} { ++(point[1]); }

            weak_ptr(weak_ptr<type>* store) : stored{store->stored}, point{store->point} { ++(point[1]); }

            bool expired()
            {
                return point[0] == (size_t)0;
            }

            shared_ptr<type> lock()
            {
                return expired() ? shared_ptr<type>{} : shared_ptr<type>{stored, point};
            }

            counter<size_t>* getCounter()
            {
                return point;
            }

            size_t use_count()
            {
                return (size_t)(point[0]);
            }

            ~weak_ptr()
            {
                --(point[1]);
                if((point[0]) == (size_t)0 and (point[1]) == (size_t)0)
                {
                    delete[] point;
                }
            }
    };

    /**
     * @brief Simple object pointers.
     * 
     * @tparam type Returned type.
     */
    template<class type>
    struct objptr
    {
        private:
            type* pointedObject = nullptr;
        public:
            objptr<type>() : pointedObject{new type} {}

            objptr<type>(const type& copied)
            {
                if(pointedObject != nullptr) delete pointedObject;
                pointedObject = new type;
                *pointedObject = copied;
            }

            objptr<type>(const objptr<type>& copied)
            {
                if(pointedObject != nullptr) delete pointedObject;
                pointedObject = new type;
                *pointedObject = (const type&)copied;
            }

            // objptr<type>(type* assigned)
            // {
            //     if(pointedObject != nullptr) delete pointedObject;
            //     pointedObject = assigned;
            // }

            objptr<type> operator=(const objptr<type>& copied)
            {
                if(pointedObject != nullptr) delete pointedObject;
                pointedObject = new type;
                *pointedObject = (const type&)copied;
                return *this;
            }

            objptr<type> operator=(const type& copied)
            {
                if(pointedObject != nullptr) delete pointedObject;
                pointedObject = new type;
                *pointedObject = copied;
            }

            // objptr<type> operator=(type* assigned)
            // {
            //     if(pointedObject != nullptr) delete pointedObject;
            //     pointedObject = assigned;
            // }

            ~objptr<type>()
            {
                if(pointedObject != nullptr) delete pointedObject;
            }

            operator type*()
            {
                return pointedObject;
            }

            operator type&()
            {
                if(pointedObject == nullptr)
                {
                    pointedObject = new type;
                }
                return *pointedObject;
            }

            type& operator*()
            {
                return *pointedObject;
            }

            type* operator->()
            {
                return pointedObject;
            }

            const type* const operator->() const
            {
                return pointedObject;
            }

            operator const type* const() const
            {
                return pointedObject;
            }

            operator const type&() const
            {
                if(pointedObject == nullptr)
                {
                    throw(EFAULT);
                }
                return *pointedObject;
            }
            
            const type* begin() const
            {
                return pointedObject;
            }

            const type* end() const
            {
                return pointedObject + 1;
            }

            type* begin()
            {
                return pointedObject;
            }

            type* end()
            {
                return pointedObject + 1;
            }
    };

    template<class first, class second>
    struct pair
    {
        private:
            void* variable = nullptr;
            size_t currentType = 0;

            void resize(const size_t& newSize)
            {
                if(variable != nullptr)
                {
                    if(currentType == typeid(first).hash_code())
                    {
                        ((first*)variable)->~first();
                    }
                    else
                    {
                        ((second*)variable)->~second();
                    }
                    free(variable);
                    variable = nullptr;
                    if(newSize == 0) return;
                }
                variable = malloc(newSize);
                if(variable == nullptr) throw(ENOMEM);
            }

            void clean()
            {
                if(variable != nullptr)
                {
                    if(currentType == typeid(first).hash_code())
                    {
                        ((first*)variable)->~first();
                        // free((first*)variable);
                    }
                    else
                    {
                        ((second*)variable)->~second();
                        // free((second*)variable);
                    }
                    free(variable);
                    variable = nullptr;
                    currentType = 0;
                }
            }

            template<class type>
            void turnTo()
            {
                if(typeid(type) != typeid(first) and typeid(type) != typeid(second))
                {
                    throw(EINVAL);
                }
                resize(sizeof(type));
                new (&variable) type;
                currentType = typeid(type).hash_code();
            }
        public:
            pair<first, second>() : variable{nullptr}, currentType{0} {}

            const size_t& currentHash() const
            {
                return currentType;
            }

            template<class type>
            pair<first, second>(const type& assigned)
            {
                turnTo<type>();
                *((type*)variable) = (type&)assigned;
            }

            template<class alternativeFirst, class alternativeSecond>
            pair<first, second>(const pair<alternativeFirst, alternativeSecond>& copied)
            {
                if(typeid(first).hash_code() != copied.currentHash() and typeid(second).hash_code() != copied.currentHash())
                {
                    throw(EINVAL);
                }
                if(typeid(first).hash_code() == copied.currentHash())
                {
                    turnTo<first>();
                    *((first*)variable) = (first&)copied;
                }
                else
                {
                    turnTo<second>();
                    *((second*)variable) = (second&)copied;
                }
                currentType = copied.currentHash();
            }

            pair<first, second>(const pair<first, second>& copied)
            {
                if(typeid(first).hash_code() != copied.currentHash() and typeid(second).hash_code() != copied.currentHash())
                {
                    throw(EINVAL);
                }
                if(typeid(first).hash_code() == copied.currentHash())
                {
                    turnTo<first>();
                    *((first*)variable) = (first&)copied;
                }
                else
                {
                    turnTo<second>();
                    *((second*)variable) = (second&)copied;
                }
                currentType = copied.currentHash();
            }

            template<class type>
            pair<first, second> operator=(const type& assigned)
            {
                turnTo<type>();
                *((type*)variable) = (type&)assigned;
                return *this;
            }

            template<class alternativeFirst, class alternativeSecond>
            pair<first, second> operator=(const pair<alternativeFirst, alternativeSecond>& copied)
            {
                if(typeid(first).hash_code() != copied.currentHash() and typeid(second).hash_code() != copied.currentHash())
                {
                    throw(EINVAL);
                }
                if(typeid(first).hash_code() == copied.currentHash())
                {
                    turnTo<first>();
                    *((first*)variable) = (first&)copied;
                }
                else
                {
                    turnTo<second>();
                    *((second*)variable) = (second&)copied;
                }
                currentType = copied.currentHash();
                return *this;
            }

            pair<first, second> operator=(const pair<first, second>& copied)
            {
                if(typeid(first).hash_code() != copied.currentHash() and typeid(second).hash_code() != copied.currentHash())
                {
                    throw(EINVAL);
                }
                if(typeid(first).hash_code() == copied.currentHash())
                {
                    turnTo<first>();
                    *((first*)variable) = (first&)copied;
                }
                else
                {
                    turnTo<second>();
                    *((second*)variable) = (second&)copied;
                }
                currentType = copied.currentHash();
                return *this;
            }

            // template<class type>
            // operator type&()
            // {
            //     if(currentHash() != typeid(type).hash_code())
            //     {
            //         throw(EINVAL);
            //     }
            //     return *((type*)variable);
            // }

            // template<class type>
            // operator const type&() const
            // {
            //     if(currentType != typeid(type).hash_code())
            //     {
            //         throw(EINVAL);
            //     }
            //     return *((type*)variable);
            // }

            // template<class type>
            // operator type&()
            // {
            //     if(currentHash() == typeid(first).hash_code())
            //     {
            //         return *((first*)variable);
            //     }
            //     else if(currentHash() == typeid(second).hash_code())
            //     {
            //         return *((second*)variable);
            //     }
            //     else throw(EINVAL);
            // }

            // template<class type>
            // operator type&() const
            // {
            //     if(currentHash() == typeid(first).hash_code())
            //     {
            //         return *((first*)variable);
            //     }
            //     else if(currentHash() == typeid(second).hash_code())
            //     {
            //         return *((second*)variable);
            //     }
            //     else throw(EINVAL);
            // }

            operator first&()
            {
                if(currentHash() != typeid(first).hash_code())
                {
                    throw(EINVAL);
                }
                return *((first*)variable);
            }

            operator const first&() const
            {
                if(currentType != typeid(first).hash_code())
                {
                    throw(EINVAL);
                }
                return *((first*)variable);
            }

            operator second&()
            {
                if(currentHash() != typeid(second).hash_code())
                {
                    throw(EINVAL);
                }
                return *((second*)variable);
            }

            operator const second&() const
            {
                if(currentType != typeid(second).hash_code())
                {
                    throw(EINVAL);
                }
                return *((second*)variable);
            }

            ~pair<first, second>()
            {
                if(variable != nullptr)
                {
                    if(currentType == typeid(first).hash_code())
                    {
                        ((first*)variable)->~first();
                        // free((first*)variable);
                    }
                    else
                    {
                        ((second*)variable)->~second();
                        // free((second*)variable);
                    }
                    free(variable);
                }
            }
    };
    
    /**
     * @brief Variant implementation.
     * 
     * @tparam types Types of variant.
     */
    template<class... types>
    struct variant
    {
        private:
            void* variable = nullptr;
            size_t currentType = 0;

            // template<class checkedType>
            // bool availableIterator() const
            // {
            //     return false;
            // }

            template<class checkedType, class processedType>
            bool availableIterator() const
            {
                return typeid(processedType) == typeid(checkedType);
            }

            template<class checkedType, class processedType, class additionalType, class... availableTypes>
            bool availableIterator() const
            {
                //if(sizeof...(availableTypes) == 0) return typeid(processedType) == typeid(checkedType);
                return (typeid(processedType) == typeid(checkedType))?(true):(availableIterator<checkedType, additionalType, availableTypes...>());
            }

            template<class processedType>
            bool availableIteratorHash(const size_t& checkedHash) const
            {
                return typeid(processedType).hash_code() == checkedHash;
            }

            template<class processedType, class additionalType, class... availableTypes>
            bool availableIteratorHash(const size_t& checkedHash) const
            {
                // if(sizeof...(availableTypes) == 0) return typeid(processedType).hash_code() == checkedHash;
                return (typeid(processedType).hash_code() == checkedHash)?(true):(availableIteratorHash<additionalType, availableTypes...>(checkedHash));
            }

            // template<class processedType>
            // void assignIterator(const variant<types...>& copied)
            // {
            //     if(copied.currentHash() == typeid(processedType).hash_code())
            //     {
            //         turnTo<processedType>();
            //         *((processedType*)variable) = (processedType&)copied;
            //     }
            // }

            // template<class processedType, class additionalType, class... assignedTypes>
            // void assignIterator(const variant<types...>& copied)
            // {
            //     if(copied.currentHash() == typeid(processedType).hash_code())
            //     {
            //         turnTo<processedType>();
            //         *((processedType*)variable) = (processedType&)copied;
            //     }
            //     else
            //     {
            //         // if(sizeof...(assignedTypes) == 0) return;
            //         assignIterator<additionalType, assignedTypes...>(copied);
            //     }
            // }

            template<class processedType>
            void assignIterator(const variant& copied)
            {
                if(copied.currentHash() == typeid(processedType).hash_code())
                {
                    turnTo<processedType>();
                    *((processedType*)variable) = (processedType)copied;
                }
            }

            template<class processedType, class additionalType, class... assignedTypes>
            void assignIterator(const variant& copied)
            {
                if(copied.currentHash() == typeid(processedType).hash_code())
                {
                    turnTo<processedType>();
                    *((processedType*)variable) = (processedType)copied;
                }
                else
                {
                    // if(sizeof...(assignedTypes) == 0) return;
                    assignIterator<additionalType, assignedTypes...>(copied);
                }
            }

            // template<class... otherTypes, class processedType>
            // void assignIteratorOther(const variant<otherTypes...>& copied)
            // {
            //     if(copied.currentHash() == typeid(processedType).hash_code())
            //     {
            //         turnTo<processedType>();
            //         *((processedType*)variable) = (processedType&)copied;
            //     }
            // }

            // template<class... otherTypes, class processedType, class additionalType, class... assignedTypes>
            // void assignIteratorOther(const variant<otherTypes...>& copied)
            // {
            //     if(copied.currentHash() == typeid(processedType).hash_code())
            //     {
            //         turnTo<processedType>();
            //         *((processedType*)variable) = (processedType&)copied;
            //     }
            //     else
            //     {
            //         // if(sizeof...(assignedTypes) == 0) return; //So no conflict happened.
            //         assignIteratorOther<otherTypes..., additionalType, assignedTypes...>(copied);
            //     }
            // }

            template<class processedType>
            void cleanIterator()
            {
                if(typeid(processedType) == typeid(currentType))
                {
                    ((processedType*)variable)->~processedType();
                }
            }

            template<class processedType, class additionalOne, class... cleanedTypes>
            void cleanIterator()
            {
                if(typeid(processedType) == typeid(currentType))
                {
                    ((processedType*)variable)->~processedType();
                }
                else
                {
                    // if(sizeof...(cleanedTypes) == 0) return;
                    cleanIterator<additionalOne, cleanedTypes...>();
                }
            }

            void clean()
            {
                if(variable != nullptr)
                {
                    cleanIterator<types...>();
                    free(variable);
                    variable = nullptr;
                }
            }

            void resize(const size_t& newSize)
            {
                if(variable != nullptr)
                {
                    clean();
                    if(newSize == 0) return;
                }
                variable = malloc(newSize);
                if(variable == nullptr) throw(ENOMEM);
            }

            template<class type>
            void turnTo()
            {
                if(!isAvailable<type>())
                {
                    throw(EINVAL);
                }
                resize(sizeof(type));
                new (&variable) type;
                currentType = typeid(type).hash_code();
            }

            // void assign(const variant<types...>& copied)
            // {
            //     if(!isAvailable(copied.currentHash()))
            //     {
            //         throw(EINVAL);
            //     }
            //     assignIterator<types...>(copied);
            //     currentType = copied.currentHash();
            // }

            // template<class... otherTypes>
            // void assign(const variant<otherTypes...>& copied)
            // {
            //     if(!isAvailable(copied.currentHash()))
            //     {
            //         throw(EINVAL);
            //     }
            //     // assignIteratorOther<otherTypes..., types...>(copied);
            //     assignIterator<types...>(copied);
            //     currentType = copied.currentHash();
            // }

            void assign(const variant& copied)
            {
                if(!isAvailable(copied.currentHash()))
                {
                    throw(EINVAL);
                }
                assignIterator<types...>(copied);
                currentType = copied.currentHash();
            }

        public:
            template<class type>
            bool isAvailable() const
            {
                return availableIterator<type, types...>();
            }

            bool isAvailable(const size_t& checkedHash) const
            {
                return availableIteratorHash<types...>(checkedHash);
            }

            variant<types...>() : variable{nullptr}, currentType{0} {}

            const size_t& currentHash() const
            {
                return currentType;
            }

            template<class type>
            variant<types...>(const type& assigned)
            {
                turnTo<type>();
                *((type*)variable) = (type&)assigned;
            }

            // template<class... otherTypes>
            // variant<types...>(const variant<otherTypes...>& copied)
            // {
            //     // assign<otherTypes...>(copied);
            //     assign(copied);
            //     // currentType = copied.currentHash();
            // }

            // variant<types...>(const variant<types...>& copied)
            // {
            //     assign(copied);
            //     // currentType = copied.currentHash();
            // }

            variant(const variant& copied)
            {
                assign(copied);
                // currentType = copied.currentHash();
            }

            variant(variant&& assigned)
            {
                // assign(assigned);
                // assigned.clean();
                // currentType = copied.currentHash();
                variable = assigned.variable;
                currentType = assigned.currentType;
                assigned.variable = nullptr;
                assigned.currentType = 0;
            }

            template<class type>
            variant<types...> operator=(const type& assigned)
            {
                turnTo<type>();
                *((type*)variable) = (type&)assigned;
                return *this;
            }

            // template<class... otherTypes>
            // variant<types...> operator=(const variant<otherTypes...>& copied)
            // {
            //     // assign<otherTypes...>(copied);
            //     assign(copied);
            //     // currentType = copied.currentHash();
            //     return *this;
            // }

            // variant<types...> operator=(const variant<types...>& copied)
            // {
            //     assign(copied);
            //     // currentType = copied.currentHash();
            //     return *this;
            // }

            variant operator=(const variant& copied)
            {
                assign(copied);
                // currentType = copied.currentHash();
                return *this;
            }

            template<class type>
            operator type&()
            {
                if(currentHash() != typeid(type).hash_code())
                {
                    throw(EINVAL);
                }
                return *((type*)variable);
            }

            template<class type>
            operator const type&() const
            {
                if(currentType != typeid(type).hash_code())
                {
                    throw(EINVAL);
                }
                return *((type*)variable);
            }

            ~variant<types...>()
            {
                clean();
            }
    };

    /**
     * @brief Variable without specified type only for trivial types.
     * 
     */
    struct untypizedtrivial
    {
        private:
            void* variable = nullptr;
            size_t capacity = 0;

            void resize(const size_t& newCapacity)
            {
                if(newCapacity == capacity) return;
                if(newCapacity == 0)
                {
                    free(variable);
                    variable = nullptr;
                    capacity = 0;
                    return;
                }

                if(variable != nullptr)
                {
                    void* newVariable = realloc(variable, newCapacity);
                    if(newVariable == nullptr)
                    {
                        // free(variable);
                        variable = malloc(newCapacity);
                        if(variable == nullptr) throw(ENOMEM);
                    }
                    else
                    {
                        variable = newVariable;
                    }
                }
                else
                {
                    // free(variable);
                    variable = malloc(newCapacity);
                    if(variable == nullptr) throw(ENOMEM);
                }
                capacity = newCapacity;
            }
        public:

            const size_t& size = capacity;
            
            untypizedtrivial()
            {
                capacity = 0;
                variable = nullptr;
                resize(0);
            }

            template<class type>
            untypizedtrivial(const type& value)
            {
                if(!std::is_trivial<type>().value) throw(EINVAL);
                resize(sizeof(value));
                *((type*)variable) = (type&)value;
            }

            untypizedtrivial(const untypizedtrivial& value)
            {
                resize(value.size);
                if(value.size != 0) *((char*)variable) = (char&)value;
            }

            untypizedtrivial(untypizedtrivial&& value)
            {
                variable = value.variable;
                capacity = value.capacity;
                value.variable = nullptr;
                value.capacity = 0;
            }

            untypizedtrivial& operator=(const untypizedtrivial& value)
            {
                if(&value == this) return *this;
                resize(value.size);
                if(value.size != 0) *((char*)variable) = (char&)value;
                return *this;
            }

            untypizedtrivial(const untypizedtrivial&& value)
            {
                resize(value.size);
                if(value.size != 0) *((char*)variable) = (char&)value;
            }

            untypizedtrivial& operator=(const untypizedtrivial&& value)
            {
                if(&value == this) return *this;
                resize(value.size);
                if(value.size != 0) *((char*)variable) = (char&)value;
                return *this;
            }

            template<class type>
            untypizedtrivial& operator=(const type& value)
            {
                if(!std::is_trivial<type>().value) throw(EINVAL);
                resize(sizeof(type));
                *((type*)variable) = value;
                return *this;
            }

            template<class type>
            operator type&() const
            {
                if(!std::is_trivial<type>().value or (sizeof(type) != capacity)) throw(EINVAL);
                return *((type*)variable);
            }

            // template<class type>
            // operator type()
            // {
            //     return *((type*)variable);
            // }

            ~untypizedtrivial()
            {
                if(variable != nullptr) free(variable);
            }
    };

    /**
     * @brief Due to the compatibility with POD. (any POD type is also trivial type)
     * 
     */
    typedef untypizedtrivial untypizedpod;
};
