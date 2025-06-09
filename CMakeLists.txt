cmake_minimum_required(VERSION 3.16)

project(Nextcloud-News VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network WebEngineWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network WebEngineWidgets)

set(PROJECT_SOURCES
        src/main.cpp
        src/loginwindow.cpp
        src/loginwindow.h
        src/loginwindow.ui
        src/mainwindow.h
        src/mainwindow.cpp
        src/mainwindow.ui
        src/news.h
        src/news.cpp
        src/newscontent.h
        src/newscontent.cpp
        src/newscontent.ui
        resources.qrc
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Nextcloud-News
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(Nextcloud-News SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Nextcloud-News
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Nextcloud-News PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::WebEngineWidgets)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Nextcloud-News)
endif()
set_target_properties(Nextcloud-News PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Nextcloud-News
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Nextcloud-News)
endif()
