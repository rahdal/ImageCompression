add_executable(compression
               image.cpp 
               main.cpp)

target_include_directories(compression PUBLIC include)
target_compile_options(compression PRIVATE -Wconversion -Wall -Wextra -pedantic -pthread)