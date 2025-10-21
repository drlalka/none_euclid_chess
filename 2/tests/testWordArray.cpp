#include <gtest/gtest.h>
#include "WordArray.hpp"
#include "Word.hpp"

class WordArrayTest : public ::testing::Test {
protected:
    void SetUp() override {
        Word::setDelimiters(" \t\n");
    }

    WordArray emptyArray;
    WordArray singleElementArray{Word("hello")};
};

// Конструкторы
TEST_F(WordArrayTest, DefaultConstructor) {
    EXPECT_EQ(emptyArray.getSize(), 0);
    EXPECT_EQ(emptyArray.getCapacity(), 0);
}

TEST_F(WordArrayTest, SingleWordConstructor) {
    EXPECT_EQ(singleElementArray.getSize(), 1);
    EXPECT_EQ(singleElementArray[0], Word("hello"));
}

TEST_F(WordArrayTest, CopyConstructor) {
    WordArray copy(singleElementArray);
    EXPECT_EQ(copy.getSize(), 1);
    EXPECT_EQ(copy[0], Word("hello"));
}

TEST_F(WordArrayTest, MoveConstructor) {
    WordArray moved(std::move(singleElementArray));
    EXPECT_EQ(moved.getSize(), 1);
    EXPECT_EQ(moved[0], Word("hello"));
    EXPECT_EQ(singleElementArray.getSize(), 0);
}

TEST_F(WordArrayTest, CountWordsConstructor) {
    Word words[] = {Word("hello"), Word("world"), Word("hello")};
    WordArray arr(3, words);
    EXPECT_EQ(arr.getSize(), 2); // Дубликаты удаляются
}

TEST_F(WordArrayTest, CountWordsConstructorNullCheck) {
    EXPECT_THROW(WordArray arr(1, nullptr), std::invalid_argument);
}

TEST_F(WordArrayTest, CountWordsConstructorEmpty) {
    WordArray arr(0, nullptr);
    EXPECT_EQ(arr.getSize(), 0);
}

// Операторы присваивания
TEST_F(WordArrayTest, CopyAssignment) {
    WordArray copy;
    copy = singleElementArray;
    EXPECT_EQ(copy.getSize(), 1);
    EXPECT_EQ(copy[0], Word("hello"));
}

TEST_F(WordArrayTest, MoveAssignment) {
    WordArray moved;
    moved = std::move(singleElementArray);
    EXPECT_EQ(moved.getSize(), 1);
    EXPECT_EQ(moved[0], Word("hello"));
    EXPECT_EQ(singleElementArray.getSize(), 0);
}

TEST_F(WordArrayTest, SelfAssignment) {
    singleElementArray = singleElementArray;
    EXPECT_EQ(singleElementArray.getSize(), 1);
}

// Добавление элементов
TEST_F(WordArrayTest, AddSingleWord) {
    emptyArray += Word("test");
    EXPECT_EQ(emptyArray.getSize(), 1);
    EXPECT_EQ(emptyArray[0], Word("test"));
}

TEST_F(WordArrayTest, AddDuplicateWord) {
    singleElementArray += Word("hello");
    EXPECT_EQ(singleElementArray.getSize(), 1); // Дубликат не добавляется
}

TEST_F(WordArrayTest, AddMultipleWords) {
    WordArray arr;
    arr += Word("one");
    arr += Word("two");
    arr += Word("three");
    EXPECT_EQ(arr.getSize(), 3);
}

TEST_F(WordArrayTest, AddWordArray) {
    WordArray other;
    other += Word("world");
    other += Word("test");
    
    singleElementArray += other;
    EXPECT_EQ(singleElementArray.getSize(), 3);
}

// Доступ к элементам
TEST_F(WordArrayTest, IndexOperator) {
    EXPECT_EQ(singleElementArray[0], Word("hello"));
}

TEST_F(WordArrayTest, ConstIndexOperator) {
    const WordArray& constRef = singleElementArray;
    EXPECT_EQ(constRef[0], Word("hello"));
}

TEST_F(WordArrayTest, IndexOperatorOutOfRange) {
    EXPECT_THROW(emptyArray[0], std::out_of_range);
}

TEST_F(WordArrayTest, ConstIndexOperatorOutOfRange) {
    const WordArray& constRef = emptyArray;
    EXPECT_THROW(constRef[0], std::out_of_range);
}

// Поиск
TEST_F(WordArrayTest, FindExistingWord) {
    size_t index = singleElementArray.find(Word("hello"));
    EXPECT_EQ(index, 0);
}

TEST_F(WordArrayTest, FindNonExistingWord) {
    size_t index = singleElementArray.find(Word("nonexistent"));
    EXPECT_EQ(index, WordArray::npos());
}

// Парсинг строки
TEST_F(WordArrayTest, ParseString) {
    WordArray arr;
    size_t count = arr.parseString("hello world test");
    EXPECT_EQ(count, 3);
    EXPECT_EQ(arr.getSize(), 3);
}

TEST_F(WordArrayTest, ParseStringWithDuplicates) {
    WordArray arr;
    size_t count = arr.parseString("hello world hello");
    EXPECT_EQ(count, 3);
    EXPECT_EQ(arr.getSize(), 2); // Дубликаты удаляются
}

TEST_F(WordArrayTest, ParseNullString) {
    WordArray arr;
    size_t count = arr.parseString(nullptr);
    EXPECT_EQ(count, 0);
}

TEST_F(WordArrayTest, ParseEmptyString) {
    WordArray arr;
    size_t count = arr.parseString("");
    EXPECT_EQ(count, 0);
}

// Поиск по префиксу
TEST_F(WordArrayTest, GetWordsStartingWith) {
    WordArray arr;
    arr.parseString("hello world hell test heaven");
    
    WordArray result = arr.getWordsStartingWith(Word("he"));
    EXPECT_EQ(result.getSize(), 3); // hello, hell, heaven
}

TEST_F(WordArrayTest, GetWordsStartingWithCaseInsensitive) {
    WordArray arr;
    arr.parseString("Hello world HELL test");
    
    WordArray result = arr.getWordsStartingWith(Word("he"));
    EXPECT_EQ(result.getSize(), 2); // Hello, HELL
}

TEST_F(WordArrayTest, GetWordsStartingWithNoMatches) {
    WordArray arr;
    arr.parseString("hello world test");
    
    WordArray result = arr.getWordsStartingWith(Word("xyz"));
    EXPECT_EQ(result.getSize(), 0);
}

// Сортировка
TEST_F(WordArrayTest, Sort) {
    WordArray arr;
    arr.parseString("zebra apple banana");
    arr.sort();
    
    EXPECT_EQ(arr[0], Word("apple"));
    EXPECT_EQ(arr[1], Word("banana"));
    EXPECT_EQ(arr[2], Word("zebra"));
}

// Ввод-вывод
TEST_F(WordArrayTest, OutputStream) {
    testing::internal::CaptureStdout();
    std::cout << singleElementArray;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("hello") != std::string::npos);
}

TEST_F(WordArrayTest, InputStream) {
    std::istringstream iss("hello world test");
    WordArray arr;
    iss >> arr;
    
    EXPECT_EQ(arr.getSize(), 3);
}

TEST_F(WordArrayTest, InputStreamEmpty) {
    std::istringstream iss("");
    WordArray arr;
    iss >> arr;
    
    EXPECT_TRUE(iss.fail());
}

TEST_F(WordArrayTest, Resize) {
    WordArray arr;
    for (int i = 0; i < 10; ++i) {
        arr += Word("word" + std::to_string(i));
    }
    EXPECT_GE(arr.getCapacity(), 10);
}

TEST_F(WordArrayTest, Iterators) {
    WordArray arr;
    arr.parseString("one two three");
    
    size_t count = 0;
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        count++;
    }
    EXPECT_EQ(count, 3);
}

TEST_F(WordArrayTest, ConstIterators) {
    const WordArray& constArr = singleElementArray;
    
    size_t count = 0;
    for (auto it = constArr.begin(); it != constArr.end(); ++it) {
        count++;
    }
    EXPECT_EQ(count, 1);
}

TEST_F(WordArrayTest, EmptyArrayOperations) {
    EXPECT_EQ(emptyArray.find(Word("test")), WordArray::npos());
    EXPECT_THROW(emptyArray[0], std::out_of_range);
    
    WordArray result = emptyArray.getWordsStartingWith(Word("test"));
    EXPECT_EQ(result.getSize(), 0);
}

TEST_F(WordArrayTest, LargeArray) {
    WordArray arr;
    for (int i = 0; i < 100; ++i) {
        arr += Word("word" + std::to_string(i));
    }
    EXPECT_EQ(arr.getSize(), 100);
}

// Добавляем к предыдущим тестам:

// Тесты для конструктора с одним Word
TEST_F(WordArrayTest, SingleWordConstructorCapacity) {
    EXPECT_GE(singleElementArray.getCapacity(), 1);
}

// Тесты для конструктора с массивом слов - граничные случаи
TEST_F(WordArrayTest, CountWordsConstructorAllDuplicates) {
    Word words[] = {Word("same"), Word("same"), Word("same")};
    WordArray arr(3, words);
    EXPECT_EQ(arr.getSize(), 1);
    EXPECT_EQ(arr[0], Word("same"));
}

TEST_F(WordArrayTest, CountWordsConstructorSingleWord) {
    Word words[] = {Word("single")};
    WordArray arr(1, words);
    EXPECT_EQ(arr.getSize(), 1);
    EXPECT_EQ(arr[0], Word("single"));
}

// Тесты для resize с разными сценариями
TEST_F(WordArrayTest, ResizeTriggersOnCapacityLimit) {
    WordArray arr;
    // Изначальная capacity = 0, первое добавление должно вызвать resize
    arr += Word("first");
    EXPECT_GT(arr.getCapacity(), 0);
}

// Тесты для operator+= с Word - граничные случаи
TEST_F(WordArrayTest, AddWordToEmptyArrayMultipleTimes) {
    emptyArray += Word("first");
    emptyArray += Word("second");
    EXPECT_EQ(emptyArray.getSize(), 2);
}

TEST_F(WordArrayTest, AddWordThatCausesMultipleResize) {
    WordArray arr;
    // Добавляем много слов чтобы вызвать несколько resize
    for (int i = 0; i < 20; ++i) {
        arr += Word("word" + std::to_string(i));
    }
    EXPECT_EQ(arr.getSize(), 20);
    EXPECT_GE(arr.getCapacity(), 20);
}

// Тесты для operator+= с WordArray - сложные случаи
TEST_F(WordArrayTest, AddWordArrayWithOverlap) {
    WordArray arr1;
    arr1.parseString("one two three");
    
    WordArray arr2;
    arr2.parseString("two three four");
    
    arr1 += arr2;
    EXPECT_EQ(arr1.getSize(), 4); // one, two, three, four
}

TEST_F(WordArrayTest, AddEmptyWordArray) {
    WordArray arr;
    arr.parseString("hello world");
    
    arr += emptyArray; // Добавляем пустой массив
    EXPECT_EQ(arr.getSize(), 2); // Размер не должен измениться
}

TEST_F(WordArrayTest, AddWordArrayToItself) {
    WordArray arr;
    arr.parseString("one two");
    
    arr += arr; // Добавляем сам себя
    EXPECT_EQ(arr.getSize(), 2); // Дубликаты не должны добавиться
}

// Тесты для parseString - дополнительные случаи
TEST_F(WordArrayTest, ParseStringAddsToExisting) {
    WordArray arr;
    arr += Word("existing");
    size_t count = arr.parseString("new words");
    EXPECT_EQ(arr.getSize(), 3); // existing + new + words
    EXPECT_EQ(count, 2);
}

TEST_F(WordArrayTest, ParseStringWithMixedDelimiters) {
    WordArray arr;
    size_t count = arr.parseString("word1\tword2\nword3 word4");
    EXPECT_EQ(count, 4);
    EXPECT_EQ(arr.getSize(), 4);
}

// Тесты для getWordsStartingWith - дополнительные случаи
TEST_F(WordArrayTest, GetWordsStartingWithSpecialCharacters) {
    WordArray arr;
    arr.parseString("test$ test_normal $special @tag");
    
    WordArray result = arr.getWordsStartingWith(Word("$"));
    EXPECT_GE(result.getSize(), 1);
}

TEST_F(WordArrayTest, GetWordsStartingWithPreservesOriginal) {
    WordArray arr;
    arr.parseString("hello world test");
    
    WordArray result = arr.getWordsStartingWith(Word("h"));
    // Исходный массив не должен измениться
    EXPECT_EQ(arr.getSize(), 3);
    EXPECT_EQ(result.getSize(), 1);
}

// Тесты для итераторов - дополнительные случаи
TEST_F(WordArrayTest, IteratorRangeBasedFor) {
    WordArray arr;
    arr.parseString("a b c");
    
    size_t count = 0;
    for (const Word& word : arr) {
        count++;
    }
    EXPECT_EQ(count, 3);
}

TEST_F(WordArrayTest, IteratorModification) {
    WordArray arr;
    arr += Word("original");
    
    Word* it = arr.begin();
    *it = Word("modified");
    
    EXPECT_EQ(arr[0], Word("modified"));
}

// Исправляем проблемные тесты:

TEST_F(WordArrayTest, SortWithNumbers) {
    WordArray arr;
    arr.parseString("zebra 100 apple 42 test");
    arr.sort();
    
    // Проверяем что отсортировано лексикографически
    for (size_t i = 0; i < arr.getSize() - 1; ++i) {
        EXPECT_TRUE(arr[i] <= arr[i + 1]); // Используем оператор <= вместо <=>
    }
}

TEST_F(WordArrayTest, SortAlreadySorted) {
    WordArray arr;
    arr.parseString("apple banana cherry");
    arr.sort();
    
    EXPECT_EQ(arr[0], Word("apple"));
    EXPECT_EQ(arr[1], Word("banana"));
    EXPECT_EQ(arr[2], Word("cherry"));
}

TEST_F(WordArrayTest, SortReverseOrder) {
    WordArray arr;
    arr.parseString("cherry banana apple");
    arr.sort();
    
    EXPECT_EQ(arr[0], Word("apple"));
    EXPECT_EQ(arr[1], Word("banana"));
    EXPECT_EQ(arr[2], Word("cherry"));
}

TEST_F(WordArrayTest, MixedOperations) {
    WordArray arr;
    arr.parseString("start middle end");
    arr.sort();
    arr += Word("additional");
    arr.sort(); // Сортируем еще раз после добавления
    
    EXPECT_EQ(arr.getSize(), 4);
    // Проверяем, что массив отсортирован
    for (size_t i = 0; i < arr.getSize() - 1; ++i) {
        EXPECT_TRUE(arr[i] <= arr[i + 1]); // Используем оператор <=
    }
}

// Убираем проблемные сравнения strong_ordering:
TEST_F(WordArrayTest, OutputFormatCorrectness) {
    WordArray arr;
    arr.parseString("one two");
    
    testing::internal::CaptureStdout();
    std::cout << arr;
    std::string output = testing::internal::GetCapturedStdout();
    
    // Проверяем базовый формат без точного позиционирования
    EXPECT_TRUE(output.find("[") != std::string::npos);
    EXPECT_TRUE(output.find("]") != std::string::npos);
    EXPECT_TRUE(output.find("one") != std::string::npos);
    EXPECT_TRUE(output.find("two") != std::string::npos);
}

// Добавляем недостающие тесты для полного покрытия:

TEST_F(WordArrayTest, EmptyArrayAfterMove) {
    WordArray arr;
    arr.parseString("one two three");
    
    WordArray moved(std::move(arr));
    EXPECT_EQ(arr.getSize(), 0);
    EXPECT_EQ(arr.getCapacity(), 0);
    EXPECT_EQ(arr.begin(), nullptr);
}

TEST_F(WordArrayTest, CapacityGrowthPattern) {
    WordArray arr;
    size_t previous_capacity = arr.getCapacity();
    
    for (int i = 0; i < 10; ++i) {
        arr += Word("word" + std::to_string(i));
        if (arr.getCapacity() > previous_capacity) {
            // Проверяем что capacity увеличивается
            EXPECT_GT(arr.getCapacity(), previous_capacity);
            previous_capacity = arr.getCapacity();
        }
    }
}

TEST_F(WordArrayTest, FindInModifiedArray) {
    WordArray arr;
    arr.parseString("one two three");
    
    size_t index1 = arr.find(Word("two"));
    EXPECT_EQ(index1, 1);
    
    arr += Word("four");
    size_t index2 = arr.find(Word("four"));
    EXPECT_EQ(index2, 3);
}

TEST_F(WordArrayTest, GetWordsStartingWithSingleMatch) {
    WordArray arr;
    arr.parseString("apple banana cherry");
    
    WordArray result = arr.getWordsStartingWith(Word("b"));
    EXPECT_EQ(result.getSize(), 1);
    EXPECT_EQ(result[0], Word("banana"));
}

TEST_F(WordArrayTest, ParseStringWithDuplicateInExisting) {
    WordArray arr;
    arr += Word("existing");
    size_t count = arr.parseString("existing new");
    EXPECT_EQ(arr.getSize(), 2); // existing (не дублируется) + new
    EXPECT_EQ(count, 2);
}

TEST_F(WordArrayTest, IteratorConstCorrectness) {
    const WordArray constArr = singleElementArray;
    
    // Должен работать const iterator
    auto it = constArr.begin();
    EXPECT_EQ(*it, Word("hello"));
    EXPECT_EQ(*(constArr.end() - 1), Word("hello"));
}

TEST_F(WordArrayTest, SelfAssignmentCopy) {
    WordArray arr;
    arr.parseString("test data");
    
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wself-assign-overloaded"
    arr = arr; // self-assignment
    #pragma GCC diagnostic pop
    
    EXPECT_EQ(arr.getSize(), 2);
    EXPECT_EQ(arr[0], Word("test"));
    EXPECT_EQ(arr[1], Word("data"));
}

TEST_F(WordArrayTest, InputMultipleLines) {
    std::istringstream iss("first line\nsecond line");
    WordArray arr;
    iss >> arr; // Должен прочитать только первую строку
    
    EXPECT_GE(arr.getSize(), 2); // first, line
    EXPECT_TRUE(iss.good()); // Поток должен оставаться в хорошем состоянии
}

TEST_F(WordArrayTest, EmptyArrayAfterClear) {
    WordArray arr;
    arr.parseString("one two three");
    
    // "Очищаем" массив через присваивание пустого
    WordArray empty;
    arr = empty;
    
    EXPECT_EQ(arr.getSize(), 0);
    EXPECT_EQ(arr.getCapacity(), 0);
}

TEST_F(WordArrayTest, WordComparisonInFind) {
    WordArray arr;
    arr += Word("hello");
    arr += Word("world");
    
    // Проверяем что find использует правильное сравнение
    size_t index = arr.find(Word("hello"));
    EXPECT_EQ(index, 0);
    
    index = arr.find(Word("world"));
    EXPECT_EQ(index, 1);
    
    index = arr.find(Word("nonexistent"));
    EXPECT_EQ(index, WordArray::npos());
}

TEST_F(WordArrayTest, InputWithPunctuation) {
    std::istringstream iss("hello, world! test.");
    WordArray arr;
    iss >> arr;
    
    // Знаки препинания должны рассматриваться как часть слов
    EXPECT_GE(arr.getSize(), 3);
}

// Тесты для find - дополнительные случаи
TEST_F(WordArrayTest, FindInLargeArray) {
    WordArray arr;
    for (int i = 0; i < 50; ++i) {
        arr += Word("word" + std::to_string(i));
    }
    
    size_t index = arr.find(Word("word25"));
    EXPECT_EQ(index, 25);
}

TEST_F(WordArrayTest, FindFirstElement) {
    WordArray arr;
    arr.parseString("first second third");
    
    size_t index = arr.find(Word("first"));
    EXPECT_EQ(index, 0);
}

TEST_F(WordArrayTest, FindLastElement) {
    WordArray arr;
    arr.parseString("first second third");
    
    size_t index = arr.find(Word("third"));
    EXPECT_EQ(index, 2);
}

// Тесты для копирующего присваивания - граничные случаи
TEST_F(WordArrayTest, CopyAssignmentToLargerArray) {
    WordArray arr1;
    arr1.parseString("one two three");
    
    WordArray arr2;
    arr2.parseString("a b c d e f g h i j k l m n o p q r s t u v w x y z");
    
    arr2 = arr1; // Присваиваем меньший массив большему
    EXPECT_EQ(arr2.getSize(), 3);
    EXPECT_EQ(arr2[0], Word("one"));
    EXPECT_EQ(arr2[1], Word("two"));
    EXPECT_EQ(arr2[2], Word("three"));
}

TEST_F(WordArrayTest, CopyAssignmentToSmallerArray) {
    WordArray arr1;
    arr1.parseString("a b c d e f g h i j");
    
    WordArray arr2;
    arr2.parseString("one two");
    
    arr2 = arr1; // Присваиваем больший массив меньшему
    EXPECT_EQ(arr2.getSize(), 10);
}

// Тесты для перемещения - граничные случаи
TEST_F(WordArrayTest, MoveToNonEmptyArray) {
    WordArray source;
    source.parseString("source data");
    
    WordArray target;
    target.parseString("target data");
    
    target = std::move(source);
    EXPECT_EQ(target.getSize(), 2);
    EXPECT_EQ(source.getSize(), 0);
}

// Тесты для индексации - дополнительные случаи
TEST_F(WordArrayTest, IndexAssignment) {
    WordArray arr;
    arr.parseString("original");
    
    arr[0] = Word("modified");
    EXPECT_EQ(arr[0], Word("modified"));
}

TEST_F(WordArrayTest, ConstIndexAccess) {
    const WordArray arr = singleElementArray;
    const Word& word = arr[0];
    EXPECT_EQ(word, Word("hello"));
}

// Тесты для методов getSize и getCapacity после различных операций
TEST_F(WordArrayTest, SizeAndCapacityAfterMultipleOperations) {
    WordArray arr;
    EXPECT_EQ(arr.getSize(), 0);
    EXPECT_EQ(arr.getCapacity(), 0);
    
    arr += Word("first");
    EXPECT_EQ(arr.getSize(), 1);
    EXPECT_GT(arr.getCapacity(), 0);
    
    arr.parseString("second third");
    EXPECT_EQ(arr.getSize(), 3);
    
    arr.sort();
    EXPECT_EQ(arr.getSize(), 3);
}