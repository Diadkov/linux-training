set(ENVIRONMENT_SOURCES exercise3.c ../common/errorFunctions.c)

add_executable(myEnvironment ${ENVIRONMENT_SOURCES})
set(CMAKE_C_STANDARD 17)

target_include_directories(myEnvironment PRIVATE ../common)
