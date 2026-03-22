set(FILE_SOURCES ft_getpwnam.c ../common/errorFunctions.c)

add_executable(GetpwnameTask ${FILE_SOURCES})
set(CMAKE_C_STANDARD 17)

target_include_directories(GetpwnameTask  PRIVATE ../common)
