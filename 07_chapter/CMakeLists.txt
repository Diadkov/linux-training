set(CH7_TASK1_SOURCES mallocFree.c ../common/errorFunctions.c)

add_executable(ch7Task1 ${CH7_TASK1_SOURCES})
set(CMAKE_C_STANDARD 17)

target_include_directories(ch7Task1  PRIVATE ../common)
