set(FILE_SOURCES timePractice.c ../common/errorFunctions.c)

add_executable(time ${FILE_SOURCES})
set(CMAKE_C_STANDARD 17)

target_include_directories(time PRIVATE ../common)
