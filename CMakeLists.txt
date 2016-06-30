#
# To build out-of-source do (example):
#
#	mkdir build
#	cd build
#	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
#
# CMAKE_BUILD_TYPE can be: empty, Debug, Release, RelWithDebInfo or MinSizeRel
#

# Make sure the user doesn't play dirty with symlinks
get_filename_component (srcdir "${CMAKE_SOURCE_DIR}" REALPATH)
get_filename_component (bindir "${CMAKE_BINARY_DIR}" REALPATH)

# Disallow in-source builds
if (${srcdir} STREQUAL ${bindir})
	message(FATAL_ERROR "In-source builds are not allowed. "
	"Please create a directory and run cmake from there, passing the path "
	"to this source directory as the last argument. This process created "
	"the file `CMakeCache.txt' and the directory `CMakeFiles' in ${srcdir}. "
	"Please remove them.")
endif (${srcdir} STREQUAL ${bindir})

# Define minimum CMake version required
cmake_minimum_required(VERSION 2.8.5 FATAL_ERROR)

# Use NEW behavior with newer CMake releases
foreach (p
		CMP0025 # CMake 3.0: Compiler id for Apple Clang is now AppleClang
		)
	if (POLICY ${p})
		cmake_policy(SET ${p} NEW)
	endif()
endforeach()

# Use OLD behavior with newer CMake releases
foreach (p
		CMP0026 # CMake 3.0: Disallow use of the LOCATION target property
		)
	if (POLICY ${p})
		cmake_policy(SET ${p} OLD)
	endif()
endforeach()

# Define project name and language
project (USBPP CXX)

# The project package name.
set (PROJECT_NAME "libusbpp")

# a short description of the project (only a few words).
set (PROJECT_DESCRIPTION "C++ wrapper API of C API for USB device access (libusb-1.0)")

# The project package version.
set (PROJECT_VERSION_MAJOR 1)
set (PROJECT_VERSION_MINOR 0)
set (PROJECT_VERSION_PATCH 0)

# The project package version.
set (PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_MINOR}")

# The package configuration version.
set (PACKAGE_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")

# The API version (SOVERSION) of the USBPP libraries.
# Increment this when more or fewer functions are included in the
# library, the function prototype changes, or data type changes.
#
# The SOVERSION is not necessarily the same as the major version.
# The rule is that any breakage of the ABI must be indicated by
# incrementing the SOVERSION. So, adding e.g. functions is no
# problem, modifying argument lists or removing functions would
# required the SOVERSION to be incremented. Similar rules hold of
# course for non-opaque data-structures. For C++ the whole issue
# is infinitely more complicated, as determining whether the ABI
# was changed in an incompatible way is AFAIK nearly impossible.
#
# Refer to:
# cmake --help-command set_target_properties
# cmake --help-property SOVERSION
# https://community.kde.org/Policies/Binary_Compatibility_Issues_With_C++
#
set (USBPP_LIB_SOVERSION 0)

# The build version (VERSION) of the USBPP libraries.
set (USBPP_LIB_VERSION "${USBPP_LIB_SOVERSION}.${PACKAGE_VERSION}")

# The build name (OUTPUT_NAME) of the USBPP libraries.
set (USBPP_LIB_NAME "usbpp-${PACKAGE_VERSION}")

# Make it possible to locate CMake modules for finding libraries
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Check for C++14 or at least C++11 support
include (CheckCXXCompilerFlag)
check_cxx_compiler_flag ("-std=c++14" COMPILER_SUPPORTS_CXX14)
if (COMPILER_SUPPORTS_CXX14)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
else (COMPILER_SUPPORTS_CXX14)
    message (WARNING "The compiler ${CMAKE_CXX_COMPILER} "
                     "has no C++14 support. Try to fall back "
                     "to older C++11 compatibility level.")
    check_cxx_compiler_flag ("-std=c++11" COMPILER_SUPPORTS_CXX11)
    check_cxx_compiler_flag ("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
    if (COMPILER_SUPPORTS_CXX11)
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    elseif (COMPILER_SUPPORTS_CXX0X)
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    else (COMPILER_SUPPORTS_CXX0X)
        message (FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} "
                             "has no C++11 support. Please use a "
                             "different C++ compiler.")
    endif (COMPILER_SUPPORTS_CXX11)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include backport_std_unique_ptr.hpp")
endif (COMPILER_SUPPORTS_CXX14)

# Find libraries
message (STATUS "Searching dependent libraries. This may take a few minutes...")

# Require libusb-1.0
find_package (LibUSB-1.0 REQUIRED)
if (LIBUSB1_FOUND)
    include_directories (${LIBUSB1_INCLUDE_DIR})
else (LIBUSB1_FOUND)
    message (FATAL_ERROR "LibUSB-1.0 was not found.")
endif (LIBUSB1_FOUND)

# Setup CMake to run tests
include (CTest)
if (BUILD_TESTING)
    # Setup CMake to provide a customized CTest -- see:
    # https://cmake.org/Wiki/CMake/Testing_With_CTest#Customizing_CTest
    enable_testing ()
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/CTestCustom.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/CTestCustom.cmake"
        IMMEDIATE @ONLY)
    # Emulate GNU Autotools 'make check'
    add_custom_target (check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)
    # Combine the fancy 'make ARGS=--verbose all test'
    add_custom_target (build_and_test COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
    add_subdirectory (test)
endif (BUILD_TESTING)

# Setup CMake to provide an uninstall target -- see:
# https://cmake.org/Wiki/CMake_FAQ#Can_I_do_.22make_uninstall.22_with_CMake.3F
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

# Exporting dependencies towards external packages
set (PKG_CONFIG_REQUIRES libusb-1.0)
set (PKG_CONFIG_LIBDIR
    "\${prefix}/lib"
)
set (PKG_CONFIG_INCLUDEDIR
    "\${prefix}/include/${PROJECT_NAME}-${PACKAGE_VERSION}"
)
set (PKG_CONFIG_LIBS
    "-L\${libdir} -l${USBPP_LIB_NAME}"
)
set (PKG_CONFIG_CFLAGS
    "-I\${includedir}"
)

configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/pkg-config.pc.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-${PACKAGE_VERSION}.pc
)

# Install package configuration file
install (FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-${PACKAGE_VERSION}.pc
         DESTINATION lib/pkgconfig)

# Preappend the local include search path
include_directories (${CMAKE_SOURCE_DIR}/headers)

## Add subdirectories
add_subdirectory (headers)
add_subdirectory (src)
add_subdirectory (examples)
