cmake_minimum_required(VERSION 3.10.0)
project(OOPPROJECT VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect all source files from LifeSync/src/
file(GLOB SOURCES
    "LifeSync/src/*.cpp"
)

add_executable(OOPPROJECT
    LifeSync/main.cpp
    ${SOURCES}
)

# Add LifeSync as include root so #include "include/Foo.h" works
target_include_directories(OOPPROJECT PRIVATE LifeSync)

# Link std::filesystem (required for GCC < 9)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
    target_link_libraries(OOPPROJECT PRIVATE stdc++fs)
endif()
