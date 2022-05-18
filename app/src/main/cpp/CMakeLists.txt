

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


	dir_exists(imgui_exists ${PROJECT_SOURCE_DIR}/vendor/imgui)


	if(NOT ${imgui_exists})

		FetchContent_Declare(imgui

			GIT_REPOSITORY https://github.com/ocornut/imgui

			GIT_TAG origin/docking

			SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/imgui

		)
		FetchContent_MakeAvailable(imgui)
	endif()


#repository download and settings for alias fmt...


	dir_exists(fmt_exists ${PROJECT_SOURCE_DIR}/vendor/fmt)


	if(NOT ${fmt_exists})

		ExternalProject_Add(fmt

			GIT_REPOSITORY https://github.com/fmtlib/fmt

			GIT_TAG 8.1.1

			CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_SOURCE_DIR}/vendor/fmt
					   -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}

		BUILD_BYPRODUCTS ${PROJECT_SOURCE_DIR}/vendor/fmt/lib/${CMAKE_STATIC_LIBRARY_PREFIX}fmt$<$<CONFIG:Debug>:d>${CMAKE_STATIC_LIBRARY_SUFFIX}

		)

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


#creating library
add_library(goodreminder SHARED

	${PROJECT_SOURCE_DIR}/main.cpp

	${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
	${PROJECT_SOURCE_DIR}/sources/android_data/android_data.cpp

	${PROJECT_SOURCE_DIR}/sources/app_manager/app_manager.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_demo.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_android.cpp

	${PROJECT_SOURCE_DIR}/vendor/imgui/misc/cpp/imgui_stdlib.cpp

)

set_property(TARGET goodreminder PROPERTY CXX_STANDARD 17)


#setting dependencies...


if(NOT ${fmt_exists})
	add_dependencies(goodreminder fmt)
endif()

#adding libraries...

target_link_libraries(goodreminder PUBLIC EGL)
target_link_libraries(goodreminder PUBLIC GLESv3)
target_link_libraries(goodreminder PUBLIC android)
target_link_libraries(goodreminder PUBLIC log)

add_library(fmt_target STATIC IMPORTED)


if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set_target_properties(fmt_target PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/vendor/fmt/lib/${CMAKE_STATIC_LIBRARY_PREFIX}fmtd${CMAKE_STATIC_LIBRARY_SUFFIX})

	target_link_libraries(goodreminder PUBLIC fmt_target)
else()

	set_target_properties(fmt_target PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/vendor/fmt/lib/${CMAKE_STATIC_LIBRARY_PREFIX}fmt${CMAKE_STATIC_LIBRARY_SUFFIX})

	target_link_libraries(goodreminder PUBLIC fmt_target)
endif()


#adding includes...


target_include_directories(goodreminder PUBLIC ${PROJECT_SOURCE_DIR}/vendor)

target_include_directories(goodreminder PUBLIC ${PROJECT_SOURCE_DIR}/sources)

target_include_directories(goodreminder PUBLIC ${ANDROID_NDK}/sources/android/native_app_glue/)

target_include_directories(goodreminder PUBLIC ${PROJECT_SOURCE_DIR}/vendor/imgui/)

target_include_directories(goodreminder PUBLIC ${PROJECT_SOURCE_DIR}/vendor/fmt/include)

target_include_directories(goodreminder PUBLIC ${PROJECT_SOURCE_DIR}/vendor/entt/single_include)
