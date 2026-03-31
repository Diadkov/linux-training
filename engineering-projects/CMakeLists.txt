set(FILE_SOURCES ft_malloc.c ../common/errorFunctions.c)

add_executable(malloc ${FILE_SOURCES})
set(CMAKE_C_STANDARD 17)

target_include_directories(malloc PRIVATE ../common .)
