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

project(info.you_ra.screen-recorder C CXX)
cmake_minimum_required(VERSION 3.5)

find_package (Qt5 COMPONENTS Core Network Qml Gui Quick LinguistTools REQUIRED)

include(FindPkgConfig)
pkg_search_module(AURORA auroraapp REQUIRED)
pkg_search_module(AURORA auroraapp_i18n REQUIRED)
pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
    libavdevice
    libavfilter
    libavformat
    libavcodec
    libswresample
    libswscale
    libavutil
)
pkg_search_module(VNC libvncclient REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

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
set(short_name screen-recorder)

FILE(GLOB TsFiles "translations/*.ts")
qt5_add_translation(QmFiles ${TsFiles})

add_executable(info.you_ra.screen-recorder ${screenrecorder_SRCS} ${QmFiles})
target_compile_definitions(info.you_ra.screen-recorder PRIVATE
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)
target_include_directories(info.you_ra.screen-recorder PRIVATE
    $<BUILD_INTERFACE:
    ${AURORA_INCLUDE_DIRS}
    ${LIBAV_INCLUDE_DIRS}
    ${VNC_INCLUDE_DIRS}
>)
target_link_libraries(info.you_ra.screen-recorder
    Qt5::Quick
    ${AURORA_LDFLAGS}
    ${LIBAV_LDFLAGS}
    ${VNC_LDFLAGS}
)

install(TARGETS info.you_ra.screen-recorder
    RUNTIME DESTINATION bin
)
install(DIRECTORY qml
    DESTINATION share/info.you_ra.screen-recorder
)

foreach(_file IN LISTS QmFiles)
    get_filename_component(_filename "${_file}" NAME)
    string(REPLACE "${short_name}" "${PROJECT_NAME}" _full_filename "${_filename}")
    install(FILES ${_file} DESTINATION share/info.you_ra.screen-recorder/translations RENAME ${_full_filename})
endforeach()

install(FILES info.you_ra.screen-recorder.desktop
    DESTINATION share/applications
)
install(FILES icons/86x86/info.you_ra.screen-recorder.png
    DESTINATION share/icons/hicolor/86x86/apps
)
install(FILES icons/108x108/info.you_ra.screen-recorder.png
    DESTINATION share/icons/hicolor/108x108/apps
)
install(FILES icons/128x128/info.you_ra.screen-recorder.png
    DESTINATION share/icons/hicolor/128x128/apps
)
install(FILES icons/172x172/info.you_ra.screen-recorder.png
    DESTINATION share/icons/hicolor/172x172/apps
)

# Get the other files reachable from the project tree in Qt Creator
FILE(GLOB TsFiles "translations/*.ts")
add_custom_target(distfiles
    SOURCES
        info.you_ra.screen-recorder.desktop
        qml/screen-recorder.qml
        qml/cover/DefaultCoverPage.qml
        qml/pages/MainPage.qml
        qml/pages/AboutPage.qml
        rpm/info.you_ra.screen-recorder.spec
        rpm/info.you_ra.screen-recorder.changes.in
        rpm/info.you_ra.screen-recorder.changes.run.in
        ${TsFiles})

file(WRITE "${CMAKE_BINARY_DIR}/QtCreatorDeployment.txt"
    "${CMAKE_INSTALL_PREFIX}
${CMAKE_BINARY_DIR}/info.you_ra.screen-recorder:bin
")

