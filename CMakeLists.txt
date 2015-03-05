cmake_minimum_required (VERSION 2.6)

PROJECT (INTELL_BENCHMARK)

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Custom setting declaration
SET(MY_COMPILER_OPTION "GCC" CACHE STRING "Use compiler")
SET(MY_PARALLEL_LIB "OMP" CACHE STRING "Parallel library to use")
SET(MY_VERBOSE_VECTORIZATION ON CACHE BOOL "Vectorization report")
SET(MY_WITH_SIMD OFF CACHE BOOL "Use SIMD hint instead ivdep")
SET(BUILD_DOCUMENTATION ON CACHE BOOL "Build DOC")

# Common compiler settings
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpmath=sse -march=native --std=c++11")

IF(MY_WITH_SIMD)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WITH_SIMD")
ENDIF()

# Enable Inell compiler
IF (MY_COMPILER_OPTION STREQUAL "Intel")
    MESSAGE(STATUS "** Compiling with Intel settings **")
    SET(CMAKE_CXX_COMPILER "/opt/intel/composer_xe_2015/bin/icpc")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ipo")
    IF(MY_VERBOSE_VECTORIZATION)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -vec_report3")
    ENDIF()
    SET(CMAKE_LINKER "/opt/intel/composer_xe_2015/bin/xild")
# Enable gcc
ELSE()
    MESSAGE(STATUS "** Compiling with GCC settings **")
    SET(CMAKE_CXX_COMPILER "/usr/bin/g++")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftree-vectorize -ffast-math -O3 -msse2")
    IF(MY_VERBOSE_VECTORIZATION)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftree-vectorizer-verbose=5")
    ENDIF()
ENDIF ()

IF(MY_PARALLEL_LIB STREQUAL "OMP")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WITH_OMP -fopenmp")
    SET(Boost_USE_STATIC_LIBS ON)
    SET(Boost_USE_MULTITHREADED OFF)
    SET(Boost_USE_STATIC_RUNTIME ON)

    FIND_PACKAGE(
        Boost 1.49 REQUIRED
        COMPONENTS
        program_options
        date_time)

    ADD_EXECUTABLE(
            IntellBenchmark
            ${PROJECT_SOURCE_DIR}/IntelBenchmark/IntelBenchmark.cpp
            ${PROJECT_SOURCE_DIR}/IntelBenchmark/external/json_value.cpp
            ${PROJECT_SOURCE_DIR}/IntelBenchmark/external/json_writer.cpp
            ${PROJECT_SOURCE_DIR}/IntelBenchmark/external/json_reader.cpp)

    TARGET_LINK_LIBRARIES (
            IntellBenchmark
            ${Boost_LIBRARIES})

    LINK_DIRECTORIES ("${PROJECT_BINARY_DIR}")

    INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/IntelBenchmark/external)

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
                program_options
                date_time)

            ADD_EXECUTABLE(
                IntellBenchmark
                ${PROJECT_SOURCE_DIR}/IntelBenchmark/IntelBenchmark.cpp
                ${PROJECT_SOURCE_DIR}/IntelBenchmark/external/json_value.cpp
                ${PROJECT_SOURCE_DIR}/IntelBenchmark/external/json_writer.cpp
                ${PROJECT_SOURCE_DIR}/IntelBenchmark/external/json_reader.cpp)

            TARGET_LINK_LIBRARIES (
                IntellBenchmark
                ${Boost_LIBRARIES}
                ${TBB_LIBRARIES})

            LINK_DIRECTORIES (
                "${PROJECT_BINARY_DIR}"
                "${TBB_LIBRARY_DIRS}")

            INCLUDE_DIRECTORIES(
                ${TBB_INCLUDE_DIRS}
		${PROJECT_SOURCE_DIR}/IntelBenchmark/external)
        ENDIF()
    ENDIF()
ENDIF()

#DOC
MACRO(CONFIGURE_DOXYGEN_FILE DOXYGEN_CONFIG_FILE FILE_NAME_SUFFIX)
    IF(EXISTS ${PROJECT_SOURCE_DIR}/${DOXYGEN_CONFIG_FILE})
	FILE(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/doxy-${FILE_NAME_SUFFIX}.conf)
        FILE(READ ${PROJECT_SOURCE_DIR}/${DOXYGEN_CONFIG_FILE} DOXYFILE_CONTENTS)
        STRING(REGEX REPLACE ";" "\\\\;" DOXYFILE_CONTENTS "${DOXYFILE_CONTENTS}")
        STRING(REGEX REPLACE "\n" ";" DOXYFILE_LINES "${DOXYFILE_CONTENTS}")
        LIST(LENGTH DOXYFILE_LINES ROW)
        MATH(EXPR ROW "${ROW} - 1")
        FOREACH(I RANGE ${ROW})
    	    LIST(GET DOXYFILE_LINES ${I} LINE)
            IF(LINE STRGREATER "")
        	STRING(REGEX MATCH "^[a-zA-Z]([^ ])+" DOXY_PARAM ${LINE})
                IF(DEFINED DOXY_${DOXY_PARAM})
            	    STRING(REGEX REPLACE "=([^\n])+" "= ${DOXY_${DOXY_PARAM}}" LINE ${LINE})
		ENDIF(DEFINED DOXY_${DOXY_PARAM})
	    ENDIF()
            FILE(APPEND ${CMAKE_CURRENT_BINARY_DIR}/doxy-${FILE_NAME_SUFFIX}.conf "${LINE}\n")
            ENDFOREACH()
    ELSE()
	MESSAGE(SEND_ERROR "Doxygen configuration file '${DOXYGEN_CONFIG_FILE}' not found. Documentation will not be generated")
    ENDIF()
ENDMACRO(CONFIGURE_DOXYGEN_FILE)

MACRO(ADD_DOCUMENTATION TARGET DOXYGEN_CONFIG_FILE)
    FIND_PACKAGE(Doxygen)
    IF(DOXYGEN_FOUND)
	CONFIGURE_DOXYGEN_FILE(${DOXYGEN_CONFIG_FILE} ${TARGET})
        ADD_CUSTOM_TARGET(${TARGET} COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxy-${TARGET}.conf)
    ELSE(DOXYGEN_FOUND)
	MESSAGE(STATUS "Doxygen not found. Documentation will not be generated")
    ENDIF(DOXYGEN_FOUND)
ENDMACRO(ADD_DOCUMENTATION)



IF(BUILD_DOCUMENTATION)
    FIND_PACKAGE(Sphinx REQUIRED)

    IF(NOT DEFINED SPHINX_THEME)
	SET(SPHINX_THEME default)
    ENDIF()

    IF(NOT DEFINED SPHINX_THEME_DIR)
        SET(SPHINX_THEME_DIR)
    ENDIF()

    # configured documentation tools and intermediate build results
    SET(BINARY_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_build")

    # Sphinx cache with pickled ReST documents
    SET(SPHINX_CACHE_DIR "${CMAKE_CURRENT_BINARY_DIR}/_doctrees")

    # HTML output directory
    SET(SPHINX_HTML_DIR "${CMAKE_CURRENT_BINARY_DIR}/doc/html")

    CONFIGURE_FILE(
	"${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in"
        "${BINARY_BUILD_DIR}/conf.py"
	@ONLY)

    
    SET(DOXY_OUTPUT_LANGUAGE "English")
    SET(DOXY_INPUT ${PROJECT_SOURCE_DIR})
    ADD_DOCUMENTATION(xml Doxyfile)

    ADD_CUSTOM_TARGET(benchmark_docs ALL
	${SPHINX_EXECUTABLE}
    	    -q -b html
    	    -c "${BINARY_BUILD_DIR}"
    	    -d "${SPHINX_CACHE_DIR}"
    	    "${CMAKE_CURRENT_SOURCE_DIR}"
            "${SPHINX_HTML_DIR}"
	COMMENT "Building HTML documentation with Sphinx")
    
    ADD_DEPENDENCIES(benchmark_docs xml)

ENDIF()

#install
install(TARGETS IntellBenchmark
    DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME}/${MY_COMPILER_OPTION}/${MY_PARALLEL_LIB}"
    )


