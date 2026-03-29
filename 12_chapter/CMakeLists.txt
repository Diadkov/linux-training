set(FILE_SOURCES getProcessesInfo.c ../common/errorFunctions.c)

add_executable(processesInfo ${FILE_SOURCES})
set(CMAKE_C_STANDARD 17)

target_include_directories(processesInfo PRIVATE ../common)
