set(SubProj "IntervalMap")

add_executable(${SubProj} main.cpp)

if(BUILD_TESTING)
    package_add_test("${SubProj}Test" test.cpp)
endif()