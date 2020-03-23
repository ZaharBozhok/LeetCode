create_project(subProjName)
add_executable(${subProjName} main.cpp)
target_compile_options(${subProjName} PRIVATE -Wall)

if(BUILD_TESTING)
    package_add_test("${subProjName}Test" test.cpp)
    target_compile_options("${subProjName}Test" PRIVATE -Wall)
endif()