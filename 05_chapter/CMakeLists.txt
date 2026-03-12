set(SOURCES_EX2356 main.c ../common/errorFunctions.c)

add_executable(chapter5 ${SOURCES_EX2356})
set(CMAKE_C_STANDARD 17)

target_include_directories(chapter5 PRIVATE ../common)

