################################################################################
##
## Copyright (C) 2022 info.you_ra
## 
## This file is part of the Screen recorder application project.
##
## Redistribution and use in source and binary forms,
## with or without modification, are permitted provided
## that the following conditions are met:
##
## * Redistributions of source code must retain the above copyright notice,
##   this list of conditions and the following disclaimer.
## * Redistributions in binary form must reproduce the above copyright notice,
##   this list of conditions and the following disclaimer
##   in the documentation and/or other materials provided with the distribution.
## * Neither the name of the copyright holder nor the names of its contributors
##   may be used to endorse or promote products derived from this software
##   without specific prior written permission.
##
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
## AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
## THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
## FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
## IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
## FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
## OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
## PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
## LOSS OF USE, DATA, OR PROFITS;
## OR BUSINESS INTERRUPTION)
## HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
## WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
## (INCLUDING NEGLIGENCE OR OTHERWISE)
## ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
## EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
##
################################################################################
#set(CMAKE_MAKE_PROGRAM "/usr/bin/make")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_C_COMPILER "/usr/bin/gcc")

project(info.you_ra.screen_recorder C CXX)
cmake_minimum_required(VERSION 3.5)


find_package (Qt5 COMPONENTS Core Network Qml Gui Quick LinguistTools REQUIRED)
set(AURORAOS_SHARED_LIBRARY_DIR /usr/share/${PROJECT_NAME}/lib)
set(short_name screen_recorder)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -std=c++0x -fPIC -pie -rdynamic -Wall -Wl,-rpath=/usr/share/${PROJECT_NAME}/lib")

#add_subdirectory(FFmpegBuild)
include(FindPkgConfig)
pkg_search_module(AURORA auroraapp REQUIRED)
pkg_search_module(AURORA auroraapp_i18n REQUIRED)
#pkg_search_module(VNC libvncclient REQUIRED)
#pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
#    libavdevice
#    libavfilter
#    libavformat
#    libavcodec
#    libswresample
#    libswscale
#    libavutil
#)
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
   CONFIGURE_COMMAND cmake .
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

#string (REPLACE " -Wl,--fatal-warnings" "" FFMPEG_LD_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
#set (FFMPEG_LD_FLAGS "${FFMPEG_C_FLAGS} ${FFMPEG_LD_FLAGS} ${FFMPEG_EXTRA_LD_FLAGS}")
if (NOT EXISTS "${FFMPEG_SOURCE_DIR}/libavfilter/libavfilter.so.")
set (CONFIGURE_COMMAND
     "./configure
     --disable-static
     --disable-doc
     --disable-asm
     --enable-shared")
     #--shlibdir=${FOLEYS_ARG_OUTPUT_DIR}
     #--libdir=${FOLEYS_ARG_OUTPUT_DIR}
     #--incdir=${FOLEYS_ARG_OUTPUT_DIR}/${CMAKE_INSTALL_INCLUDEDIR}
     #--prefix=${FOLEYS_ARG_OUTPUT_DIR}")

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
#qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} screen_recorder.ts screen_recorder_ru.ts)

add_executable(${PROJECT_NAME} ${screenrecorder_SRCS} ${engen_qm_file} ${QmFiles} ${FOLEYS_ARG_BUILD_TARGET})
#set(QT_DEPLOY_TRANSLATIONS_DIR "share/info.you_ra.screen_recorder/translations")

set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
add_dependencies(${PROJECT_NAME} libvncclient)
#find_path("AVCODEC_INCLUDE_DIR" "libavcodec/avcodec.h" PATHS "${CMAKE_SOURCE_DIR}/FFmpegBuild/Cache/ffmpeg-4.4.4/libavcodec/")
#find_library("AVCODEC_LIBRARY" "avcodec" PATHS "${CMAKE_SOURCE_DIR}/FFmpegBuild/Cache/ffmpeg-4.4.4/libavcodec/")

foreach (libname IN ITEMS avutil swresample avcodec avformat swscale avdevice avfilter)
    string(TOUPPER ${libname} LIBNAMEUPPER)
    message (STATUS "Name ${libname} and ${LIBNAMEUPPER}...")
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
    message (STATUS "Also ${LIBNAMEUPPER_INCLUDE_DIR} and ${LIBNAMEUPPER_LIBRARY}")
    message (STATUS "Path ${CMAKE_BINARY_DIR}/ffmpeg-4.4.4/ and ${CMAKE_BINARY_DIR}/ffmpeg-4.4.4/lib${libname}/")
    #file(RENAME "${CMAKE_SOURCE_DIR}/FFmpegBuild/Cache/ffmpeg-4.4.4/lib${libname}/lib${libname}.so."
    #            "${CMAKE_BINARY_DIR}/FFmpegBuild/ffmpeg/lib${libname}.so")
    #install(FILES
    #    "${CMAKE_BINARY_DIR}/ffmpeg-4.4.4/lib${libname}/lib${libname}.so"
    #    DESTINATION ${AURORAOS_SHARED_LIBRARY_DIR}
    #    )
    
    add_library(${libname} STATIC IMPORTED)
    #add_dependencies("lib${LIBNAMEUPPER}.so" ${PROJECT_NAME})
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
    #    ${LIBAV_INCLUDE_DIRS}
    #${VNC_INCLUDE_DIRS}
    "${LIBVNCCLIENT_DIRECTORY}/include/"
    "${LIBVNCCLIENT_DIR_CONFIG}/include/"
    ${FFMPEG_SOURCE_DIR}

    #${AVCODEC_INCLUDE_DIR} 
    #${AVFORMAT_INCLUDE_DIR}
    #${AVFILTER_INCLUDE_DIR}
    #${AVUTIL_INCLUDE_DIR} 
    #${AVDEVICE_INCLUDE_DIR}
    #${SWRESAMPLE_INCLUDE_DIR}
    #${SWSCALE_INCLUDE_DIR}
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
    #${avutil_path}
    #${swresample_path}
    #${avcodec_path}
    #${avformat_path}
    #${swscale_path}
    #${avdevice_path}
    #${avfilter_path}
    #${AVCODEC_LIBRARY} 
    #${AVFORMAT_LIBRARY} 
    #${AVFILTER_LIBRARY} 
    #${AVUTIL_LIBRARY} 
    #${AVDEVICE_LIBRARY}
    #${SWRESAMPLE_LIBRARY}
    #${SWSCALE_LIBRARY}
)


foreach (libname IN ITEMS avutil swresample avcodec avformat swscale avdevice avfilter)
    #file(COPY "${FFMPEG_SOURCE_DIR}/lib${libname}/lib${libname}.so."
    #    DESTINATION "${CMAKE_BINARY_DIR}/ffmpeg-4.4.4/")
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
        rpm/info.you_ra.screen_recorder.spec
        rpm/info.you_ra.screen_recorder.changes.in
        rpm/info.you_ra.screen_recorder.changes.run.in
        ${TsFiles})

file(WRITE "${CMAKE_BINARY_DIR}/QtCreatorDeployment.txt"
    "${CMAKE_INSTALL_PREFIX}
${CMAKE_BINARY_DIR}/info.you_ra.screen_recorder:bin
")

