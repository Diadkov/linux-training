set(CH7_TASK1_SOURCES mallocFree.c ../common/errorFunctions.c)

add_executable(MallocFree ${CH7_TASK1_SOURCES})
set(CMAKE_C_STANDARD 17)

target_include_directories(MallocFree PRIVATE ../common)
