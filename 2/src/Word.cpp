#include <cstring>
#include <string>
#include <algorithm>
#include <cctype>
#include <iostream>
#include "Word.hpp"

bool Word::s_delimiterTable[256] = {};
bool Word::s_tableInitialized = false;

void Word::initDefaultDelimiters()
{
    setDelimiters(" \t\n");
}

bool Word::isDelimiter(char c)
{
    if (!s_tableInitialized)
    {
        initDefaultDelimiters();
    }
    return s_delimiterTable[static_cast<unsigned char>(c)];
}

void Word::setDelimiters(const char *delimiters)
{
    std::fill_n(s_delimiterTable, 256, false);
    for (const char *c = delimiters; *c != '\0'; ++c)
    {
        s_delimiterTable[static_cast<unsigned char>(*c)] = true;
    }
    s_tableInitialized = true;
}

Word::Word() = default;

Word::~Word()
{
    delete[] array;
    array = nullptr;
    size = 0;
}
Word::Word(const char *str, std::size_t pos)
{

    if (!s_tableInitialized)
    {
        initDefaultDelimiters();
    }

    if (str == nullptr || pos < 0)
    {
        return;
    }

    const char *current = str;
    int wordCount = 0;

    while (*current && wordCount <= pos)
    {
        while (*current && isDelimiter(*current))
        {
            current++;
        }

        if (!*current)
            break;

        if (wordCount == pos)
        {
            const char *wordStart = current;
            while (*current && !isDelimiter(*current))
            {
                current++;
            }

            size_t wordLength = current - wordStart;
            if (wordLength > 0)
            {
                array = new char[wordLength];
                size = wordLength;
                std::copy(wordStart, wordStart + wordLength, array);
            }
            break;
        }

        while (*current && !isDelimiter(*current))
        {
            current++;
        }
        wordCount++;
    }
}

Word::Word(std::string str, std::size_t pos) : Word(str.c_str(), pos) {};

Word::Word(const Word &other)
{
    if (other.array != nullptr && other.size > 0)
    {
        size = other.size;
        array = new char[size];
        std::copy(other.array, other.array + size, array);
    }
}

Word::Word(std::size_t len, const char *arr)
{
    if (arr == nullptr || len == 0)
    {
        return;
    }
    size = len;
    array = new char[len];
    std::copy(arr, arr + len, array);
}

Word::Word(std::size_t len)
{
    if (len > 0)
    {
        array = new char[len];
        size = len;
    }
}

Word::Word(Word &&other) noexcept
    : array(other.array), size(other.size)
{
    other.array = nullptr;
    other.size = 0;
}

Word &Word::operator=(Word &&other) noexcept
{
    if (this != &other)
    {
        delete[] array;
        array = other.array;
        size = other.size;
        other.array = nullptr;
        other.size = 0;
    }
    return *this;
}

std::strong_ordering Word::operator<=>(const Word &other) const
{
    return std::lexicographical_compare_three_way(begin(), end(), other.begin(), other.end());
}

bool Word::operator==(const Word &other) const
{
    if (size != other.size)
        return false;
    return std::equal(array, array + size, other.array);
}

Word &Word::operator=(const Word &other)
{
    if (this == &other)
        return *this;
    if (array != nullptr)
        delete[] array;
    array = nullptr;
    size = 0;
    if (other.array != nullptr && other.size > 0)
    {
        size = other.size;
        array = new char[size];
        std::copy(other.begin(), other.end(), array);
    }
    return *this;
}

Word &Word::toLowerCase()
{
    std::for_each(begin(), end(), [](char &c)
                  { c = std::tolower(static_cast<unsigned char>(c)); });
    return *this;
}

bool Word::startsWith(const char *prefix) const
{
    if (!prefix)
        return true;
    if (!array)
        return false;
    size_t prefix_len = std::strlen(prefix);
    if (prefix_len > size)
        return false;
    return std::equal(prefix, prefix + prefix_len, array);
}

bool Word::startsWith(const Word &word) const
{
    return std::equal(word.begin(), word.end(), array);
}

std::size_t Word::getSize() const
{
    return size;
}

const char *Word::begin() const
{
    return array;
}

char *Word::begin()
{
    return array;
}

const char *Word::end() const
{
    return array + size;
}

char *Word::end()
{
    return array + size;
}

char *Word::getCStr() const
{
    char *rezult = new char[size + 1];
    std::copy(begin(), end(), rezult);
    rezult[size] = '\0';
    return rezult;
}

void Word::setStr(const char *str)
{
    delete[] array;
    size = 0;
    if (!str)
    {
        array = nullptr;
        return;
    }
    std::size_t len = std::strlen(str);
    if (len == 0)
        array = nullptr;
    array = new char[std::strlen(str)];
    size = len;
    std::copy(str, str + len * sizeof(char), array);
}

std::ostream &operator<<(std::ostream &os, const Word &word)
{
    char *str = word.getCStr();
    os << str;
    delete[] str;
    return os;
}

std::istream &operator>>(std::istream &is, Word &word)
{
    std::istreambuf_iterator<char> begin(is), end;
    auto start = std::find_if_not(begin, end, Word::isDelimiter);

    if (start == end)
    {
        is.setstate(std::ios::failbit);
        return is;
    }

    auto finish = std::find_if(start, end, Word::isDelimiter);

    std::string temp(start, finish);
    word = Word(temp.c_str());

    return is;
}