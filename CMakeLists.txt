PROJECT(altitution CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

# Quiet the complaint about CMake no longer defining WIN32 on MSYS.
set(CMAKE_LEGACY_CYGWIN_WIN32 0)

# Teach CMake where MSys2 keeps libraries and modules.
list(APPEND CMAKE_PREFIX_PATH /mingw64/lib/cmake/SDL2)

SET(CMAKE_CXX_STANDARD 17)
FIND_PACKAGE(SDL2 REQUIRED)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(SDL2_TTF_LIBRARY /mingw64/bin/SDL2_ttf.dll)
set(SDL2_IMAGE_LIBRARY /mingw64/bin/SDL2_image.dll)
FIND_PACKAGE(SDL2_ttf REQUIRED)
FIND_PACKAGE(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

ADD_EXECUTABLE(altitution-bin src/main.cpp src/view.cpp src/button_view.cpp src/font_manager.cpp src/menu_view.cpp)

TARGET_LINK_LIBRARIES(altitution-bin ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
