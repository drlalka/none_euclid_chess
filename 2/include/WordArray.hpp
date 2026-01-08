#pragma once
#include "Word.hpp"
#include <iostream>
#include <algorithm>

/**
 * @class WordArray
 * @brief Array of unique words with dynamic memory management
 *
 * Maintains a collection of Word objects without duplicates.
 * Uses shared delimiter table with Word class for consistent parsing.
 */
class WordArray
{
private:
    Word *data = nullptr;
    size_t size = 0;
    size_t capacity = 0;

    void resize(size_t new_capacity);

public:
    /**
     * @brief Default constructor
     * @post Creates empty WordArray
     * @throws None
     */
    WordArray();

    /**
     * @brief Constructor with multiple words
     * @param count Number of words
     * @param words Array of C-style strings
     * @throws None
     */
    explicit WordArray(const Word &word);

    /**
     * @brief Constructor with multiple words
     * @param count Number of words
     * @param words Array of C-style strings
     * @throws std::invalid_argument if word[] empty, count>0
     */
    WordArray(const size_t count, const Word words[]);

    /**
     * @brief Destructor
     * @throws None
     */
    ~WordArray();

    /**
     * @brief Copy constructor
     * @param other WordArray to copy from
     * @throws std::bad_alloc if memory allocation fails
     */
    WordArray(const WordArray &other);

    /**
     * @brief Copy assignment operator
     * @param other WordArray to assign from
     * @return Reference to this WordArray
     * @throws std::bad_alloc if memory allocation fails
     */
    WordArray &operator=(const WordArray &other);

    /**
     * @brief Move constructor
     * @param other WordArray to move from
     * @throws None
     */
    WordArray(WordArray &&other);

    /**
     * @brief Move assignment operator
     * @param other WordArray to move from
     * @return Reference to this WordArray
     * @throws None
     */
    WordArray &operator=(WordArray &&other);

    /**
     * @brief Adds word to array
     * @param word Word to add
     * @return Reference to this WordArray
     * @throws std::invalid_argument if word already exists
     */
    WordArray &operator+=(const Word &word);

    /**
     * @brief Merges another WordArray into this one
     * @param other WordArray to merge
     * @return Reference to this WordArray
     * @throws None
     */
    WordArray &operator+=(const WordArray &other);
    /**
     * @brief Finds word in array
     * @param word Word to find
     * @return Index of word or -1 if not found
     * @throws None
     */
    size_t find(const Word &word) const;

    /**
     * @brief Access word by index
     * @param index Zero-based index
     * @return Reference to word at index
     * @throws std::out_of_range if index >= size
     */
    Word &operator[](size_t index);

    /**
     * @brief Access word by index (const version)
     * @param index Zero-based index
     * @return Const reference to word at index
     * @throws std::out_of_range if index >= size
     */
    const Word &operator[](size_t index) const;

    /**
     * @brief Gets words starting with specified word
     * @param prefix Character to match (case-insensitive)
     * @return New WordArray with matching words
     * @throws None
     */
    WordArray getWordsStartingWith(Word prefix) const;

    /**
     * @brief Sorts words alphabetically
     * @throws None
     */
    void sort();

    /**
     * @brief Parses string and adds all words to array
     * @param str String to parse
     * @return count of parsed words
     * @throws None
     */
    size_t parseString(const char *str);

    /**
     * @brief Output operator
     * @param os Output stream
     * @param arr WordArray to output
     * @return Reference to output stream
     * @throws None
     */
    friend std::ostream &operator<<(std::ostream &os, const WordArray &arr);

    /**
     * @brief Input operator
     * @param is Input stream
     * @param arr WordArray to read into
     * @return Reference to input stream
     * @throws None
     */
    friend std::istream &operator>>(std::istream &is, WordArray &arr);

    /**
     * @brief Gets number of words in array
     * @return Current size
     * @throws None
     */
    size_t getSize() const;

    /**
     * @brief Gets current capacity of array
     * @return Current capacity
     * @throws None
     */
    size_t getCapacity() const;

    /**
    * @brief Gets static const of not found
    * @return value of not found
    */
    static size_t npos() { return static_cast<size_t>(-1);}

    /**
     * @brief Provides access to internal array via iterators
     */
    const Word *begin() const { return data; }
    const Word *end() const { return data + size; }
    Word *begin() { return data; }
    Word *end() { return data + size; }
};