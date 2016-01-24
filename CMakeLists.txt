cmake_minimum_required (VERSION 2.6)
project("OpenGL Gravity" C)
include_directories("include")

IF (SOIL_INCLUDE_DIR)
    # Already in cache, be silent
    SET(SOIL_FIND_QUIETLY TRUE)
ENDIF (SOIL_INCLUDE_DIR)

FIND_PATH(SOIL_INCLUDE_DIR SOIL.h PATH_SUFFIXES include/SOIL include)

SET(SOIL_NAMES SOIL Soil soil)
FIND_LIBRARY(SOIL_LIBRARY NAMES ${SOIL_NAMES} )
MARK_AS_ADVANCED( SOIL_LIBRARY SOIL_INCLUDE_DIR )

# Per-recommendation
SET(SOIL_INCLUDE_DIRS "${SOIL_INCLUDE_DIR}")
SET(SOIL_LIBRARIES    "${SOIL_LIBRARY}")

# handle the QUIETLY and REQUIRED arguments and set SOIL_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SOIL DEFAULT_MSG SOIL_LIBRARIES
    SOIL_INCLUDE_DIRS)

include_directories(${SOIL_INCLUDE_DIRS})
link_libraries(${SOIL_LIBRARIES})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()


file(GLOB_RECURSE SOURCE_FILES "src/*.c")
add_executable(gravity ${SOURCE_FILES})

IF(UNIX)
    target_link_libraries(gravity m)
ENDIF(UNIX)

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_C_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  else()
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

target_link_libraries(gravity ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
