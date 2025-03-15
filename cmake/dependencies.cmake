include(FetchContent)

FetchContent_Declare(
  hard_assert
  GIT_REPOSITORY https://github.com/CesarBerriot/hard_assert.git
  GIT_TAG 1.2.0
)
FetchContent_Declare(
  make_string
  GIT_REPOSITORY https://github.com/CesarBerriot/make_string.git
  GIT_TAG 2.2.0
)
FetchContent_Declare(
  dirent
  GIT_REPOSITORY https://github.com/CesarBerriot/dirent.git
  GIT_TAG v1.24
)
FetchContent_Declare(
  discover_sources
  GIT_REPOSITORY https://github.com/CesarBerriot/discover_sources.git
  GIT_TAG 1.0.0
)
FetchContent_Declare(
  setup_gtest
  GIT_REPOSITORY https://github.com/CesarBerriot/setup_gtest.git
  GIT_TAG 1.2.0
)
FetchContent_Declare(
  pre_main_runner
  GIT_REPOSITORY https://github.com/CesarBerriot/pre_main_runner.git
  GIT_TAG 1.0.0
)
FetchContent_Declare(
  make_string
  GIT_REPOSITORY https://github.com/CesarBerriot/make_string.git
  GIT_TAG 2.2.0
)

FetchContent_MakeAvailable(hard_assert make_string dirent discover_sources setup_gtest pre_main_runner make_string)

include(${discover_sources_SOURCE_DIR}/discover_sources.cmake)
include(${setup_gtest_SOURCE_DIR}/setup_gtest.cmake)