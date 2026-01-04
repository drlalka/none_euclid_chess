#include <gtest/gtest.h>
#include <cstring>
#include "Word.hpp"

class WordTest : public ::testing::Test {
protected:
    void SetUp() override {
        Word::setDelimiters(" \t\n");
    }

    bool compareWordToString(const Word& word, const char* expected) {
        if (word.getSize() != std::strlen(expected)) return false;
        return std::equal(word.begin(), word.end(), expected);
    }
};

TEST_F(WordTest, SetDelimiters) {
    Word::setDelimiters(",;");
    Word word("hello,world;test", 0);
    EXPECT_TRUE(compareWordToString(word, "hello"));
    
    Word word2("hello,world;test", 1);
    EXPECT_TRUE(compareWordToString(word2, "world"));
}

TEST_F(WordTest, DefaultConstructor) {
    Word word;
    EXPECT_EQ(word.getSize(), 0);
    EXPECT_EQ(word.begin(), nullptr);
    EXPECT_EQ(word.end(), nullptr);
}

TEST_F(WordTest, ConstructorFromCString_FirstWord) {
    Word word1("hello");
    EXPECT_EQ(word1.getSize(), 5);
    EXPECT_TRUE(compareWordToString(word1, "hello"));
}

TEST_F(WordTest, ConstructorFromCString_WithLeadingSpaces) {
    Word word2("   world");
    EXPECT_TRUE(compareWordToString(word2, "world"));
}

TEST_F(WordTest, ConstructorFromCString_OnlySpaces) {
    Word word3("   ");
    EXPECT_EQ(word3.getSize(), 0);
}

TEST_F(WordTest, ConstructorFromCString_SecondWord) {
    Word word("hello world", 1);
    EXPECT_TRUE(compareWordToString(word, "world"));
}

TEST_F(WordTest, ConstructorFromCString_OutOfBounds) {
    Word word("hello world", 5);
    EXPECT_EQ(word.getSize(), 0);
}

TEST_F(WordTest, ConstructorFromCString_CustomDelimiters) {
    Word::setDelimiters(",");
    Word word("hello,world,test", 1);
    EXPECT_TRUE(compareWordToString(word, "world"));
}

TEST_F(WordTest, ConstructorFromCharArray) {
    const char arr[] = {'t', 'e', 's', 't'};
    Word word(4, arr);
    EXPECT_EQ(word.getSize(), 4);
    EXPECT_TRUE(compareWordToString(word, "test"));
}

TEST_F(WordTest, ConstructorFromCharArray_Empty) {
    const char arr[] = {'t', 'e', 's', 't'};
    Word word(0, arr);
    EXPECT_EQ(word.getSize(), 0);
}

TEST_F(WordTest, ConstructorFromCharArray_Nullptr) {
    Word word(5, nullptr);
    EXPECT_EQ(word.getSize(), 0);
}

TEST_F(WordTest, ConstructorWithLength) {
    Word word(5);
    EXPECT_EQ(word.getSize(), 5);
    EXPECT_NE(word.begin(), nullptr);
}

TEST_F(WordTest, CopyConstructor) {
    Word original("original");
    Word copy(original);
    
    EXPECT_EQ(original.getSize(), copy.getSize());
    EXPECT_NE(original.begin(), copy.begin());
    EXPECT_TRUE(compareWordToString(copy, "original"));
}

TEST_F(WordTest, CopyConstructor_Empty) {
    Word original;
    Word copy(original);
    EXPECT_EQ(copy.getSize(), 0);
}

// Move конструктор
TEST_F(WordTest, MoveConstructor) {
    Word original("moveme");
    Word moved(std::move(original));
    
    EXPECT_TRUE(compareWordToString(moved, "moveme"));
    EXPECT_EQ(original.getSize(), 0);
    EXPECT_EQ(original.begin(), nullptr);
}

// Операторы присваивания
TEST_F(WordTest, AssignmentOperator) {
    Word word1("source");
    Word word2;
    
    word2 = word1;
    
    EXPECT_EQ(word1.getSize(), word2.getSize());
    EXPECT_NE(word1.begin(), word2.begin());
    EXPECT_TRUE(compareWordToString(word2, "source"));
}

TEST_F(WordTest, SelfAssignment) {
    Word word1("source");
    word1 = word1;
    EXPECT_TRUE(compareWordToString(word1, "source"));
}

TEST_F(WordTest, MoveAssignment) {
    Word word1("moveme");
    Word word2;
    
    word2 = std::move(word1);
    
    EXPECT_TRUE(compareWordToString(word2, "moveme"));
    EXPECT_EQ(word1.getSize(), 0);
    EXPECT_EQ(word1.begin(), nullptr);
}

TEST_F(WordTest, ThreeWayComparison) {
    Word word1("apple");
    Word word2("banana");
    Word word3("apple");
    
    EXPECT_EQ((word1 <=> word2), std::strong_ordering::less);
    EXPECT_EQ((word2 <=> word1), std::strong_ordering::greater);
    EXPECT_EQ((word1 <=> word3), std::strong_ordering::equal);
}

TEST_F(WordTest, EqualityOperator) {
    Word word1("apple");
    Word word2("banana"); 
    Word word3("apple");
    
    EXPECT_TRUE(word1 == word3);
    EXPECT_FALSE(word1 == word2);
}

TEST_F(WordTest, ComparisonOperators) {
    Word word1("apple");
    Word word2("banana"); 
    Word word3("apple");
    
    EXPECT_TRUE(word1 < word2);
    EXPECT_TRUE(word2 > word1);
    EXPECT_TRUE(word1 <= word3);
    EXPECT_TRUE(word2 >= word1);
}

// Методы
TEST_F(WordTest, ToLowerCase) {
    Word word("HeLLo");
    word.toLowerCase();
    EXPECT_TRUE(compareWordToString(word, "hello"));
}

TEST_F(WordTest, ToLowerCase_Empty) {
    Word word;
    word.toLowerCase();
    EXPECT_EQ(word.getSize(), 0);
}

TEST_F(WordTest, StartsWith_CharPtr) {
    Word word("hello world");
    
    EXPECT_TRUE(word.startsWith("hello"));
    EXPECT_TRUE(word.startsWith("he"));
    EXPECT_FALSE(word.startsWith("world"));
    EXPECT_FALSE(word.startsWith("hello world!"));
}

TEST_F(WordTest, StartsWith_Word) {
    Word word("hello world");
    Word prefix1("hell");
    Word prefix2("world");
    
    EXPECT_TRUE(word.startsWith(prefix1));
    EXPECT_FALSE(word.startsWith(prefix2));
}

TEST_F(WordTest, StartsWith_Empty) {
    Word word;
    EXPECT_FALSE(word.startsWith("test"));
    
    Word empty;
    EXPECT_TRUE(word.startsWith(empty));
}

TEST_F(WordTest, GetCStr) {
    Word word("test");
    char* cstr = word.getCStr();
    
    EXPECT_NE(cstr, nullptr);
    EXPECT_STREQ(cstr, "test");
    
    delete[] cstr;
}

TEST_F(WordTest, GetCStr_Empty) {
    Word word;
    char* cstr = word.getCStr();
    
    EXPECT_STREQ(cstr, "");
    delete[] cstr;
}

TEST_F(WordTest, SetStr) {
    Word word;
    word.setStr("newvalue");
    EXPECT_TRUE(compareWordToString(word, "newvalue"));
}

TEST_F(WordTest, SetStr_Replace) {
    Word word("oldvalue");
    word.setStr("newvalue");
    EXPECT_TRUE(compareWordToString(word, "newvalue"));
}

TEST_F(WordTest, SetStr_Empty) {
    Word word("oldvalue");
    word.setStr("");
    EXPECT_EQ(word.getSize(), 0);
}

// Итераторы
TEST_F(WordTest, Iterators) {
    Word word("test");
    
    EXPECT_NE(word.begin(), nullptr);
    EXPECT_NE(word.end(), nullptr);
    EXPECT_EQ(word.end() - word.begin(), 4);
    
    const Word& const_word = word;
    EXPECT_NE(const_word.begin(), nullptr);
    EXPECT_NE(const_word.end(), nullptr);
}

TEST_F(WordTest, Iterators_Empty) {
    Word word;
    
    EXPECT_EQ(word.begin(), nullptr);
    EXPECT_EQ(word.end(), nullptr);
    
    const Word& const_word = word;
    EXPECT_EQ(const_word.begin(), nullptr);
    EXPECT_EQ(const_word.end(), nullptr);
}

// Граничные случаи
TEST_F(WordTest, NullptrInput) {
    Word word(static_cast<const char*>(nullptr));
    EXPECT_EQ(word.getSize(), 0);
    
    Word word2(static_cast<const char*>(nullptr), 5);
    EXPECT_EQ(word2.getSize(), 0);
}

// Тест на конфликт конструкторов
TEST_F(WordTest, ConstructorConflictResolution) {
    // Должен вызываться конструктор Word(const char*, size_t) - извлечение слова
    Word word1("hello", 0);
    EXPECT_TRUE(compareWordToString(word1, "hello"));
    
    // Должен вызываться конструктор Word(size_t, const char*) - создание из массива
    const char arr[] = "hello";
    Word word2(5, arr);
    EXPECT_TRUE(compareWordToString(word2, "hello"));
}

// Тест на explicit конструктор
TEST_F(WordTest, ExplicitConstructor) {
    // Должен компилироваться - explicit только для одного параметра
    Word word1("hello");
    Word word2("hello", 0);
    
    EXPECT_TRUE(compareWordToString(word1, "hello"));
    EXPECT_TRUE(compareWordToString(word2, "hello"));
}

TEST_F(WordTest, Constructor_NullptrString) {
    Word word(nullptr, 0);
    EXPECT_EQ(word.getSize(), 0);
    EXPECT_EQ(word.begin(), nullptr);
}

TEST_F(WordTest, Constructor_NegativePosition) {
    Word word("hello", static_cast<std::size_t>(-1));  // unsigned, но проверяем границу
    EXPECT_EQ(word.getSize(), 0);
}

TEST_F(WordTest, Constructor_EmptyStringWithPosition) {
    Word word("", 5);
    EXPECT_EQ(word.getSize(), 0);
}

TEST_F(WordTest, Constructor_MultipleSpacesBetweenWords) {
    Word word("hello   world    test", 1);
    EXPECT_TRUE(compareWordToString(word, "world"));
}

TEST_F(WordTest, Constructor_TabsAndSpacesMix) {
    Word word("hello\t world \t test", 2);
    EXPECT_TRUE(compareWordToString(word, "test"));
}

TEST_F(WordTest, Constructor_OnlyDelimiters) {
    Word word("   \t\n   ", 0);
    EXPECT_EQ(word.getSize(), 0);
}

// Тесты для конструктора Word(size_t, const char*)
TEST_F(WordTest, ConstructorFromCharArray_NullptrWithLength) {
    Word word(5, nullptr);
    EXPECT_EQ(word.getSize(), 0);
}

TEST_F(WordTest, ConstructorFromCharArray_ZeroLength) {
    const char arr[] = "hello";
    Word word(0, arr);
    EXPECT_EQ(word.getSize(), 0);
}

TEST_F(WordTest, ConstructorFromCharArray_PartialCopy) {
    const char arr[] = "hello world";
    Word word(5, arr);  // Копируем только "hello"
    EXPECT_TRUE(compareWordToString(word, "hello"));
}

// Тесты для конструктора Word(size_t)
TEST_F(WordTest, ConstructorWithLength_Zero) {
    Word word((std::size_t) 0);
    EXPECT_EQ(word.getSize(), 0);
    EXPECT_STREQ(word.begin(), nullptr);
}

// Тесты для копирующего конструктора
TEST_F(WordTest, CopyConstructor_FromEmpty) {
    Word original;
    Word copy(original);
    EXPECT_EQ(copy.getSize(), 0);
    EXPECT_EQ(copy.begin(), nullptr);
}

TEST_F(WordTest, CopyConstructor_FromWordWithNullptr) {
    Word original(0, nullptr);
    Word copy(original);
    EXPECT_EQ(copy.getSize(), 0);
}

// Тесты для move конструктора
TEST_F(WordTest, MoveConstructor_FromEmpty) {
    Word original;
    Word moved(std::move(original));
    EXPECT_EQ(moved.getSize(), 0);
    EXPECT_EQ(original.getSize(), 0);
    EXPECT_EQ(original.begin(), nullptr);
}

TEST_F(WordTest, MoveConstructor_FromWordWithNullptr) {
    Word original(0, nullptr);
    Word moved(std::move(original));
    EXPECT_EQ(moved.getSize(), 0);
    EXPECT_EQ(original.getSize(), 0);
}

// Тесты для оператора присваивания
TEST_F(WordTest, Assignment_ToEmptyFromEmpty) {
    Word word1;
    Word word2;
    word1 = word2;
    EXPECT_EQ(word1.getSize(), 0);
}

TEST_F(WordTest, Assignment_ToNonEmptyFromEmpty) {
    Word word1("hello");
    Word word2;
    word1 = word2;
    EXPECT_EQ(word1.getSize(), 0);
    EXPECT_EQ(word1.begin(), nullptr);
}

TEST_F(WordTest, Assignment_ToEmptyFromNonEmpty) {
    Word word1;
    Word word2("hello");
    word1 = word2;
    EXPECT_TRUE(compareWordToString(word1, "hello"));
}

TEST_F(WordTest, Assignment_ToNonEmptyFromNullptrWord) {
    Word word1("hello");
    Word word2(0, nullptr);
    word1 = word2;
    EXPECT_EQ(word1.getSize(), 0);
}

// Тесты для move оператора присваивания
TEST_F(WordTest, MoveAssignment_ToEmptyFromEmpty) {
    Word word1;
    Word word2;
    word1 = std::move(word2);
    EXPECT_EQ(word1.getSize(), 0);
}

TEST_F(WordTest, MoveAssignment_ToNonEmptyFromEmpty) {
    Word word1("hello");
    Word word2;
    word1 = std::move(word2);
    EXPECT_EQ(word1.getSize(), 0);
}

// Тесты для операторов сравнения
TEST_F(WordTest, Comparison_EmptyWords) {
    Word empty1, empty2;
    EXPECT_TRUE(empty1 == empty2);
    EXPECT_FALSE(empty1 != empty2);
    EXPECT_FALSE(empty1 < empty2);
    EXPECT_TRUE(empty1 <= empty2);
    EXPECT_FALSE(empty1 > empty2);
    EXPECT_TRUE(empty1 >= empty2);
}

TEST_F(WordTest, Comparison_EmptyWithNonEmpty) {
    Word empty;
    Word nonEmpty("hello");
    
    EXPECT_FALSE(empty == nonEmpty);
    EXPECT_TRUE(empty != nonEmpty);
    EXPECT_TRUE(empty < nonEmpty);
    EXPECT_TRUE(empty <= nonEmpty);
    EXPECT_FALSE(empty > nonEmpty);
    EXPECT_FALSE(empty >= nonEmpty);
}

TEST_F(WordTest, Comparison_NonEmptyWithEmpty) {
    Word nonEmpty("hello");
    Word empty;
    
    EXPECT_FALSE(nonEmpty == empty);
    EXPECT_TRUE(nonEmpty != empty);
    EXPECT_FALSE(nonEmpty < empty);
    EXPECT_FALSE(nonEmpty <= empty);
    EXPECT_TRUE(nonEmpty > empty);
    EXPECT_TRUE(nonEmpty >= empty);
}

TEST_F(WordTest, Comparison_SameLengthDifferentContent) {
    Word word1("apple");
    Word word2("banana");
    
    EXPECT_TRUE(word1 < word2);
    EXPECT_FALSE(word1 > word2);
}

// Тесты для toLowerCase
TEST_F(WordTest, ToLowerCase_EmptyWord) {
    Word word;
    word.toLowerCase();
    EXPECT_EQ(word.getSize(), 0);
}

TEST_F(WordTest, ToLowerCase_AlreadyLower) {
    Word word("hello");
    word.toLowerCase();
    EXPECT_TRUE(compareWordToString(word, "hello"));
}

TEST_F(WordTest, ToLowerCase_MixedCase) {
    Word word("HeLLo WoRLd");
    word.toLowerCase();
    EXPECT_TRUE(compareWordToString(word, "hello"));
}

TEST_F(WordTest, ToLowerCase_SpecialChars) {
    Word word("TEST123!");
    word.toLowerCase();
    EXPECT_TRUE(compareWordToString(word, "test123!"));
}

// Тесты для startsWith
TEST_F(WordTest, StartsWith_NullptrPrefix) {
    Word word("hello");
    EXPECT_TRUE(word.startsWith(nullptr));  // Согласно вашей логике
}

TEST_F(WordTest, StartsWith_EmptyPrefix) {
    Word word("hello");
    EXPECT_TRUE(word.startsWith(""));
}

TEST_F(WordTest, StartsWith_PrefixLongerThanWord) {
    Word word("hi");
    EXPECT_FALSE(word.startsWith("hello"));
}

TEST_F(WordTest, StartsWith_EmptyWord) {
    Word word;
    EXPECT_FALSE(word.startsWith("test"));
}

TEST_F(WordTest, StartsWith_EmptyWordWithEmptyPrefix) {
    Word word;
    Word empty;
    EXPECT_TRUE(word.startsWith(empty));
}

TEST_F(WordTest, StartsWith_WordPrefixLongerThanWord) {
    Word word("hi");
    Word prefix("hello");
    EXPECT_FALSE(word.startsWith(prefix));
}

// Тесты для getCStr
TEST_F(WordTest, GetCStr_EmptyWord) {
    Word word;
    char* cstr = word.getCStr();
    // В зависимости от реализации - либо nullptr, либо пустая строка
    if (cstr != nullptr) {
        EXPECT_STREQ(cstr, "");
        delete[] cstr;
    }
}

TEST_F(WordTest, GetCStr_WordWithNullptrArray) {
    Word word(nullptr);  // Создает word с size=0, array=?
    char* cstr = word.getCStr();
    if (cstr != nullptr) {
        delete[] cstr;
    }
}

// Тесты для setStr
TEST_F(WordTest, SetStr_Nullptr) {
    Word word("hello");
    word.setStr(nullptr);
    EXPECT_EQ(word.getSize(), 0);
}

TEST_F(WordTest, SetStr_EmptyString) {
    Word word("hello");
    word.setStr("");
    EXPECT_EQ(word.getSize(), 0);
}

TEST_F(WordTest, SetStr_ToEmptyFromNonEmpty) {
    Word word;
    word.setStr("hello");
    EXPECT_TRUE(compareWordToString(word, "hello"));
}

TEST_F(WordTest, SetStr_ReplaceWithShorter) {
    Word word("longword");
    word.setStr("short");
    EXPECT_TRUE(compareWordToString(word, "short"));
}

TEST_F(WordTest, SetStr_ReplaceWithLonger) {
    Word word("short");
    word.setStr("verylongword");
    EXPECT_TRUE(compareWordToString(word, "verylongword"));
}

// Тесты для итераторов
TEST_F(WordTest, Iterators_EmptyWord) {
    Word word;
    EXPECT_EQ(word.begin(), nullptr);
    EXPECT_EQ(word.end(), nullptr);
    
    const Word& constWord = word;
    EXPECT_EQ(constWord.begin(), nullptr);
    EXPECT_EQ(constWord.end(), nullptr);
}

TEST_F(WordTest, Iterators_WordWithNullptrArray) {
    Word word((size_t) 0);
    word.begin();
    word.end();
}

// Тесты для граничных случаев с делимитерами
TEST_F(WordTest, CustomDelimiters_SingleChar) {
    Word::setDelimiters(",");
    Word word("hello,world,test", 2);
    EXPECT_TRUE(compareWordToString(word, "test"));
}

TEST_F(WordTest, CustomDelimiters_MultipleChars) {
    Word::setDelimiters(".,;");
    Word word("hello.world;test,again", 3);
    EXPECT_TRUE(compareWordToString(word, "again"));
}

TEST_F(WordTest, CustomDelimiters_NoMatch) {
    Word::setDelimiters("|");
    Word word("hello world", 0);
    EXPECT_TRUE(compareWordToString(word, "hello world"));  // Нет делимитеров - вся строка
}

// Тесты для operator== с разными случаями
TEST_F(WordTest, Equality_BothEmpty) {
    Word w1, w2;
    EXPECT_TRUE(w1 == w2);
}

TEST_F(WordTest, Equality_OneEmptyOneNonEmpty) {
    Word w1, w2("hello");
    EXPECT_FALSE(w1 == w2);
}

TEST_F(WordTest, Equality_SameContent) {
    Word w1("hello"), w2("hello");
    EXPECT_TRUE(w1 == w2);
}

TEST_F(WordTest, Equality_DifferentContentSameLength) {
    Word w1("hello"), w2("world");
    EXPECT_FALSE(w1 == w2);
}

TEST_F(WordTest, Equality_DifferentLength) {
    Word w1("hello"), w2("hell");
    EXPECT_FALSE(w1 == w2);
}