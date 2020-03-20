create_project(subProjName)
add_executable(${subProjName} main.cpp)

if(BUILD_TESTING)
    package_add_test("${subProjName}Test" test.cpp)
endif()