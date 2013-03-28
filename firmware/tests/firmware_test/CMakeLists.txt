PROJECT(FirmwareTest)

cmake_minimum_required(VERSION 2.6)

# Use relative paths
# This is mostly to reduce path size for command-line limits on windows
if(WIN32)
  # This seems to break Xcode projects so definitely don't enable on Apple builds
  set(CMAKE_USE_RELATIVE_PATHS true)
  set(CMAKE_SUPPRESS_REGENERATION true)
endif()

SET ( HEADER_FILES
)

SET ( SOURCE_FILES
	firmware_tests.cpp
	../../dali_encode.c
	../../Interpreter.c
)

add_definitions( -DTEST -Dgtest_build_tests=ON	)

add_executable(test ${HEADER_FILES} ${SOURCE_FILES} )

TARGET_LINK_LIBRARIES( test gtest pthread)
