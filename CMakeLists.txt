cmake_minimum_required(VERSION 3.27)

include(cmake/CPM.cmake)

CPMAddPackage("gh:fazers/cmake#1.0.0")
set(CMAKE_MODULE_PATH "${cmake_SOURCE_DIR};")

include(version)

project(llm-chat
  VERSION ${VERSION_SHORT}
  DESCRIPTION "LLM-Chat"
  HOMEPAGE_URL "https://naurislinde.dev"
  LANGUAGES CXX C)

include(cmake/Options.cmake)
include(cmake/CompilationFlags.cmake)
include(system-link)

if(ENABLE_IPO)
  include(interprocedural-optimization)
  enable_ipo()
endif()

include(cmake/Dependencies.cmake)
setup_dependencies()

add_library(project_warnings INTERFACE)
add_library(project_options INTERFACE)

include(compiler-warnings)
set_project_warnings(project_warnings ${WARNINGS_AS_ERRORS} "" "" "")

if(ENABLE_USER_LINKER)
  include(linker)
  configure_linker(project_options)
endif()

include(sanitizers)
enable_sanitizers(project_options
  ${ENABLE_SANITIZER_ADDRESS}
  ${ENABLE_SANITIZER_LEAK}
  ${ENABLE_SANITIZER_UNDEFINED}
  ${ENABLE_SANITIZER_THREAD}
  ${ENABLE_SANITIZER_MEMORY})

if(ENABLE_HARDENING)
  include(hardening)

  if(NOT SUPPORTS_UBSAN
    OR ENABLE_SANITIZER_UNDEFINED
    OR ENABLE_SANITIZER_ADDRESS
    OR ENABLE_SANITIZER_THREAD
    OR ENABLE_SANITIZER_LEAK)
    set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
  else()
    set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
  endif()

  message("${ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${ENABLE_SANITIZER_UNDEFINED}")
  enable_hardening(project_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
endif()

if(ENABLE_PCH)
  target_precompile_headers(
    project_options
    INTERFACE
    <vector>
    <string>
    <utility>)
endif()

if(ENABLE_CACHE)
  include(cache)
  enable_cache()
endif()

if(ENABLE_COVERAGE)
  include(code-coverage)
  enable_coverage(project_options)
endif()

target_compile_features(project_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

add_library(libs::project_options ALIAS project_options)
add_library(libs::project_warnings ALIAS project_warnings)

add_subdirectory(src)

include(cmake/CPack.cmake)

if(COMPILE_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if(ENABLE_DOCS)
  include(doxygen)
  enable_doxygen("")
endif()

message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Release cxx: ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "Debug cxx: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS " ")

if(BUILD_TYPE STREQUAL "release")
  message(STATUS "Complete cxx: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
  message(STATUS "Complete ld: ${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
else()
  message(STATUS "Complete cxx: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
  message(STATUS "Complete ld: ${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
endif()
