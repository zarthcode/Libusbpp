#
# headers/CMakeLists.txt
#

# Preappend the local include search path
include_directories (${CMAKE_SOURCE_DIR})

# Collect all library header files
set (libheader
    libusbpp.hpp
    libusbpp/Configuration.hpp
    libusbpp/Device.hpp
    libusbpp/EndpointDefs.hpp
    libusbpp/Endpoint.hpp
    libusbpp/Exception.hpp
    libusbpp/Interface.hpp
    libusbpp/TransferDefs.hpp
    libusbpp/Transfer.hpp
)

# Check if all headers were catched
message (STATUS "Searching for uncatched library header...")
file (GLOB_RECURSE allheader RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    *.h
    *.inl
    *.hpp
)
foreach (_header ${allheader})
    list (FIND libheader ${_header} matchidx)
    if (${matchidx} EQUAL -1)
        message (AUTHOR_WARNING "  ${_header}")
    endif ()
endforeach ()

# Install collected header files
foreach (file ${libheader})
    get_filename_component( dir ${file} DIRECTORY )
    install (FILES ${file} DESTINATION include/${PROJECT_NAME}-${PACKAGE_VERSION}/${dir})
endforeach ()

