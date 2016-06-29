cmake_minimum_required(VERSION 2.8.7)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

project(reimbursements)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

set(REIMBURSEMENTS_ID ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE INTERNAL "")
set(REIMBURSEMENTS_SD ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INI_ID ${CMAKE_CURRENT_SOURCE_DIR}/opt/ini/src)

if(NOT ${CMAKE_VERSION} LESS 3.2)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
else()
    message(STATUS "Checking compiler flags for C++11 support.")
    # Set C++11 support flags for various compilers
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
    check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
    if(COMPILER_SUPPORTS_CXX11)
        message(STATUS "C++11 is supported.")
        if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        endif()
    elseif(COMPILER_SUPPORTS_CXX0X)
        message(STATUS "C++0x is supported.")
        if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -stdlib=libc++")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
        endif()
    else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
    endif()
endif()

include_directories(include)
add_subdirectory(opt/crest)

set(REIMBURSEMENTS_SOURCE_LIB
        ${REIMBURSEMENTS_SD}/main.cpp
        ${REIMBURSEMENTS_SD}/Price.cpp
        ${REIMBURSEMENTS_SD}/Db.cpp

        ${REIMBURSEMENTS_ID}/reimbursements/Price.h
        ${REIMBURSEMENTS_ID}/reimbursements/Db.h
        )

add_executable(Reimbursements ${REIMBURSEMENTS_SOURCE_LIB})
target_link_libraries(Reimbursements libmysqlcppconn.so)
target_link_libraries(Reimbursements crest)
target_link_libraries(Reimbursements ${CREST_LIBRARIES})
include_directories(${CREST_ID} ${INI_ID})
