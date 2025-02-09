# CMakeLists.txt for the "event" package. It creates a library and a main program.
# If ROOT is not installed in a default system location you need to tell CMake where to find it.
# Sourcing `thisroot.sh` already sets the required environment variables.
# Otherwise, you must tell the build system where to look for ROOT,
# for example by passing `-DROOT_DIR="/path/to/root/installation` at CMake configuration time.

   cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
   project(FaserNu_vertices)

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


if(DEFINED ENV{FASTJET_CONTRIB_DIR})
      # Use the environment variable for include and library paths
      set(FASTJET_CONTRIB_DIR $ENV{FASTJET_CONTRIB_DIR})
else()
      message(FATAL_ERROR "Environment variable FJCONTRIB_PATH is not set. Please set it to the path of FastJet contrib.")
endif()

if(DEFINED ENV{ACTS_DIR})
      # Use the environment variable for include and library paths
      set(ACTS_DIR $ENV{ACTS_DIR})
else()
      message(FATAL_ERROR "Environment variable ACTS_DIR is not set. Please set it to the path of Acts.")
endif()

# Locate the ROOT package and define a number of useful targets and variables.
   find_package(ROOT REQUIRED RIO Graf Hist Net Core)
   find_package(CGAL REQUIRED COMPONENTS Qt5)
   find_package(Qt5 REQUIRED COMPONENTS Widgets)
   find_package(Eigen3 REQUIRED NO_MODULE)
   
   # REQUIRED COMPONENTS RIO Net)
   include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
   include_directories(${CMAKE_CURRENT_SOURCE_DIR})
   include_directories(${FASTJET_INCLUDE_DIRS})
   include_directories(${FASTJET_CONTRIB_DIR}/include)
   include_directories(${EIGEN3_INCLUDE_DIRS})
   include_directories(${ACTS_DIR}/include)
   link_directories(${FASTJET_CONTRIB_DIR}/lib)
   link_directories(${ACTS_DIR}/lib64)
# Create a shared library.
# Passing cmake targets such as `ROOT::RIO` as dependencies (rather than plain
# library names for example via ${ROOT_LIBRARIES}) ensures that properties such as required
# include directories and C++ standard are propagated to our libraries or executables.
# Note: To ensure compatibility with Cling, targets *must* be compiled using the
# same C++ standard as ROOT was compiled with.
   # glob all the files with the .cxx extension in the src directory and its subdirectories
   file(GLOB_RECURSE SOURCES src/*.cxx)
   # log the sources
   message(STATUS "Sources: ${SOURCES}")
   add_library(VerticesLib SHARED ${SOURCES})
   target_link_libraries(VerticesLib PUBLIC ROOT::Core ROOT::RIO ROOT::Net ROOT::Hist ROOT::Tree ROOT::Graf ROOT::Graf3d 
   ROOT::Gpad ROOT::Physics ROOT::TreePlayer EnergyCorrelator CGAL::CGAL CGAL::CGAL_Basic_viewer)   

   set(CMAKE_BUILD_TYPE Debug)
# Create the main program using the library.
   add_executable(Vertices src/main.cxx)
   target_link_libraries(Vertices VerticesLib ${FASTJET_LIBS})
