cmake_minimum_required(VERSION 3.10)


project(ConsoleMath VERSION 1.0)

set(SOURCES
    char.cpp
    expression.cpp
    function.cpp
    local_math.cpp
    main.cpp
    parse_tree.cpp
    parser.cpp
    symbol_table.cpp
    tokenizer.cpp
)


set(LIBS
    stdc++
    m
)


add_executable(math ${SOURCES})
target_link_libraries(math ${LIBS})