cmake_minimum_required(VERSION 2.8.7)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

project(reimbursements)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

set(REIMBURSEMENTS_ID ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE INTERNAL "")
set(REIMBURSEMENTS_SD ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(include)
add_subdirectory(opt/crest)

set(REIMBURSEMENTS_SOURCE_LIB
        ${REIMBURSEMENTS_SD}/main.cpp
        ${REIMBURSEMENTS_SD}/price.cpp
        ${REIMBURSEMENTS_SD}/db.cpp

        ${REIMBURSEMENTS_ID}/reimbursements/price.h
        ${REIMBURSEMENTS_ID}/reimbursements/db.h
        )

add_executable(Reimbursements ${REIMBURSEMENTS_SOURCE_LIB})
target_link_libraries(Reimbursements libmysqlcppconn.so)
target_link_libraries(Reimbursements crest)
target_link_libraries(Reimbursements ${CREST_LIBRARIES})
include_directories(${CREST_ID})