set(SOURCES_EX2 main.c ../common/errorFunctions.c)

add_executable(chapter1Ex2 ${SOURCES_EX2})
set(CMAKE_C_STANDARD 17)
target_include_directories(chapter1Ex2 PRIVATE ../common)

