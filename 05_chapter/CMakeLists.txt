set(SOURCES_EX2356 exercise5.c ../common/errorFunctions.c)

add_executable(EXE5 ${SOURCES_EX2356})
set(CMAKE_C_STANDARD 17)

target_include_directories(EXE5 PRIVATE ../common)

