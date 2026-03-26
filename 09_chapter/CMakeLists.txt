set(FILE_SOURCES setuidtask.c ../common/errorFunctions.c)

add_executable(differentIDs ${FILE_SOURCES})
set(CMAKE_C_STANDARD 17)

target_include_directories(differentIDs  PRIVATE ../common)
