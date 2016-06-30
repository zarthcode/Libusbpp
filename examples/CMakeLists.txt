#
# LibusbTest/CMakeLists.txt
#

# Collect PIC example source files
set (picappsrc
    LibusbTest.cpp
)

# Join and sort all known example source files
set (appsrc ${picappsrc})
list (SORT appsrc)

# Check if all sources were catched
message (STATUS "Searching for uncatched example sources...")
file (GLOB allsrc RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    *.cpp
)
foreach (_src ${allsrc})
    list (FIND appsrc ${_src} matchidx)
    if (${matchidx} EQUAL -1)
        message (AUTHOR_WARNING "  ${_src}")
    endif ()
endforeach ()

add_executable (${PROJECT_NAME}-pic-example ${picappsrc})
target_link_libraries (${PROJECT_NAME}-pic-example sharedlib
                                                   ${LIBUSB1_LIBRARIES})

# Install executable
install (TARGETS ${PROJECT_NAME}-pic-example
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)
