#include <gtest/gtest.h>
#include "WordArray.hpp"
#include <sstream>
#include <stdexcept>

class WordArrayTest : public ::testing::Test {
protected:
    void SetUp() override {
        Word::setDelimiters(" \t\n\r");
    }

    WordArray emptyArray;
    WordArray singleWordArray{Word("hello")};
    WordArray multiWordArray;
    
    void initMultiWordArray() {
        multiWordArray += Word("apple");
        multiWordArray += Word("banana");
        multiWordArray += Word("cherry");
    }
};

TEST_F(WordArrayTest, DefaultConstructor) {
    EXPECT_EQ(emptyArray.getSize(), 0);
    EXPECT_EQ(emptyArray.getCapacity(), 0);
}

TEST_F(WordArrayTest, SingleWordConstructor) {
    EXPECT_EQ(singleWordArray.getSize(), 1);
    EXPECT_EQ(singleWordArray[0], Word("hello"));
}

TEST_F(WordArrayTest, MultiWordConstructor) {
    Word words[] = {Word("ane"), Word("bwt"), Word("chr")};
    WordArray array(3, words);
    
    EXPECT_EQ(array.getSize(), 3);
    EXPECT_EQ(array[0], Word("ane"));
    EXPECT_EQ(array[1], Word("bwt"));
    EXPECT_EQ(array[2], Word("chr"));
}

// Тесты добавления слов
TEST_F(WordArrayTest, AddSingleWord) {
    emptyArray += Word("test");
    EXPECT_EQ(emptyArray.getSize(), 1);
    EXPECT_EQ(emptyArray[0], Word("test"));
}

TEST_F(WordArrayTest, AddMultipleWords) {
    initMultiWordArray();
    EXPECT_EQ(multiWordArray.getSize(), 3);
    EXPECT_EQ(multiWordArray[0], Word("apple"));
    EXPECT_EQ(multiWordArray[1], Word("banana"));
    EXPECT_EQ(multiWordArray[2], Word("cherry"));
}

TEST_F(WordArrayTest, CopyConstructor) {
    initMultiWordArray();
    WordArray copy = multiWordArray;
    EXPECT_EQ(copy.getSize(), 3);
    EXPECT_EQ(copy[0], Word("apple"));
    EXPECT_EQ(copy[1], Word("banana"));
    EXPECT_EQ(copy[2], Word("cherry"));
    copy += Word("date");
    EXPECT_EQ(copy.getSize(), 4);
    EXPECT_EQ(multiWordArray.getSize(), 3);
}

TEST_F(WordArrayTest, CopyAssignment) {
    initMultiWordArray();
    WordArray copy;
    copy = multiWordArray;
    
    EXPECT_EQ(copy.getSize(), 3);
    EXPECT_EQ(copy[0], Word("apple"));
}

TEST_F(WordArrayTest, MoveConstructor) {
    initMultiWordArray();
    WordArray moved = std::move(multiWordArray);
    
    EXPECT_EQ(moved.getSize(), 3);
    EXPECT_EQ(moved[0], Word("apple"));
    EXPECT_EQ(multiWordArray.getSize(), 0);  // оригинал перемещен
}

TEST_F(WordArrayTest, MoveAssignment) {
    initMultiWordArray();
    WordArray moved;
    moved = std::move(multiWordArray);
    
    EXPECT_EQ(moved.getSize(), 3);
    EXPECT_EQ(multiWordArray.getSize(), 0);
}

// Тесты поиска
TEST_F(WordArrayTest, FindExistingWord) {
    initMultiWordArray();
    EXPECT_EQ(multiWordArray.find(Word("banana")), 1);
    EXPECT_EQ(multiWordArray.find(Word("apple")), 0);
}

TEST_F(WordArrayTest, FindNonExistingWord) {
    initMultiWordArray();
    EXPECT_EQ(multiWordArray.find(Word("zucchini")), WordArray::npos());
}

TEST_F(WordArrayTest, CaseSensitiveFind) {
    initMultiWordArray();
    multiWordArray += Word("Apple");
    EXPECT_EQ(multiWordArray.find(Word("apple")), 0);
    EXPECT_EQ(multiWordArray.find(Word("Apple")), 3);
}

// Тесты оператора []
TEST_F(WordArrayTest, AccessByIndex) {
    initMultiWordArray();
    EXPECT_EQ(multiWordArray[0], Word("apple"));
    EXPECT_EQ(multiWordArray[1], Word("banana"));
    EXPECT_EQ(multiWordArray[2], Word("cherry"));
}

TEST_F(WordArrayTest, AccessByIndexOutOfRange) {
    EXPECT_THROW(emptyArray[0], std::out_of_range);
    initMultiWordArray();
    EXPECT_THROW(multiWordArray[5], std::out_of_range);
}

TEST_F(WordArrayTest, ModifyByIndex) {
    initMultiWordArray();
    multiWordArray[1] = Word("blueberry");
    EXPECT_EQ(multiWordArray[1], Word("blueberry"));
}

TEST_F(WordArrayTest, GetWordsStartingWith) {
    initMultiWordArray();
    multiWordArray += Word("apricot");
    multiWordArray += Word("avocado");
    multiWordArray += Word("berry");
    WordArray result = multiWordArray.getWordsStartingWith(Word("ap"));
    EXPECT_EQ(result.getSize(), 2);
    EXPECT_EQ(result.find(Word("apple")), 0);
    EXPECT_EQ(result.find(Word("apricot")), 1);
    EXPECT_EQ(result.find(Word("avocado")), WordArray::npos());
}

TEST_F(WordArrayTest, GetWordsStartingWithCaseInsensitive) {
    multiWordArray += Word("Apple");
    multiWordArray += Word("APRICOT");
    
    WordArray result = multiWordArray.getWordsStartingWith(Word("a"));
    EXPECT_GE(result.getSize(), 2);  // как минимум apple и Apple
}

TEST_F(WordArrayTest, GetWordsStartingWithNoMatches) {
    initMultiWordArray();
    WordArray result = multiWordArray.getWordsStartingWith(Word("xyz"));
    EXPECT_EQ(result.getSize(), 0);
}

// Тесты сортировки
TEST_F(WordArrayTest, SortEmptyArray) {
    emptyArray.sort();
    EXPECT_EQ(emptyArray.getSize(), 0);
}

TEST_F(WordArrayTest, SortSingleElement) {
    singleWordArray.sort();
    EXPECT_EQ(singleWordArray.getSize(), 1);
    EXPECT_EQ(singleWordArray[0], Word("hello"));
}

TEST_F(WordArrayTest, SortMultipleElements) {
    WordArray array;
    array += Word("banana");
    array += Word("apple");
    array += Word("cherry");
    
    array.sort();
    
    EXPECT_EQ(array[0], Word("apple"));
    EXPECT_EQ(array[1], Word("banana"));
    EXPECT_EQ(array[2], Word("cherry"));
}

// Тесты парсинга строк
TEST_F(WordArrayTest, ParseString) {
    WordArray array;
    size_t count = array.parseString("hello world test");
    
    EXPECT_EQ(count, 3);
    EXPECT_EQ(array.getSize(), 3);
    EXPECT_EQ(array[0], Word("hello"));
    EXPECT_EQ(array[1], Word("world"));
    EXPECT_EQ(array[2], Word("test"));
}

TEST_F(WordArrayTest, ParseStringWithMultipleDelimiters) {
    WordArray array;
    size_t count = array.parseString("hello\tworld\n test  again");
    
    EXPECT_EQ(count, 4);
    EXPECT_EQ(array.getSize(), 4);
}

TEST_F(WordArrayTest, ParseStringWithDuplicates) {
    WordArray array;
    size_t count = array.parseString("hello hello world");
    
    EXPECT_EQ(count, 3);
    EXPECT_EQ(array.getSize(), 2);
}

TEST_F(WordArrayTest, ParseEmptyString) {
    WordArray array;
    size_t count = array.parseString("");
    
    EXPECT_EQ(count, 0);
    EXPECT_EQ(array.getSize(), 0);
}

// Тесты ввода/вывода
TEST_F(WordArrayTest, OutputOperator) {
    initMultiWordArray();
    std::ostringstream oss;
    oss << multiWordArray;
    
    std::string output = oss.str();
    EXPECT_FALSE(output.empty());
    // Проверяем что вывод содержит наши слова
    EXPECT_NE(output.find("apple"), std::string::npos);
    EXPECT_NE(output.find("banana"), std::string::npos);
}

TEST_F(WordArrayTest, InputOperator) {
    WordArray array;
    std::istringstream iss("hello world test");
    iss >> array;
    
    EXPECT_EQ(array.getSize(), 3);
    EXPECT_EQ(array[0], Word("hello"));
    EXPECT_EQ(array[1], Word("world"));
    EXPECT_EQ(array[2], Word("test"));
}

TEST_F(WordArrayTest, InputOperatorWithEmptyLine) {
    WordArray array;
    std::istringstream iss("");
    iss >> array;
    
    EXPECT_TRUE(iss.fail() || array.getSize() == 0);
}

// Тесты итераторов
TEST_F(WordArrayTest, Iterators) {
    initMultiWordArray();
    
    size_t count = 0;
    for (const Word& word : multiWordArray) {
        EXPECT_FALSE(word.getSize() == 0);
        count++;
    }
    EXPECT_EQ(count, 3);
}

TEST_F(WordArrayTest, EmptyIterators) {
    EXPECT_EQ(emptyArray.begin(), emptyArray.end());
}

// Тесты объединения массивов
TEST_F(WordArrayTest, MergeArrays) {
    WordArray array1;
    array1 += Word("one");
    array1 += Word("two");
    
    WordArray array2;
    array2 += Word("three");
    array2 += Word("four");
    
    array1 += array2;
    
    EXPECT_EQ(array1.getSize(), 4);
    EXPECT_EQ(array1.find(Word("one")), 0);
    EXPECT_EQ(array1.find(Word("two")), 1);
    EXPECT_EQ(array1.find(Word("three")), 2);
    EXPECT_EQ(array1.find(Word("four")), 3);
}

TEST_F(WordArrayTest, MergeWithDuplicates) {
    WordArray array1;
    array1 += Word("one");
    array1 += Word("two");
    
    WordArray array2;
    array2 += Word("two");
    array2 += Word("three");
    
    array1 += array2;
    
    EXPECT_EQ(array1.getSize(), 3);
}
TEST_F(WordArrayTest, SelfAssignment) {
    initMultiWordArray();
    multiWordArray = multiWordArray;
    EXPECT_EQ(multiWordArray.getSize(), 3);
}

TEST_F(WordArrayTest, LargeNumberOfWords) {
    WordArray array;
    for (int i = 0; i < 1000; ++i) {
        array += Word("word_" + std::to_string(i));
    }
    EXPECT_EQ(array.getSize(), 1000);
    EXPECT_GE(array.getCapacity(), 1000);
}

TEST_F(WordArrayTest, NposValue) {
    EXPECT_EQ(WordArray::npos(), static_cast<size_t>(-1));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}