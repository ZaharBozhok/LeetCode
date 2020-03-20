add_executable(IntervalMap main.cpp)

if(TESTS STREQUAL "enabled")
    add_executable(IntervalMapTest test.cpp)
endif()