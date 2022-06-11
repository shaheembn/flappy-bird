cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Flappy-Bird)

add_executable(flappy-bird
	src/main.cpp
    src/bird.cpp
	src/obstacle.cpp
)

target_include_directories(flappy-bird PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(flappy-bird PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
# target_include_directories(flappy-bird PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dep)

if (UNIX)
target_link_libraries(flappy-bird PUBLIC sfml-system sfml-window sfml-graphics ${CMAKE_DL_LIBS})
endif (UNIX)

if (WIN32)
target_link_options(flappy-bird PUBLIC -static-libgcc -static-libstdc++)
target_link_libraries(flappy-bird PUBLIC sfml-system sfml-window sfml-graphics)
endif(WIN32)
