cmake_minimum_required (VERSION 3.20)

project (Sub-Goal LANGUAGES CXX VERSION 1.0)

include(CTest)
enable_testing()

include(${CMAKE_SOURCE_DIR}/cmake/option.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/CompilerWarnings.cmake)

set_choice (CMAKE_BUILD_TYPE "Debug;Release;MinSizeRel;RelWithDebInfo" "Debug" "Choose type of build")

set (SRCS
	src/main.cpp
    src/App/App.cpp
)

set (HEADERS
    src/App/App.hpp    
)

set (ASSETS
    assets/telethon.png
    assets/christmas-tree.png
    assets/waltograph.ttf
    assets/telethon_finish.png
)

if (MINGW)
    set (CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

    if (${CMAKE_BUILD_TYPE} MATCHES "Release" OR ${CMAKE_BUILD_TYPE} MATCHES "MinSizeRel")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
    endif()
endif (MINGW)

message(STATUS "${CMAKE_MODULE_PATH}")

find_package(SDL2pp REQUIRED)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set_sanitizer ()



# Win32 SDL2 dll
if (WIN32)
    set_option(SDL2PP_DLL_DIR "SDL2PP_BINARY_DIR NOT FOUND" PATH "Path to SDL2pp.dll directory")
    configure_file(${SDL2PP_DLL_DIR}/libSDL2pp.dll libSDL2pp.dll COPYONLY)

	# SDL2
    set_option(SDL2_DLL_DIR "SDL2_BINARY_DIR NOT FOUND" PATH "Path to SDL2.dll directory")
    configure_file(${SDL2_DLL_DIR}/SDL2.dll SDL2.dll COPYONLY)
	
	# SDL2 ttf
	set_option(SDL2_TTF_DLL_DIR "SDL2_TTF_BINARY_DIR NOT FOUND" PATH "Path to SDL2_ttf.dll directory")
    configure_file(${SDL2_TTF_DLL_DIR}/SDL2_ttf.dll SDL2_ttf.dll COPYONLY)

    set_option(SDL2_IMAGE_DLL_DIR "SDL2_IMAGE_BINARY_DIR NOT FOUND" PATH "Path to SDL2_image.dll directory")
    configure_file(${SDL2_IMAGE_DLL_DIR}/SDL2_image.dll SDL2_image.dll COPYONLY)

    set_option(SDL2_MIXER_DLL_DIR "SDL2_MIXER_BINARY_DIR NOT FOUND" PATH "Path to SDL2_mixer.dll directory")
    configure_file(${SDL2_MIXER_DLL_DIR}/SDL2_mixer.dll SDL2_mixer.dll COPYONLY)

    foreach(df ${ASSETS})
        configure_file(${CMAKE_SOURCE_DIR}/${df} ${CMAKE_BINARY_DIR}/${df} COPYONLY)
    endforeach()
    

    install(FILES ${SDL2_DLL_DIR}/SDL2.dll DESTINATION .)
    install(FILES ${SDL2PP_DLL_DIR}/libSDL2pp.dll DESTINATION .)
    install(FILES ${SDL2_TTF_DLL_DIR}/SDL2_ttf.dll DESTINATION .)
    install(FILES ${SDL2_IMAGE_DLL_DIR}/SDL2_image.dll DESTINATION .)
    install(FILES ${SDL2_MIXER_DLL_DIR}/SDL2_mixer.dll DESTINATION .)

endif(WIN32)

add_executable(${PROJECT_NAME} ${SRCS} ${HEADERS})
target_link_libraries(${PROJECT_NAME} SDL2pp::SDL2pp)
set_target_warnings(${PROJECT_NAME})

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2PP_INCLUDE_DIRS})

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(DIRECTORY assets/ DESTINATION assets)

foreach(df ${ASSETS})
    file(COPY ${df} DESTINATION ${CMAKE_BINARY_DIR}/assets)
endforeach()


include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR NSIS)
set(CPACK_NSIS_DISPLAY_NAME "${PROJECT_NAME}" )
set(CPACK_NSIS_PACKAGE_NAME "${PROJECT_NAME}" )
set(CPACK_PACKAGE_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}")
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VENDOR "Quentin Horgues")
set (CPACK_NSIS_MODIFY_PATH "ON")

set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}")
set(CPACK_SOURCE_IGNORE_FILES ${CPACK_IGNORE_FILES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)