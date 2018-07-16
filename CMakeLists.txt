cmake_minimum_required (VERSION 3.5)


if (MINGW)
	message(STATUS "MINGW project")
	set(MINGW_BIN_DIR "$ENV{cagr}/partners/open_source/mingw32/bin")
	set(CMAKE_C_COMPILER "${MINGW_BIN_DIR}/gcc.exe")
	set(CMAKE_CXX_COMPILER "${MINGW_BIN_DIR}/g++.exe")
	set(COMPILE_ADDITONAL_FLAGS -Wall -Werror -std=c++11)
	set(EXECUTABLE_OUTPUT_PATH "bin")
else()
	message(STATUS "Non MINGW project")
endif()

set(TARGET_NAME "MedianFinder")
set(TARGET_NAME_TESTS ${TARGET_NAME}_test)

project("${target_name}")

set(SRC_DIR "src")
set(TESTS_SRC_DIR "unit_tests")
set(EXTERNAL_DIR "external")

set(SRC 
	${SRC_DIR}/main.cpp
	${SRC_DIR}/medianFinder.cpp
)

set(TESTS_SRC
	${TESTS_SRC_DIR}/heap_test.cpp
)


add_executable(${TARGET_NAME} ${SRC})


set(WINDOWS_VERSION 0x0601)#Windows 7 - minimal version https://msdn.microsoft.com/en-us/library/6sehtctf.aspx
target_compile_definitions(${TARGET_NAME} PRIVATE _WIN32_WINNT=${WINDOWS_VERSION} INI_HANDLER_LINENO=1)

if (MINGW)
    target_link_libraries(${TARGET_NAME} PUBLIC "-static")
endif()

target_include_directories(${TARGET_NAME} 
	PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/inc"
)

target_link_libraries(${TARGET_NAME})

target_compile_options(${TARGET_NAME} PUBLIC ${COMPILE_ADDITONAL_FLAGS})
target_compile_definitions(${TARGET_NAME} PUBLIC "_CRT_SECURE_NO_WARNINGS")


####################################
## GTest
####################################
#enable_testing()
# set(GTEST_SOURCE_DIR $ENV{cagr}/partners/open_source/googletest)
# set(GTEST_LIB_DIR ${GTEST_SOURCE_DIR}/lib)
# include_directories(${GTEST_SOURCE_DIR}/include ${GTEST_SOURCE_DIR}/src)

# add_executable(${TARGET_NAME_TESTS} ${TESTS_SRC})

# target_include_directories(${TARGET_NAME_TESTS} 
# 	PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/inc"
# )

# target_link_libraries(${TARGET_NAME_TESTS} ${GTEST_LIB_DIR}/libgtest.a ${GTEST_LIB_DIR}/libgtest_main.a)

# target_compile_options(${TARGET_NAME_TESTS} PUBLIC ${COMPILE_ADDITONAL_FLAGS})

# add_test(
#     NAME ${TARGET_NAME_TESTS}
#     COMMAND ${TARGET_NAME_TESTS}
# )