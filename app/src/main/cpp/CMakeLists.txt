

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

# --------------------- Repository declarations ------------------------


#repository download and settings for alias imgui...


	FetchContent_Declare(imgui_project

		GIT_REPOSITORY https://github.com/ocornut/imgui

		GIT_TAG origin/master

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/imgui

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES imgui_project)


#repository download and settings for alias entt...


	FetchContent_Declare(entt_project

		GIT_REPOSITORY https://github.com/skypjack/entt

		GIT_TAG origin/master

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/entt

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES entt_project)


#repository download and settings for alias glm...


	FetchContent_Declare(glm_project

		GIT_REPOSITORY https://github.com/g-truc/glm

		GIT_TAG 0.9.9.8

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/glm

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES glm_project)


#repository download and settings for alias sdl_repo...


	FetchContent_Declare(sdl_repo_project

		GIT_REPOSITORY https://github.com/libsdl-org/SDL

		GIT_TAG release-2.0.22

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/sdl_repo

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES sdl_repo_project)


#repository download and settings for alias fmt...


	FetchContent_Declare(fmt_project

		GIT_REPOSITORY https://github.com/fmtlib/fmt

		GIT_TAG 8.1.1

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/fmt

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES fmt_project)


#repository download and settings for alias imgui_editor...


	FetchContent_Declare(imgui_editor_project

		GIT_REPOSITORY https://github.com/BalazsJako/ImGuiColorTextEdit

		GIT_TAG origin/master

		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/imgui_editor

	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES imgui_editor_project)


	FetchContent_MakeAvailable(${${PROJECT_NAME}_DEPENDENCIES})

	foreach(X IN LISTS ${PROJECT_NAME}_DEPENDENCIES)

		FetchContent_GetProperties(${X}
			SOURCE_DIR ${X}_SOURCE_DIR
			BINARY_DIR ${X}_BINARY_DIR
		)

	endforeach() 

#creating library
add_library(goodreminder SHARED

	${PROJECT_SOURCE_DIR}/main.cpp

	${PROJECT_SOURCE_DIR}/sources/android_data/android_data.cpp

	${PROJECT_SOURCE_DIR}/sources/app_manager/app_manager.cpp

	${PROJECT_SOURCE_DIR}/sources/helpers/color.cpp

	${PROJECT_SOURCE_DIR}/sources/helpers/helpers.cpp

	${PROJECT_SOURCE_DIR}/sources/global.cpp

	${PROJECT_SOURCE_DIR}/sources/good_reminder/app_layout.cpp

	${PROJECT_SOURCE_DIR}/sources/good_reminder/data_structures.cpp

	${PROJECT_SOURCE_DIR}/sources/good_reminder/initial_window.cpp

	${PROJECT_SOURCE_DIR}/sources/opengl_wrappers/framebuffer.cpp

	${PROJECT_SOURCE_DIR}/sources/opengl_wrappers/index_buffer.cpp

	${PROJECT_SOURCE_DIR}/sources/opengl_wrappers/shader.cpp

	${PROJECT_SOURCE_DIR}/sources/opengl_wrappers/texture.cpp

	${PROJECT_SOURCE_DIR}/sources/opengl_wrappers/vertex_array.cpp

	${PROJECT_SOURCE_DIR}/sources/opengl_wrappers/vertex_buffer.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_demo.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_sdl.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/misc/cpp/imgui_stdlib.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui_editor/TextEditor.cpp

)

set_property(TARGET goodreminder PROPERTY CXX_STANDARD 17)


#adding libraries...

target_link_libraries(goodreminder PUBLIC EGL)
target_link_libraries(goodreminder PUBLIC GLESv3)
target_link_libraries(goodreminder PUBLIC android)
target_link_libraries(goodreminder PUBLIC log)

target_link_libraries(goodreminder PUBLIC SDL2)

target_link_libraries(goodreminder PUBLIC fmt::fmt)


#adding includes...


target_include_directories(goodreminder PUBLIC ${PROJECT_SOURCE_DIR}/vendor)

target_include_directories(goodreminder PUBLIC ${PROJECT_SOURCE_DIR}/sources)

target_include_directories(goodreminder PUBLIC ${PROJECT_SOURCE_DIR}/vendor/imgui/)

target_include_directories(goodreminder PUBLIC ${PROJECT_SOURCE_DIR}/vendor/entt/single_include)

target_include_directories(goodreminder PUBLIC ${PROJECT_SOURCE_DIR}/vendor/glm/)

target_include_directories(goodreminder PUBLIC ${PROJECT_SOURCE_DIR}/vendor/sdl_repo/include)

target_include_directories(goodreminder PUBLIC ${PROJECT_SOURCE_DIR}/vendor/fmt/include)
