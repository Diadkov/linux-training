set(FILE_SOURCES ft_tail.c ../common/errorFunctions.c)

add_executable(tail ${FILE_SOURCES})
set(CMAKE_C_STANDARD 17)

target_include_directories(tail PRIVATE ../common)
