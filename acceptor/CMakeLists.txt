CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

ADD_DEFINITIONS(-DVERSION="1.00")
SET(PRJ acceptor)
PROJECT(${PRJ})

OPTION(DEBUG "if Debug." OFF)

SET(CMAKE_C_FLAGS_DEBUG "-Wall")
SET(CMAKE_C_FLAGS "-Wall")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if (UNIX AND NOT APPLE)                                                          
    SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D__LINUX__")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__LINUX__")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__LINUX__")
else()
    execute_process(
        COMMAND brew --prefix libevent
        OUTPUT_VARIABLE LIBEVENT_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    INCLUDE_DIRECTORIES("${LIBEVENT_PREFIX}/include")
    INCLUDE_DIRECTORIES("/usr/local/include/")
    LINK_DIRECTORIES("/usr/local/lib")
    LINK_DIRECTORIES("${LIBEVENT_PREFIX}/lib")
endif()

INCLUDE_DIRECTORIES("../natlibs/ext/")
LINK_DIRECTORIES("../natlibs/ext/")

IF (NOT DEBUG)
    SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O2 ")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 ")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 ")
ELSE()
    SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g ")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g ")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g ")
ENDIF()


LINK_DIRECTORIES("/usr/local/lib/")
INCLUDE_DIRECTORIES("/usr/local/include/")
INCLUDE_DIRECTORIES("./")
### c source.
SET(SOURCE_FILES
        main.cc
        acceptor_setup_tcpsrv.cc
        acceptor_setup_udssrv.cc
        acceptor_handler_tcpsrv.cc
        acceptor_handler_udssrv.cc
        acceptor_loop.cc
        acceptor_timeout_tcpsrv.cc
        acceptor_timeout_udssrv.cc
        acceptor_sendfd.cc
        acceptor_recieve_req.cc
        acceptor_initialize.cc
    )
ADD_EXECUTABLE(${PRJ} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(${PRJ} pthread)
TARGET_LINK_LIBRARIES(${PRJ} event)
TARGET_LINK_LIBRARIES(${PRJ} event_pthreads)
TARGET_LINK_LIBRARIES(${PRJ} natipc)


if (UNIX AND NOT APPLE)
    TARGET_LINK_LIBRARIES(${PRJ} rt)
else()
    TARGET_LINK_LIBRARIES(${PRJ} ncurses)
endif()
