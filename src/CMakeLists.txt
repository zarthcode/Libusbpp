#
# src/CMakeLists.txt
#

# On Windows do not prefix the DLLs with the 'lib' prefix
if (WIN32)
    set (LIB_PREFIX "")
else (WIN32)
    set (LIB_PREFIX "lib")
endif (WIN32)

# Create position independent code on all targets [auto]
# (needed for static build on non-x86):
set (CMAKE_POSITION_INDEPENDENT_CODE on)

## Preappend the local include search path
#include_directories (${CMAKE_SOURCE_DIR})

## Prep ourselves for compiling boost
#include_directories (${Boost_INCLUDE_DIRS})

# Collect all library source files
set (libsrc
    Configuration.cpp
    ConfigurationImpl.cpp
    ConfigurationImpl.hpp
    Device.cpp
    DeviceImpl.cpp
    DeviceImpl.hpp
    Endpoint.cpp
    EndpointImpl.cpp
    EndpointImpl.hpp
    Exception.cpp
    Interface.cpp
    InterfaceImpl.cpp
    InterfaceImpl.hpp
    LibusbImpl.cpp
    LibusbImpl.hpp
    libusbpp.cpp
    Transfer.cpp
    TransferImpl.cpp
    TransferImpl.hpp
    Wideconvert.cpp
    Wideconvert.hpp
)

# Check if all sources were catched
message (STATUS "Searching for uncatched library sources...")
file (GLOB allsrc RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    *.cpp
)
foreach (_src ${allsrc})
    list (FIND libsrc ${_src} matchidx)
    if (${matchidx} EQUAL -1)
        message (AUTHOR_WARNING "  ${_src}")
    endif ()
endforeach ()

# This is the "object library" target:
# - compiles the sources only once
# - shared libraries need PIC
add_library (objlib OBJECT ${libsrc})
set_property (TARGET ${objlib} PROPERTY POSITION_INDEPENDENT_CODE on)

# Actual shared and static libraries built from the same object files
add_library (sharedlib SHARED $<TARGET_OBJECTS:objlib>)
target_link_libraries(sharedlib ${LIBUSB1_LIBRARIES})
set_target_properties (sharedlib PROPERTIES OUTPUT_NAME "${USBPP_LIB_NAME}"
                                            PREFIX "${LIB_PREFIX}"
                                            IMPORT_PREFIX "${LIB_PREFIX}"
                                            VERSION ${USBPP_LIB_VERSION}
                                            SOVERSION ${USBPP_LIB_SOVERSION})

add_library (staticlib STATIC $<TARGET_OBJECTS:objlib>)
set_target_properties (staticlib PROPERTIES OUTPUT_NAME "${USBPP_LIB_NAME}"
                                            PREFIX "${LIB_PREFIX}"
                                            IMPORT_PREFIX "${LIB_PREFIX}")

# Install shared and static libraries
install (TARGETS sharedlib staticlib
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)
