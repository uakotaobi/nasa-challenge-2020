PROJECT(altitution CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
SET(CMAKE_CXX_STANDARD 17)

# For Cygwin environments only: helps find_package to locate Cygwin's SDL2 libraries.
IF(CYGWIN)
    SET(SDL2_TTF_LIBRARY "c:/tools/cygwin/lib/libSDL2_ttf.dll.a")
    SET(SDL2_TTF_INCLUDE_DIR "c:/tools/cygwin/usr/include/SDL2")
    SET(SDL2_IMAGE_LIBRARY "c:/tools/cygwin/lib/libSDL2_image.dll.a")
    SET(SDL2_IMAGE_INCLUDE_DIR "c:/tools/cygwin/usr/include/SDL2")
ENDIF()

FIND_PACKAGE(SDL2 REQUIRED)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


FIND_PACKAGE(SDL2_ttf REQUIRED)
FIND_PACKAGE(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

ADD_EXECUTABLE(altitution-bin src/Main.cpp src/view.cpp src/button_view.cpp src/asset_manager.cpp src/menu_view.cpp src/main_view.cpp src/moon_view.cpp src/info_view.cpp src/nav_view.cpp src/point.cpp src/vector.cpp src/matrix.cpp src/plane.cpp src/grid.cpp src/basis.cpp src/common.cpp src/render.cpp src/polygon.cpp src/fps_view.cpp)

TARGET_LINK_LIBRARIES(altitution-bin ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
