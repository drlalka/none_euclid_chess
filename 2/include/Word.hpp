#pragma once
#include <cstddef>
#include <compare>
#include <string>
#include <ostream>  
#include <istream>
/**
 * @class Word
 * @brief Class for working with words (character strings without whitespace)
 *
 * Automatically manages memory allocation and deallocation.
 * Uses shared delimiter table with WordArray for consistent parsing.
 */
class Word
{
private:
    char *array = nullptr;
    std::size_t size = 0;

    static bool s_delimiterTable[256];
    static bool s_tableInitialized;

    static void initDefaultDelimiters();
    static bool isDelimiter(char c);

public:
    /**
     * @brief Sets delimiters for all Word objects
     * @param delimiters String with delimiter characters
     * @throws std::invalid_argument if delimiters == nullptr
     */
    static void setDelimiters(const char *delimiters);

    /**
     * @brief Default constructor - creates empty word
     * @post array == nullptr, size == 0
     * @throws None
     */
    Word();

    /**
     * @brief Destructor - frees allocated memory
     * @throws None
     */
    ~Word();

    /**
     * @brief Constructor from C-style string (with number of the word in a str)
     * @param str Pointer to null-terminated string
     * @param pos number of word in a str, 0 by default. if pos > real return epty word
     * @post Extracts pos'th word (ignoring leading delimiters)
     * @throws None
     * @warning dont forget about another constructor Word(std::size_t len, const char *arr)
     */
    explicit Word(const char *str, std::size_t pos = 0);

    /**
     * @brief Constructor from std::string (with number of the word in a str)
     * @param str string
     * @param pos number of word in a str, 0 by default. if pos > real return epty word
     * @post Extracts pos'th word (ignoring leading delimiters)
     * @throws None
     */
    Word(const std::string str, std::size_t pos = 0);

    /**
     * @brief Copy constructor
     * @param other Word object to copy from
     * @throws std::bad_alloc if memory allocation fails
     */
    Word(const Word &other);

    /**
     * @brief Constructor from character array and length
     * @param arr Pointer to character array
     * @param len Number of characters to copy
     * @throws std::invalid_argument if arr is nullptr
     * @warning dont forget about another constructor Word(const char *str, std::size_t pos = 0)
     */
    Word(std::size_t len, const char *arr);

    /**
     * @brief Constructor that allocates uninitialized word
     * @param len Number of characters to allocate
     * @throws std::bad_alloc if memory allocation fails
     */
    Word(std::size_t len);

    /**
     * @brief Move constructor
     * @param other Word to move from
     * @throws None
     */
    Word(Word &&other) noexcept;

    /**
     * @brief Move assignment operator
     * @param other Word to move from
     * @return Reference to this word
     * @throws None
     */
    Word &operator=(Word &&other) noexcept;

    /**
     * @brief Three-way comparison operator
     * @param other Word to compare with
     * @return std::strong_ordering
     * @throws None
     */
    std::strong_ordering operator<=>(const Word &other) const;

    /**
     * @brief Output operator
     * @param os Output stream
     * @param word Word to output
     * @return Reference to output stream
     * @throws None
     */
    friend std::ostream &operator<<(std::ostream &os, const Word &word);

    /**
     * @brief Input operator
     * @param is Input stream
     * @param word Word to read into
     * @return Reference to input stream
     * @throws None
     */
    friend std::istream& operator>>(std::istream& is, Word& word);

    /**
     * @brief Equality comparison operator
     * @param other Word to compare with
     * @return true if words are equal
     * @throws None
     */
    bool operator==(const Word &other) const;

    /**
     * @brief Copy assignment operator
     * @param other Word to assign from
     * @return Reference to this word
     * @throws std::bad_alloc if memory allocation fails
     */
    Word &operator=(const Word &other);

    /**
     * @brief Converts all characters to lowercase
     * @return Reference to this word
     * @throws None
     */
    Word &toLowerCase();

    /**
     * @brief Sets word content from C-style string
     * @param str Null-terminated string to copy from
     * @throws None
     */
    void setStr(const char *str);

    /**
     * @brief Checks if word starts with specified prefix
     * @param prefix Null-terminated string prefix to check
     * @return true if word starts with prefix
     * @throws None
     */
    bool startsWith(const char *prefix) const;

    /**
     * @brief Checks if word starts with another word as prefix
     * @param word Word to check as prefix
     * @return true if word starts with given word
     * @throws None
     */
    bool startsWith(const Word &word) const;

    /**
     * @brief Gets the length of the word
     * @return Number of characters in the word
     * @throws None
     */
    std::size_t getSize() const;

    /**
     * @brief Gets word content as C-style string
     * @return Newly allocated null-terminated string
     * @throws std::bad_alloc if memory allocation fails
     */
    char *getCStr() const;

    /**
     * @brief Iterator to beginning of word
     * @return pointer to first character
     * @throws None
     */
    const char *begin() const;
    char *begin();

    /**
     * @brief Iterator to end of word
     * @return pointer to position after last character
     * @throws None
     */
    const char *end() const;
    char *end();
};