set(SOURCES_EX2356 exercise3.c ../common/errorFunctions.c)

add_executable(atomic_append ${SOURCES_EX2356})
set(CMAKE_C_STANDARD 17)

target_include_directories(atomic_append PRIVATE ../common)

