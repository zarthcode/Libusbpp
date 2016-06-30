#
# test/CMakeLists.txt
#

# Setup CMake to run tests
enable_testing()

# Require Boost
find_package (Boost REQUIRED
                    COMPONENTS system filesystem
                               unit_test_framework)
if (Boost_FOUND)
    message (STATUS "Found Boost in ${Boost_LIBRARY_DIRS}")
endif (Boost_FOUND)

# Preappend the local include search path
include_directories (${CMAKE_SOURCE_DIR})

# Prep ourselves for compiling boost
include_directories (${Boost_INCLUDE_DIRS})
add_definitions (-DBOOST_TEST_DYN_LINK) 

# Collect all test source files
set (testsrc
    main.cpp
)

# Create the test suite
add_executable (testbin ${testsrc})
target_link_libraries (testbin sharedlib
                               ${LIBUSB1_LIBRARIES}
                               ${Boost_SYSTEM_LIBRARY}
                               ${Boost_FILESYSTEM_LIBRARY}
                               ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

# Provide the test suite by ctest
add_dependencies (check testbin)
add_dependencies (build_and_test testbin)
# OFF: add_test (NAME TestSuit
# OFF:           COMMAND testbin -i -l test_suite -t TestSuit)
# OFF: add_test (NAME TestSuit/TestCase
# OFF:           COMMAND testbin -i -l test_suite -t TestSuit/TestCase)
