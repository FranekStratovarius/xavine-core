set(NAME "xavine")
project(${NAME})
cmake_minimum_required(VERSION 3.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-Wall")

if (DEBUGBUILD)
	set(CMAKE_BUILD_TYPE Debug)
else (DEBUGBUILD)
	set(CMAKE_BUILD_TYPE Release)
endif (DEBUGBUILD)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/libs/mylua)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/libs/SDL)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/libs/mylua/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/libs/SDL/include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/include)

file(GLOB_RECURSE SOURCES_CXX src/cpp/*.cpp)

add_executable(${NAME} ${SOURCES_CXX})

if (LINUX)
	target_link_libraries(${NAME} SDL2 mylua ${CMAKE_DL_LIBS})
endif (LINUX)

if (OSX)
	set(BUILD_SHARED_LIBS OFF)
	target_link_libraries(${NAME} SDL2 mylua ${CMAKE_DL_LIBS})
endif (OSX)

if (WINDOWS)
	#static or shared libs
	#set(BUILD_SHARED_LIBS OFF)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static -static-libgcc -lmingw32")

	# Only show the console in debug builds and also strip unused dependencies on MinSizeRel.
	set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-mconsole")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-mwindows")
	set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "-mwindows -s")
	target_link_libraries(${NAME} mingw32 SDL2main SDL2 mylua ${CMAKE_DL_LIBS})
endif (WINDOWS)
