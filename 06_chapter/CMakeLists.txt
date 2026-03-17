set(JUMP_SOURCES exercise2.c ../common/errorFunctions.c)

add_executable(jump_broken ${JUMP_SOURCES})
set(CMAKE_C_STANDARD 17)

target_include_directories(jump_broken PRIVATE ../common)
