CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
MESSAGE(STATUS "natlib test")
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -g -O0")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fPIC")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC")

LINK_DIRECTORIES(/usr/local/lib)
INCLUDE_DIRECTORIES(
        "../"
        "."
)
SET(TEST_SOURCES
        ../nat_shared_memory_base.cc
        ../nat_shared_memory_config.cc
        ../nat_shared_memory_statistics.cc
        )

IF(UNIX AND NOT APPLE)
     SET(TARGET_LIBS "pthread" "rt")
ELSE()
     SET(TARGET_LIBS "pthread")
ENDIF()


### --------- test basic -----
SET(SOURCE_FILES test_basic.cc)

ADD_EXECUTABLE(test_basic
        ${SOURCE_FILES}
        ${TEST_SOURCES}
        )
TARGET_LINK_LIBRARIES(test_basic
        ${TARGET_LIBS}
        gtest
        gtest_main
        )
