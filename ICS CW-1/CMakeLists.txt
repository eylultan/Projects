cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME inf2cs-2023-coursework1)

project(${PROJECT_NAME})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -g")

set(INCLUDE_DIRS ${INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)

add_executable(word_count src/t1_word_count.c)
add_executable(print_words src/t2_print_words.c)
add_executable(unique_words src/t3_unique_words.c)
add_executable(compress src/t4_compress.c)
add_executable(compress_more src/t5_compress_more.c)
add_executable(huffman src/t6_huffman.c)




enable_testing()


### TESTS ###
### 1 ###
add_test(NAME 1.1C
        COMMAND bash -c "echo 'computer science is nice' > task1.txt; ./word_count")
set_tests_properties(1.1C PROPERTIES
        PASS_REGULAR_EXPRESSION "4\n")

add_test(NAME 1.1MIPS
        COMMAND bash -c "echo 'computer science is nice' > task1.txt; java -jar ./src/Mars.jar sm me src/t1_word_count.s")
set_tests_properties(1.1MIPS PROPERTIES
        PASS_REGULAR_EXPRESSION "4\n")


### 2 ###
add_test(NAME 2.1C
        COMMAND bash -c "echo 'computer science is nice' > task2.txt; ./print_words")
set_tests_properties(2.1C PROPERTIES
        PASS_REGULAR_EXPRESSION "computer\nscience\nis\nnice\n")

add_test(NAME 2.1MIPS
        COMMAND bash -c "echo 'computer science is nice' > task2.txt; java -jar ./src/Mars.jar sm me src/t2_print_words.s")
set_tests_properties(2.1MIPS PROPERTIES
        PASS_REGULAR_EXPRESSION "computer\nscience\nis\nnice\n")



### 3 ###
add_test(NAME 3.1C
        COMMAND bash -c "echo 'bob meets little mary and mary meets bob' > task3.txt; ./unique_words")
set_tests_properties(3.1C PROPERTIES
        PASS_REGULAR_EXPRESSION "1 bob\n2 meets\n3 little\n4 mary\n5 and\n")

add_test(NAME 3.1MIPS
        COMMAND bash -c "echo 'bob meets little mary and mary meets bob' > task3.txt; java -jar ./src/Mars.jar sm me src/t3_unique_words.s")
set_tests_properties(3.1MIPS PROPERTIES
        PASS_REGULAR_EXPRESSION "1 bob\n2 meets\n3 little\n4 mary\n5 and\n")


### 4 ###
add_test(NAME 4.1C
        COMMAND bash -c "echo 'bob meets little mary and mary meets bob' > task4.txt; ./compress")
set_tests_properties(4.1C PROPERTIES
        PASS_REGULAR_EXPRESSION "1 2 3 4 5 4 2 1\n")

add_test(NAME 4.1MIPS
        COMMAND bash -c "echo 'bob meets little mary and mary meets bob' > task4.txt; java -jar ./src/Mars.jar sm me src/t4_compress.s")
set_tests_properties(4.1MIPS PROPERTIES
        PASS_REGULAR_EXPRESSION "1 2 3 4 5 4 2 1\n")


### 5 ###
add_test(NAME 5.1C
        COMMAND bash -c "echo 'bob meets little mary and mary meets bob' > task5.txt; ./compress_more")
set_tests_properties(5.1C PROPERTIES
        PASS_REGULAR_EXPRESSION "1 2 little 3 and 3 2 1 \n1 bob\n2 meets\n3 mary\n")

add_test(NAME 5.1MIPS
        COMMAND bash -c "echo 'bob meets little mary and mary meets bob' > task5.txt; java -jar ./src/Mars.jar sm me src/t5_compress_more.s")
set_tests_properties(5.1MIPS PROPERTIES
        PASS_REGULAR_EXPRESSION "1 2 little 3 and 3 2 1 \n1 bob\n2 meets\n3 mary\n")


### 6 ###
add_test(NAME 6.1.1C
        COMMAND bash -c "echo 'abracadabra' > task6.txt; ./huffman")
set_tests_properties(6.1.1C PROPERTIES
        PASS_REGULAR_EXPRESSION "a 5\nb 2\nr 2\nc 1\nd 1\n")

add_test(NAME 6.1.1MIPS
        COMMAND bash -c "echo 'abracadabra' > task6.txt; java -jar ./src/Mars.jar sm me src/t6_huffman.s")
set_tests_properties(6.1.1MIPS PROPERTIES
        PASS_REGULAR_EXPRESSION "a 5\nb 2\nr 2\nc 1\nd 1\n")


add_test(NAME 6.2.1C
        COMMAND bash -c "echo 'abracadabra' > task6.txt; ./huffman")
set_tests_properties(6.2.1C PROPERTIES
        PASS_REGULAR_EXPRESSION "a 5\nb 2\nr 2\nc 1\nd 1\ncdrba 11\n")

add_test(NAME 6.2.1MIPS
        COMMAND bash -c "echo 'abracadabra' > task6.txt; java -jar ./src/Mars.jar sm me src/t6_huffman.s")
set_tests_properties(6.2.1MIPS PROPERTIES
        PASS_REGULAR_EXPRESSION "a 5\nb 2\nr 2\nc 1\nd 1\ncdrba 11\n")


add_test(NAME 6.4.1C
        COMMAND bash -c "echo 'abracadabra' > task6.txt; ./huffman")
set_tests_properties(6.4.1C PROPERTIES
        PASS_REGULAR_EXPRESSION "a 5\nb 2\nr 2\nc 1\nd 1\ncdrba 11\n10100110000100011010011\n")

add_test(NAME 6.4.1MIPS
        COMMAND bash -c "echo 'abracadabra' > task6.txt; java -jar ./src/Mars.jar sm me src/t6_huffman.s")
set_tests_properties(6.4.1MIPS PROPERTIES
        PASS_REGULAR_EXPRESSION "a 5\nb 2\nr 2\nc 1\nd 1\ncdrba 11\n10100110000100011010011\n")
