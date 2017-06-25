cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 14)

set(PRJ_TARGET "tbbtest")
set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
project(${PRJ_TARGET})

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules/")
#set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules/)

# ---------------------------------------------------------
# itbb
SET(TBB_INCLUDE_DIR "/home/oliver/Downloads/tbb2017_20170226oss/include")
SET(TBB_LIBRARIES "/home/oliver/Downloads/tbb2017_20170226oss/lib/")
SET(TBB_DEFINITIONS_DEBUG 1)
SET(TBB_LIBRARIES_DEBUG /home/oliver/Downloads/tbb2017_20170226oss/lib/intel64/gcc4.7/libtbb_debug.so)
SET(TBB_LIBRARY "/home/oliver/Downloads/tbb2017_20170226oss/lib/intel64/gcc4.7libtbb.so")
find_package(TBB)

include_directories(${TBB_INCLUDE_DIR})
add_definitions(${TBB_DEFINITIONS})



# -------------------------------------------------------
# SDL
find_package(SDL2 REQUIRED)
find_package(SDL2TTF)
include_directories(${SDL2_INCLUDE_DIR})

file(GLOB source_files src/core/*.cpp src/core/*.h src/view/*.cpp src/view/*.h)
add_executable(boids        src/main.cpp     ${source_files})
target_link_libraries(boids ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY})

file(GLOB source_files src/core/*.cpp src/core/*.h src/view/*.cpp src/view/*.h)
add_executable(boids_tbb1 src/main_tbb1.cpp ${source_files})
target_link_libraries(boids_tbb1 ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY})
target_link_libraries(boids_tbb1 ${TBB_LIBRARIES})
target_link_libraries(boids_tbb1 ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY})


#file(GLOB source_files src/core/Vector3D.cpp src/core/Stopwatch.cpp src/core/Stopwatch.h src/view/Vector3.h src/core/Boid.h)
file(GLOB source_files src/core/*.cpp src/core/*.h src/view/*.cpp src/view/*.h)
add_executable(boids_tbb    src/main_tbb.cpp ${source_files})
target_link_libraries(boids_tbb ${TBB_LIBRARIES})
target_link_libraries(boids_tbb ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY})

file(GLOB source_files src/core/*.cpp src/core/*.h src/view/*.cpp src/view/*.h)
add_executable(boids_tbb2    src/main_tbb2.cpp ${source_files})
target_link_libraries(boids_tbb2 ${TBB_LIBRARIES})
target_link_libraries(boids_tbb2 ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY})

file(GLOB source_files src/core/*.cpp src/core/*.h src/view/*.cpp src/view/*.h)
add_executable(boids_tbb4    src/main_tbb4.cpp ${source_files})
target_link_libraries(boids_tbb4 ${TBB_LIBRARIES})
target_link_libraries(boids_tbb4 ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY})


add_executable(tbb_test        src/test_tbb.cpp)
target_link_libraries(tbb_test ${TBB_LIBRARIES})