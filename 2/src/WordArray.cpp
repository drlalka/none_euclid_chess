#include "WordArray.hpp"
#include "Word.hpp"
#include <cstring>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <ranges>
#include <algorithm>

WordArray::WordArray() : data(nullptr), size(0), capacity(0) {}

WordArray::~WordArray()
{
    delete[] data;
    capacity = 0;
    size = 0;
}

WordArray::WordArray(const Word &word)
{
    data = new Word[4];
    data[0] = word;
    capacity = 4;
    size = 1;
}

WordArray::WordArray(const size_t count, const Word *words)
{
    if (count > 0 && words == nullptr)
    {
        throw std::invalid_argument("Words array cannot be null");
    }

    Word *temp = new Word[count];
    for (size_t i = 0; i < count; ++i)
    {
        temp[i] = words[i];
    }

    std::sort(temp, temp + count);

    Word *unique_end = std::unique(temp, temp + count, [](const Word &a, const Word &b)
                                   { return (a <=> b) == std::strong_ordering::equal; });
    size = unique_end - temp;
    capacity = 1;
    while (capacity < size)
        capacity *= 2;
    data = new Word[capacity];
    std::copy(temp, unique_end, data);
    delete[] temp;
}

WordArray::WordArray(const WordArray &other)
    : size(other.size), capacity(other.size > 0 ? other.size : 1), data(nullptr)
{
    if (size > 0) {
        data = new Word[capacity];
        std::copy(other.begin(), other.end(), data);
    } else {
        data = new Word[1];
        capacity = 1;
    }
}

WordArray::WordArray(WordArray &&other)
{
    data = other.data;
    size = other.size;
    capacity = other.capacity;

    other.data = nullptr;
    other.size = 0;
    other.capacity = 0;
}

void WordArray::resize(size_t new_capacity)
{
    if (new_capacity <= capacity)
        return;

    Word *new_data = new Word[new_capacity];
    size_t moved_count = 0;
    try
    {
        for (; moved_count < size; ++moved_count)
        {
            new_data[moved_count] = std::move(data[moved_count]);
        }
    }
    catch (...)
    {
        for (size_t i = 0; i < moved_count; ++i)
        {
            data[i] = std::move(new_data[i]);
        }
        delete[] new_data;
        throw;
    }
    delete[] data;
    data = new_data;
    capacity = new_capacity;
}

WordArray &WordArray::operator+=(const Word &other)
{
    for (size_t i = 0; i < size; ++i)
    {
        if (data[i] == other)
        {
            return *this;
        }
    }

    if (capacity <= size)
    {
        resize((capacity + 1) * 2);
    }
    data[size] = other;
    size++;
    return *this;
}

WordArray &WordArray::operator+=(const WordArray &other)
{
    std::for_each(other.begin(), other.end(), [this](const Word &it)
                  { *this += it; });
    return *this;
}

size_t WordArray::parseString(const char *str)
{
    if (str == nullptr)
        return 0;

    size_t words_added = 0;

    while (true)
    {
        Word word(str, words_added);
        if (word.getSize() == 0)
            break;

        *this += word;
        words_added++;
    }

    return words_added;
}

WordArray &WordArray::operator=(const WordArray &other)
{
    if (this == &other)
        return *this;
    
    Word* new_data = new Word[other.capacity];
    for (size_t i = 0; i < other.size; ++i) {
        new_data[i] = other.data[i];
    }
    
    delete[] data;
    data = new_data;
    size = other.size;
    capacity = other.capacity;
    
    return *this;
}
WordArray &WordArray::operator=(WordArray &&other)
{
    if (this != &other)
    {
        delete[] data;

        data = other.data;
        size = other.size;
        capacity = other.capacity;

        other.data = nullptr;
        other.size = other.capacity = 0;
    }
    return *this;
}

size_t WordArray::find(const Word &word) const
{
    auto it = std::find(begin(), end(), word);
    return (it != end()) ? static_cast<size_t>(it - begin()) : npos();
}

Word &WordArray::operator[](size_t index)
{
    if (index >= size)
        throw std::out_of_range("Index out of range, kys");
    return data[index];
}

const Word &WordArray::operator[](size_t index) const
{
    if (index >= size)
        throw std::out_of_range("Index out of range, kys");
    return data[index];
}

WordArray WordArray::getWordsStartingWith(Word prefix) const
{
    WordArray res;
    prefix.toLowerCase();
    std::ranges::for_each(
        *this | std::views::filter([&prefix](const Word &w)
                                   { return Word(w).toLowerCase().startsWith(prefix); }),
        [&res](const Word &w)
        {
            res += w;
        });
    return res;
}

void WordArray::sort()
{
    std::sort(begin(), end());
}

std::ostream &operator<<(std::ostream &os, const WordArray &arr)
{
    os << "[";
    bool first = true;
    for (const Word &word : arr)
    {
        if (!first)
        {
            os << ", ";
        }
        os << word;
        first = false;
    }
    os << "]";
    return os;
}

std::istream &operator>>(std::istream &is, WordArray &arr)
{
    arr = WordArray();
    std::string line;
    if (std::getline(is, line))
    {
        size_t count = arr.parseString(line.c_str());
        if (count == 0)
        {
            is.setstate(std::ios::failbit);
        }
    }
    return is;
}

size_t WordArray::getSize() const
{
    return size;
}

size_t WordArray::getCapacity() const
{
    return capacity;
}