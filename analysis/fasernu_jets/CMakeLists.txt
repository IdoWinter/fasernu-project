# CMakeLists.txt for the "event" package. It creates a library and a main program.
# If ROOT is not installed in a default system location you need to tell CMake where to find it.
# Sourcing `thisroot.sh` already sets the required environment variables.
# Otherwise, you must tell the build system where to look for ROOT,
# for example by passing `-DROOT_DIR="/path/to/root/installation` at CMake configuration time.

   cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
   project(FaserNu_Jets)

# fastjet part #########
# Find FastJet using fastjet-config
#find_package(FastJet REQUIRED)

# If FastJet does not have a built-in find_package support, use this workaround
execute_process(COMMAND fastjet-config --cxxflags
                OUTPUT_VARIABLE FASTJET_CXXFLAGS
                OUTPUT_STRIP_TRAILING_WHITESPACE)

string(REPLACE "-I" "" FASTJET_INCLUDE_DIRS "${FASTJET_CXXFLAGS}")
separate_arguments(FASTJET_INCLUDE_DIRS)

execute_process(COMMAND fastjet-config --libs
                OUTPUT_VARIABLE FASTJET_LIBS
                OUTPUT_STRIP_TRAILING_WHITESPACE)


# Locate the ROOT package and define a number of useful targets and variables.
   find_package(ROOT REQUIRED RIO Graf Hist Net Core)
   # REQUIRED COMPONENTS RIO Net)
   include_directories(${CMAKE_CURRENT_SOURCE_DIR}/FaserNu_Jets)
   include_directories(${CMAKE_CURRENT_SOURCE_DIR})
   include_directories(${FASTJET_INCLUDE_DIRS})
# Create a shared library.
# Passing cmake targets such as `ROOT::RIO` as dependencies (rather than plain
# library names for example via ${ROOT_LIBRARIES}) ensures that properties such as required
# include directories and C++ standard are propagated to our libraries or executables.
# Note: To ensure compatibility with Cling, targets *must* be compiled using the
# same C++ standard as ROOT was compiled with.
   file(GLOB SOURCES "src/*.cxx")
   add_library(GenieEvent SHARED ${SOURCES})
   target_link_libraries(GenieEvent PUBLIC ROOT::Core ROOT::RIO ROOT::Net ROOT::Hist ROOT::Tree ROOT::Graf ROOT::Graf3d ROOT::Gpad ROOT::Physics)   

   set(CMAKE_BUILD_TYPE Debug)
# Create the main program using the library.
   add_executable(AnaExample_Event app/AnaExample_Event.cxx)
   target_link_libraries(AnaExample_Event GenieEvent ${FASTJET_LIBS})
