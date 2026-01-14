CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

SET(PRJ natipc)

SET(SOURCE_FILES
        nat_shared_memory_base.cc
        nat_shared_memory_statistics.cc
        nat_shared_memory_config.cc
    )
PROJECT(${PRJ})

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -g -O0")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fPIC")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC")

OPTION(DEBUG "Debug Mode" OFF)
OPTION(TESTMODE "Test Mode" OFF)

IF(TESTMODE)
    MESSAGE(STATUS "Test Mode ON")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__TEST_MODE__")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__TEST_MODE__")
ELSE()
    MESSAGE(STATUS "Test Mode OFF")
ENDIF()

IF(DEBUG)
    MESSAGE(STATUS "Debug Mode ON")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
ELSE()
    MESSAGE(STATUS "Debug Mode OFF")
ENDIF()

IF(UNIX AND NOT APPLE)
    SET(TARGET_LIBS "pthread" "rt")
ELSE()
    SET(TARGET_LIBS "pthread")
ENDIF()

LINK_DIRECTORIES(
        /usr/local/lib
)
INCLUDE_DIRECTORIES(
        "/usr/local/include"
)

ADD_LIBRARY(${PRJ} SHARED ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(${PRJ}
        ${TARGET_LIBS}
        )

IF(UNIX AND NOT APPLE)
    ADD_CUSTOM_COMMAND(TARGET ${PRJ}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/ext/
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/ext/
            COMMAND ${CMAKE_COMMAND} -E copy ${natipc_BINARY_DIR}/*.so ${CMAKE_BINARY_DIR}/ext/.
            COMMAND ${CMAKE_COMMAND} -E copy ${natipc_SOURCE_DIR}/*.hh ${CMAKE_BINARY_DIR}/ext/.
            COMMAND ${CMAKE_COMMAND} -E copy ${natipc_SOURCE_DIR}/*.cc ${CMAKE_BINARY_DIR}/ext/.
            COMMAND echo -E ${CMAKE_BINARY_DIR}/ext | sudo tee /etc/ld.so.conf.d/natlib.conf
            COMMAND sudo ldconfig
            )
ELSE()
    ADD_CUSTOM_COMMAND(TARGET ${PRJ}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/ext/
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/ext/
            COMMAND ${CMAKE_COMMAND} -E copy ${natipc_BINARY_DIR}/*.dylib ${CMAKE_BINARY_DIR}/ext/.
            COMMAND ${CMAKE_COMMAND} -E copy ${natipc_SOURCE_DIR}/*.hh ${CMAKE_BINARY_DIR}/ext/.
            COMMAND ${CMAKE_COMMAND} -E copy ${natipc_SOURCE_DIR}/*.cc ${CMAKE_BINARY_DIR}/ext/.
            )
ENDIF()


SET(INSTALL_FILES
        nat_libs.hh
        nat_shared_memory_base.hh
        nat_shared_memory_statistics.hh
        nat_shared_memory_config.hh
        )
INSTALL (TARGETS ${PRJ} DESTINATION ext)
INSTALL (FILES ${INSTALL_FILES} DESTINATION ext)

ADD_SUBDIRECTORY(test)
ENABLE_TESTING()
ADD_TEST(NAME test_basic COMMAND test_basic)
