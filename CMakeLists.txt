cmake_minimum_required(VERSION 3.17)
project(LeetCode-and-such-stuff)
set(CMAKE_CXX_STANDARD 17)

macro(create_project projectName)
    get_filename_component(${projectName} ${CMAKE_CURRENT_LIST_DIR} NAME)
endmacro()

option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    include(googletest)
    include(CTest)
    add_subdirectory("${PROJECT_SOURCE_DIR}/googletest" "googletest")
    mark_as_advanced(
        BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
        gmock_build_tests gtest_build_samples gtest_build_tests
        gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
    )
    macro(package_add_test TESTNAME)
        add_executable(${TESTNAME} ${ARGN})
        target_link_libraries(${TESTNAME} gtest gmock gtest_main)
        add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
    endmacro()
endif()

add_subdirectory(IntervalMap)
add_subdirectory(Candy)