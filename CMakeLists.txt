cmake_minimum_required (VERSION 2.6)

PROJECT (INTELL_BENCHMARK)

SET(MY_COMPILER_OPTION "GCC" CACHE STRING "Use compiler")
SET(MY_PARALLEL_LIB "OMP" CACHE STRING "Parallel library to use")


IF (MY_COMPILER_OPTION STREQUAL "Intel")
    MESSAGE(STATUS "** Compiling with Intel settings **")
    SET(CMAKE_CXX_COMPILER "/opt/intel/composer_xe_2015/bin/icpc")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpmath=sse -march=native -ipo -vec_report3")
    SET(CMAKE_LINKER "/opt/intel/composer_xe_2015/bin/xild")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} --std=c++11")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --std=c++11")
ELSE()
    MESSAGE(STATUS "** Compiling with GCC settings **")
    SET(CMAKE_CXX_COMPILER "/usr/bin/g++")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpmath=sse -march=native -ftree-vectorize -ftree-vectorizer-verbose=3")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} --std=c++11")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --std=c++11")
ENDIF ()

IF(MY_PARALLEL_LIB STREQUAL "OMP")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WITH_OMP -fopenmp")
    SET(Boost_USE_STATIC_LIBS ON)
    SET(Boost_USE_MULTITHREADED OFF)
    SET(Boost_USE_STATIC_RUNTIME ON)

    FIND_PACKAGE(
        Boost 1.49 REQUIRED
        COMPONENTS
        program_options)

    ADD_EXECUTABLE(
            IntellBenchmark
            ${PROJECT_SOURCE_DIR}/IntelBenchmark/IntelBenchmark.cpp
            ${PROJECT_SOURCE_DIR}/IntelBenchmark/json_value.cpp
            ${PROJECT_SOURCE_DIR}/IntelBenchmark/json_writer.cpp
            ${PROJECT_SOURCE_DIR}/IntelBenchmark/json_reader.cpp)

    TARGET_LINK_LIBRARIES (
            IntellBenchmark
            ${Boost_LIBRARIES})

    LINK_DIRECTORIES ("${PROJECT_BINARY_DIR}")

ELSE()
    IF(MY_PARALLEL_LIB STREQUAL "TBB")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WITH_TBB")
        MESSAGE (STATUS "TBB BUILD")
        SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/FindTBB/")
        FIND_PACKAGE(TBB)

        IF(NOT TBB_FOUND)
            MESSAGE (STATUS "TBB not found. Example project will not be built.")
        ELSE(NOT TBB_FOUND)

            SET(Boost_USE_STATIC_LIBS ON)
            SET(Boost_USE_MULTITHREADED OFF)
            SET(Boost_USE_STATIC_RUNTIME ON)

            FIND_PACKAGE(
                Boost 1.49 REQUIRED
                COMPONENTS
                program_options)

            ADD_EXECUTABLE(
                IntellBenchmark
                ${PROJECT_SOURCE_DIR}/IntelBenchmark/IntelBenchmark.cpp
                ${PROJECT_SOURCE_DIR}/IntelBenchmark/json_value.cpp
                ${PROJECT_SOURCE_DIR}/IntelBenchmark/json_writer.cpp
                ${PROJECT_SOURCE_DIR}/IntelBenchmark/json_reader.cpp)

            TARGET_LINK_LIBRARIES (
                IntellBenchmark
                ${Boost_LIBRARIES}
                ${TBB_LIBRARIES})

            LINK_DIRECTORIES (
                "${PROJECT_BINARY_DIR}"
                "${TBB_LIBRARY_DIRS}")

            INCLUDE_DIRECTORIES(
                ${TBB_INCLUDE_DIRS})
        ENDIF()
    ENDIF()
ENDIF()

install(TARGETS IntellBenchmark
    DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME}/${MY_COMPILER_OPTION}/${MY_PARALLEL_LIB}"
    )


