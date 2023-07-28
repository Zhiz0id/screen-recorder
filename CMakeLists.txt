###############################################################################
##
## Copyright (C) 2023 Yura Beznos at You-ra.info
##
## This file is part of the Screen recorder application project.
##
## This program is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2
## of the License, or (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write an email to the license@you-ra.info
###############################################################################

set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_C_COMPILER "/usr/bin/gcc")

project(info.you_ra.screen_recorder C CXX)
cmake_minimum_required(VERSION 3.5)


find_package (Qt5 COMPONENTS Core Network Qml Gui Quick LinguistTools REQUIRED)
set(AURORAOS_SHARED_LIBRARY_DIR /usr/share/${PROJECT_NAME}/lib)
set(short_name screen_recorder)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -fPIC -pie -rdynamic -Wall -Wl,-rpath=/usr/share/${PROJECT_NAME}/lib")

include(FindPkgConfig)
pkg_search_module(AURORA auroraapp REQUIRED)
pkg_search_module(AURORA auroraapp_i18n REQUIRED)

include(QtTranslationWithID.cmake)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# DEBUG
set(CMAKE_DEBUG_TARGET_PROPERTIES
  INCLUDE_DIRECTORIES
  COMPILE_DEFINITIONS
  POSITION_INDEPENDENT_CODE
  CONTAINER_SIZE_REQUIRED
  LIB_VERSION
)

# libvncclient
include(ExternalProject)
set (LIBVNCCLIENT_DIRECTORY "${CMAKE_BINARY_DIR}/libvncserver")
set (LIBVNCCLIENT_DIR_CONFIG "${CMAKE_HOME_DIRECTORY}/libvncserver")
ExternalProject_Add(libvncclient
   SOURCE_DIR "${CMAKE_HOME_DIRECTORY}/libvncserver"
   BINARY_DIR "${LIBVNCCLIENT_DIRECTORY}"
   BUILD_IN_SOURCE 0
   CONFIGURE_COMMAND cmake -DWITH_JPEG=OFF .
   BUILD_COMMAND cmake --build .
   INSTALL_COMMAND ""
   )

install(FILES
       "${LIBVNCCLIENT_DIRECTORY}/libvncclient.so"
         DESTINATION ${AURORAOS_SHARED_LIBRARY_DIR}
         )
install(FILES
       "${LIBVNCCLIENT_DIRECTORY}/libvncclient.so.0.9.14"
         DESTINATION ${AURORAOS_SHARED_LIBRARY_DIR}
         )
install(FILES
       "${LIBVNCCLIENT_DIRECTORY}/libvncclient.so.1"
         DESTINATION ${AURORAOS_SHARED_LIBRARY_DIR}
         )

# end of libvncclient

# ffmpeg
set (FFMPEG_VERSION 4.4.4 CACHE STRING "FFmpeg version")
set (FFMPEG_NAME "ffmpeg-${FFMPEG_VERSION}")
set (cache_dir "${CMAKE_BINARY_DIR}")
set (FFMPEG_SOURCE_DIR "${cache_dir}/${FFMPEG_NAME}" CACHE PATH "Path to FFmpeg source code")
mark_as_advanced (FORCE FFMPEG_VERSION FFMPEG_SOURCE_DIR FFMPEG_MAKE_EXECUTABLE FFMPEG_INSTALL_DEST)

if (NOT EXISTS "${FFMPEG_SOURCE_DIR}")
    set (FFMPEG_URL "https://ffmpeg.org/releases/${FFMPEG_NAME}.tar.bz2")
    get_filename_component (FFMPEG_ARCHIVE_NAME "${FFMPEG_URL}" NAME)
    set (ffmpeg_tarball "${cache_dir}/${FFMPEG_ARCHIVE_NAME}")
    if(NOT EXISTS "${ffmpeg_tarball}")
        message (STATUS "Downloading FFmpeg sources from ${FFMPEG_URL} to ${FFMPEG_SOURCE_DIR}...")
        file (DOWNLOAD "${FFMPEG_URL}" "${ffmpeg_tarball}")
    endif()
    message (VERBOSE "Unpacking ffmpeg tarball...")
    execute_process (COMMAND "${CMAKE_COMMAND}" -E tar xzf "${ffmpeg_tarball}"
                     WORKING_DIRECTORY "${cache_dir}")
endif ()

if (NOT EXISTS "${FFMPEG_SOURCE_DIR}/libavfilter/libavfilter.so.")
set (CONFIGURE_COMMAND
     "./configure
     --disable-static
     --disable-doc
     --disable-asm
     --enable-shared")

separate_arguments (ffmpeg_config_command UNIX_COMMAND "${CONFIGURE_COMMAND}")

execute_process (
    COMMAND ${ffmpeg_config_command}
    WORKING_DIRECTORY "${FFMPEG_SOURCE_DIR}"
    COMMAND_ECHO STDOUT
    COMMAND_ERROR_IS_FATAL ANY)

find_program (FFMPEG_MAKE_EXECUTABLE 
              NAMES make gmake nmake
              DOC "Path to the make executable used for building FFmpeg"
              REQUIRED)

include (ProcessorCount)
ProcessorCount (NUM_PROCESSORS)
execute_process (
    COMMAND "${FFMPEG_MAKE_EXECUTABLE}" "-j${NUM_PROCESSORS}"
    WORKING_DIRECTORY "${FFMPEG_SOURCE_DIR}"
    COMMAND_ECHO STDOUT
    COMMAND_ERROR_IS_FATAL ANY)
endif ()
# end of ffmpeg

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/
)

set(screenrecorder_SRCS
    src/main.cpp
    src/interfacer.cpp
    src/recordingsmodel.cpp
    src/settings.cpp
    src/recordthread.cpp
    src/vnc2mpg.c
)
file(GLOB_RECURSE screenrecorder_QML_SRCS qml/*.qml)

set(screenrecorder_TS_SRCS
    ${screenrecorder_SRCS}
    ${screenrecorder_QML_SRCS}
)

create_translation(engen_qm_file ${CMAKE_SOURCE_DIR}/translations/screen_recorder.ts ${CMAKE_SOURCE_DIR}/translations/screen_recorder_ru.ts ${screenrecorder_TS_SRCS})

FILE(GLOB TsFiles "translations/*.ts")
qt5_add_translation(QmFiles ${TsFiles})

add_executable(${PROJECT_NAME} ${screenrecorder_SRCS} ${engen_qm_file} ${QmFiles} ${FOLEYS_ARG_BUILD_TARGET})

set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
add_dependencies(${PROJECT_NAME} libvncclient)

foreach (libname IN ITEMS avutil swresample avcodec avformat swscale avdevice avfilter)
    string(TOUPPER ${libname} LIBNAMEUPPER)
    set("${LIBNAMEUPPER}_INCLUDE_DIR" "")
    set("${LIBNAMEUPPER}_LIBRARY" "")
    find_path(
        "${LIBNAMEUPPER}_INCLUDE_DIR"
        "lib${libname}/${libname}.h"
        PATHS ENV
        "${FFMPEG_SOURCE_DIR}")
    find_library("${LIBNAMEUPPER}_LIBRARY"
        "lib${libname}.so"
        PATHS ENV
        "${FFMPEG_SOURCE_DIR}")
    
    add_library(${libname} STATIC IMPORTED)
    set_target_properties(${libname} PROPERTIES IMPORTED_LOCATION 
        "${CMAKE_BINARY_DIR}/ffmpeg-4.4.4/lib${libname}/lib${libname}.so")
    set("${libname}_path" "${CMAKE_BINARY_DIR}/ffmpeg-4.4.4/lib${libname}/lib${libname}.so")
    message (STATUS "Lib ${libname} and ${libname_path}")
endforeach ()

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)
target_include_directories(${PROJECT_NAME} PRIVATE
    $<BUILD_INTERFACE:
    ${AURORA_INCLUDE_DIRS}
    "${LIBVNCCLIENT_DIRECTORY}/include/"
    "${LIBVNCCLIENT_DIR_CONFIG}/include/"
    ${FFMPEG_SOURCE_DIR}
>)
target_link_libraries(${PROJECT_NAME}
    Qt5::Quick
    ${AURORA_LDFLAGS}
    #    ${LIBAV_LDFLAGS}
    ${VNC_LDFLAGS}

    "${FFMPEG_SOURCE_DIR}/libavutil/libavutil.so."
    "${FFMPEG_SOURCE_DIR}/libswresample/libswresample.so."
    "${FFMPEG_SOURCE_DIR}/libavcodec/libavcodec.so."
    "${FFMPEG_SOURCE_DIR}/libavformat/libavformat.so."
    "${FFMPEG_SOURCE_DIR}/libswscale/libswscale.so."
    "${FFMPEG_SOURCE_DIR}/libavdevice/libavdevice.so."
    "${FFMPEG_SOURCE_DIR}/libavfilter/libavfilter.so."
    "${LIBVNCCLIENT_DIRECTORY}/libvncclient.so"
)


foreach (libname IN ITEMS avutil swresample avcodec avformat swscale avdevice avfilter)
    install(FILES
        "${FFMPEG_SOURCE_DIR}/lib${libname}/lib${libname}.so."
        DESTINATION ${AURORAOS_SHARED_LIBRARY_DIR}
        )
    install(FILES
        "${FFMPEG_SOURCE_DIR}/lib${libname}/lib${libname}.so"
        DESTINATION ${AURORAOS_SHARED_LIBRARY_DIR}
        )
endforeach ()


install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)
install(DIRECTORY qml
    DESTINATION share/info.you_ra.screen_recorder
)

foreach(_file IN LISTS QmFiles)
    get_filename_component(_filename "${_file}" NAME)
    string(REPLACE "${short_name}" "${PROJECT_NAME}" _full_filename "${_filename}")
    install(FILES ${_file} DESTINATION share/info.you_ra.screen_recorder/translations)# RENAME ${_full_filename})
endforeach()
#install(FILES ${engen_qm_file} DESTINATION share/info.you_ra.screen_recorder/translations RENAME ${_full_filename})

install(FILES info.you_ra.screen_recorder.desktop
    DESTINATION share/applications
)
install(FILES icons/86x86/info.you_ra.screen_recorder.png
    DESTINATION share/icons/hicolor/86x86/apps
)
install(FILES icons/108x108/info.you_ra.screen_recorder.png
    DESTINATION share/icons/hicolor/108x108/apps
)
install(FILES icons/128x128/info.you_ra.screen_recorder.png
    DESTINATION share/icons/hicolor/128x128/apps
)
install(FILES icons/172x172/info.you_ra.screen_recorder.png
    DESTINATION share/icons/hicolor/172x172/apps
)

# Get the other files reachable from the project tree in Qt Creator
FILE(GLOB TsFiles "translations/*.ts")
add_custom_target(distfiles
    SOURCES
        info.you_ra.screen_recorder.desktop
        qml/screen_recorder.qml
        qml/cover/DefaultCoverPage.qml
        qml/pages/MainPage.qml
        qml/pages/AboutPage.qml
        qml/pages/Settings.qml
        qml/pages/RenameDialog.qml
        qml/pages/ViewVideoPage.qml
        qml/components/RecordingDelegate.qml
        qml/components/MediaTitleOverlay.qml
        rpm/info.you_ra.screen_recorder.spec
        rpm/info.you_ra.screen_recorder.changes.in
        rpm/info.you_ra.screen_recorder.changes.run.in
        ${TsFiles})

file(WRITE "${CMAKE_BINARY_DIR}/QtCreatorDeployment.txt"
    "${CMAKE_INSTALL_PREFIX}
${CMAKE_BINARY_DIR}/info.you_ra.screen_recorder:bin
")

