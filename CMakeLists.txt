cmake_minimum_required(VERSION 3.25)
project(student_management_system VERSION 2.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Crow CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)

add_library(student_core
    backend/src/student_service.cpp
    backend/src/student_repository.cpp
)
target_include_directories(student_core PUBLIC backend/include)
target_link_libraries(student_core PUBLIC unofficial::sqlite3::sqlite3)

add_executable(student-api backend/src/main.cpp)
target_link_libraries(student-api PRIVATE student_core Crow::Crow)

if(MSVC)
    target_compile_options(student_core PRIVATE /W4 /permissive-)
    target_compile_options(student-api PRIVATE /W4 /permissive-)
else()
    target_compile_options(student_core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(student-api PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(CTest)
if(BUILD_TESTING)
    add_executable(student-core-tests backend/tests/validation_test.cpp)
    target_link_libraries(student-core-tests PRIVATE student_core)
    add_test(NAME student-core-tests COMMAND student-core-tests)

    add_executable(student-repository-tests backend/tests/repository_test.cpp)
    target_link_libraries(student-repository-tests PRIVATE student_core)
    add_test(NAME student-repository-tests COMMAND student-repository-tests)
endif()
