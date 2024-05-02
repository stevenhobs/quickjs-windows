# Project information
cmake_minimum_required(VERSION 3.6.0)
project("QUICKJS_MINGW_BUILD")
file(STRINGS "./quickjs/VERSION" QJS_VERSION)

##### * Configuration You could modify * #####
set(CONFIG_SHARED  ON)                  # Build dynamic shared library
set(CONFIG_CLANG   OFF)                 # Use LLVM-Clang instead of GNU-C
set(CONFIG_LTO     OFF)                 # LTO Optimision
set(CONFIG_WERROR  OFF)                 # Warning is Error
set(CONFIG_TARGET_PLATFORM "x86_64")    # Options: x86_64 i686 aarch64 armv7
##### * Configuration You could modify * #####

# set build output directory
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Build type: Debug.")
    set(CMAKE_BINARY_DIR "debug/${CONFIG_TARGET_PLATFORM}")
else()
    message(STATUS "Current build type is Realse.")
    set(CMAKE_BINARY_DIR "release/${CONFIG_TARGET_PLATFORM}")
endif()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# compiler configuration
if(CMAKE_HOST_WIN32 AND ${MINGW})
if(CONFIG_CLANG)
    set(CC clang)
else()
    set(CC gcc)
endif()
else()
    message(FATAL_ERROR "**Windows System & MinGW Toolchain is required!**")
endif()
set(CC "${CONFIG_TARGET_PLATFORM}-w64-mingw32-${CC}")
set(CMAKE_C_STANDARD 17)
message(STATUS Toolchain: ${CC})

# c flags
set(CMAKE_C_FLAGS "-fPIC -fwrapv -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-array-bounds -Wno-format-truncation")
if(CONFIG_WERROR)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
endif()
if(CONFIG_LTO)
    set(QJSC_LTO "-DCONFIG_LTO")
    set(CMAKE_C_FLAGS "-flto ${CMAKE_C_FLAGS}")
endif()

# Definions
add_definitions(-DCONFIG_VERSION="${QJS_VERSION}" -D__USE_MINGW_ANSI_STDIO -D_GNU_SOURCE -DCONFIG_BIGNUM)

# include dirs
include_directories("./quickjs" "./patch")

# Link options
add_link_options("-Wl,--no-undefined")
if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_link_options("-s") # Use strip if not in debug
endif()

# build libdl
message(STATUS "Please ignore the Warning from dlfcn-win32/CMakeLists.txt")
set(BUILD_SHARED_LIBS OFF)
add_subdirectory("./dlfcn-win32")
message(STATUS "libdl build: OK")

# quickjs-libc.c patch
set(QJS_LIBC_FILE "${CMAKE_SOURCE_DIR}/quickjs/quickjs-libc.c")
set(QJS_LIBC_FILE_PATCHED "${CMAKE_CACHEFILE_DIR}/gen/quickjs-libc.c")
set(SED_CMD ${CMAKE_SOURCE_DIR}/patch/sed)
add_custom_command(
    OUTPUT ${QJS_LIBC_FILE_PATCHED}
    COMMAND ${SED_CMD} "464a\\#include <quickjs-libc_patch.h> //patched insert line" ${QJS_LIBC_FILE} > ${QJS_LIBC_FILE_PATCHED}
    COMMAND ${SED_CMD} -i "522a\\#include <quickjs-libc_patch.h> //patched insert line" ${QJS_LIBC_FILE_PATCHED}
    DEPENDS ${QJS_LIBC_FILE}
    COMMENT "Patched file quickjs/quickjs-libc.c"
    VERBATIM
)

#quickjs lib build
if(CONFIG_SHARED)
    set(QJS_LIB_TYPE SHARED)
else()
    set(QJS_LIB_TYPE STATIC)
endif()
add_library(quickjs ${QJS_LIB_TYPE}
    "quickjs/cutils.c"
    "quickjs/libbf.c"
    "quickjs/libregexp.c"
    "quickjs/libunicode.c"
    "quickjs/quickjs.c"
    ${QJS_LIBC_FILE_PATCHED}
)
target_link_libraries(quickjs PUBLIC m pthread dl)

# qjsc bin build
add_compile_definitions(qjsc ${QJSC_LTO} CONFIG_CC=${CC})
add_executable(qjsc "quickjs/qjsc.c")
target_link_libraries(qjsc PUBLIC quickjs)

# prebuild repl.c & qjscalc.c
set(REPL_JS_FILE "${CMAKE_SOURCE_DIR}/quickjs/repl.js")
set(REPL_C_FILE "${CMAKE_CACHEFILE_DIR}/gen/repl.c")  # This file requires qjsc to generate
set(CALC_JS_FILE "${CMAKE_SOURCE_DIR}/quickjs/qjscalc.js")
set(CALC_C_FILE "${CMAKE_CACHEFILE_DIR}/gen/qjscalc.c")
add_custom_command(
    OUTPUT ${REPL_C_FILE}
    COMMAND qjsc -c -o ${REPL_C_FILE} -m ${REPL_JS_FILE}
    COMMENT "Generate repl.c file for the qjs binary"
    VERBATIM
)
add_custom_command(
    OUTPUT ${CALC_C_FILE}
    COMMAND qjsc -fbignum -c -o ${CALC_C_FILE} ${CALC_JS_FILE}
    COMMENT "Generate qjcalc.c file for the qjs binary"
    VERBATIM
)
# qjs bin build
add_executable(qjs "quickjs/qjs.c" ${REPL_C_FILE} ${CALC_C_FILE})
target_link_libraries(qjs PUBLIC quickjs)

# cmake install
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)
install(TARGETS quickjs qjs qjsc
        DESTINATION ${CMAKE_INSTALL_PREFIX})