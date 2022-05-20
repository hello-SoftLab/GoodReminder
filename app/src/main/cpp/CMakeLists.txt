

#this CMakeLists was created with EasyCmake - V2 
#the repository can be found at https://github.com/knz13/EasyCmake_Cpp


cmake_minimum_required(VERSION 3.18)



#adding useful functions

function(DIR_EXISTS variable dir_path)

file(GLOB ${variable}_check ${dir_path}/*)

list(LENGTH ${variable}_check ${variable}_len)

if(${${variable}_len} EQUAL 0)

set(${variable} FALSE PARENT_SCOPE)

else()

set(${variable} TRUE PARENT_SCOPE)

endif()

endfunction()

#adding extra cmake libs
include(GNUInstallDirs)
include(ExternalProject)
include(FetchContent)

#project name
project("app_project")

#adding subdirectories...

add_subdirectory(vendor/sdl_repo)


# --------------------- Repository declarations ------------------------


#repository download and settings for alias imgui...


	dir_exists(imgui_exists ${PROJECT_SOURCE_DIR}/vendor/imgui)


	if(NOT ${imgui_exists})

		FetchContent_Declare(imgui

			GIT_REPOSITORY https://github.com/ocornut/imgui

			GIT_TAG origin/docking

			SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/imgui

		)
		FetchContent_MakeAvailable(imgui)
	endif()


#repository download and settings for alias entt...


	dir_exists(entt_exists ${PROJECT_SOURCE_DIR}/vendor/entt)


	if(NOT ${entt_exists})

		FetchContent_Declare(entt

			GIT_REPOSITORY https://github.com/skypjack/entt

			GIT_TAG origin/master

			SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/entt

		)
		FetchContent_MakeAvailable(entt)
	endif()


#repository download and settings for alias glm...


	dir_exists(glm_exists ${PROJECT_SOURCE_DIR}/vendor/glm)


	if(NOT ${glm_exists})

		FetchContent_Declare(glm

			GIT_REPOSITORY https://github.com/g-truc/glm

			GIT_TAG 0.9.9.8

			SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/glm

		)
		FetchContent_MakeAvailable(glm)
	endif()


#repository download and settings for alias sdl_repo...


	dir_exists(sdl_repo_exists ${PROJECT_SOURCE_DIR}/vendor/sdl_repo)


	if(NOT ${sdl_repo_exists})

		FetchContent_Declare(sdl_repo

			GIT_REPOSITORY https://github.com/libsdl-org/SDL

			GIT_TAG release-2.0.22

			SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/sdl_repo

		)
		FetchContent_MakeAvailable(sdl_repo)
	endif()


#creating library
add_library(goodreminder SHARED

	${PROJECT_SOURCE_DIR}/main.cpp

	${PROJECT_SOURCE_DIR}/sources/android_data/android_data.cpp

	${PROJECT_SOURCE_DIR}/sources/app_manager/app_manager.cpp

	${PROJECT_SOURCE_DIR}/sources/helpers/color.cpp

	${PROJECT_SOURCE_DIR}/sources/helpers/helpers.cpp

	${PROJECT_SOURCE_DIR}/sources/global.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_demo.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_sdl.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/misc/cpp/imgui_stdlib.cpp

)

set_property(TARGET goodreminder PROPERTY CXX_STANDARD 17)


#adding libraries...

target_link_libraries(goodreminder PUBLIC EGL)
target_link_libraries(goodreminder PUBLIC GLESv3)
target_link_libraries(goodreminder PUBLIC android)
target_link_libraries(goodreminder PUBLIC log)
target_link_libraries(goodreminder PUBLIC SDL2)


#adding includes...


target_include_directories(goodreminder PUBLIC ${PROJECT_SOURCE_DIR}/vendor)

target_include_directories(goodreminder PUBLIC ${PROJECT_SOURCE_DIR}/sources)

target_include_directories(goodreminder PUBLIC ${PROJECT_SOURCE_DIR}/vendor/imgui/)

target_include_directories(goodreminder PUBLIC ${PROJECT_SOURCE_DIR}/vendor/entt/single_include)

target_include_directories(goodreminder PUBLIC ${PROJECT_SOURCE_DIR}/vendor/glm/)

target_include_directories(goodreminder PUBLIC ${PROJECT_SOURCE_DIR}/vendor/sdl_repo/include)
