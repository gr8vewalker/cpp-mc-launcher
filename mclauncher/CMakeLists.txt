set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

include(FetchContent)
 
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable (mclauncher "mclauncher.cpp" "mclauncher.h" "consolelauncher.h"  "consolelauncher.cpp" "constants.h" "fileutils.h" "cliutils.h" "strutils.h"  "download.h" "sha1.h" "os.h" "unzip.h" "unzip.cpp"  "exec.h")

target_link_libraries(mclauncher PRIVATE nlohmann_json::nlohmann_json urlmon.lib ws2_32)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET mclauncher PROPERTY CXX_STANDARD 20)
endif()
