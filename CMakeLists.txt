cmake_minimum_required(VERSION 3.16)

project(pb-safety VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

include(FetchContent)
option(PB_SAFETY_ENABLE_LIMEREPORT "Enable report generation through LimeReport" ON)
option(PB_SAFETY_ENABLE_WINDOWS_DEPLOY "Run windeployqt after build on Windows" ON)
option(PB_SAFETY_ENABLE_MACOS_DMG "Enable DMG packaging on macOS" ON)

set(PB_SAFETY_USE_LIMEREPORT ${PB_SAFETY_ENABLE_LIMEREPORT})
if(PB_SAFETY_USE_LIMEREPORT)
    set(PB_SAFETY_LIMEREPORT_QT_COMPONENTS Qml Xml PrintSupport Svg Network)
    set(PB_SAFETY_LIMEREPORT_MISSING_QT_COMPONENTS)

    set(PB_SAFETY_QT_PACKAGE_DIR "${Qt${QT_VERSION_MAJOR}_DIR}")
    get_filename_component(PB_SAFETY_QT_CMAKE_ROOT "${PB_SAFETY_QT_PACKAGE_DIR}" DIRECTORY)

    foreach(PB_SAFETY_QT_COMPONENT IN LISTS PB_SAFETY_LIMEREPORT_QT_COMPONENTS)
        set(PB_SAFETY_QT_COMPONENT_CONFIG
            "${PB_SAFETY_QT_CMAKE_ROOT}/Qt${QT_VERSION_MAJOR}${PB_SAFETY_QT_COMPONENT}/Qt${QT_VERSION_MAJOR}${PB_SAFETY_QT_COMPONENT}Config.cmake")
        if(NOT EXISTS "${PB_SAFETY_QT_COMPONENT_CONFIG}")
            list(APPEND PB_SAFETY_LIMEREPORT_MISSING_QT_COMPONENTS ${PB_SAFETY_QT_COMPONENT})
        endif()
    endforeach()

    if(NOT PB_SAFETY_LIMEREPORT_MISSING_QT_COMPONENTS)
        find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS ${PB_SAFETY_LIMEREPORT_QT_COMPONENTS})
    endif()

    foreach(PB_SAFETY_QT_COMPONENT IN LISTS PB_SAFETY_LIMEREPORT_QT_COMPONENTS)
        if(NOT TARGET Qt${QT_VERSION_MAJOR}::${PB_SAFETY_QT_COMPONENT})
            if(NOT PB_SAFETY_QT_COMPONENT IN_LIST PB_SAFETY_LIMEREPORT_MISSING_QT_COMPONENTS)
                list(APPEND PB_SAFETY_LIMEREPORT_MISSING_QT_COMPONENTS ${PB_SAFETY_QT_COMPONENT})
            endif()
        endif()
    endforeach()

    if(PB_SAFETY_LIMEREPORT_MISSING_QT_COMPONENTS)
        string(JOIN ", " PB_SAFETY_LIMEREPORT_MISSING_QT_COMPONENTS_TEXT ${PB_SAFETY_LIMEREPORT_MISSING_QT_COMPONENTS})
        message(WARNING
            "LimeReport support is disabled: missing Qt${QT_VERSION_MAJOR} components: "
            "${PB_SAFETY_LIMEREPORT_MISSING_QT_COMPONENTS_TEXT}.")
        set(PB_SAFETY_USE_LIMEREPORT OFF)
    endif()
endif()

set(PB_SAFETY_HAS_LIMEREPORT OFF)
if(PB_SAFETY_USE_LIMEREPORT)
    set(LIMEREPORT_DEMO OFF CACHE BOOL "" FORCE)
    set(LIMEREPORT_STATIC ON CACHE BOOL "" FORCE)
    set(ENABLE_ZINT OFF CACHE BOOL "" FORCE)
    set(ENABLE_DIALOG_DESIGNER OFF CACHE BOOL "" FORCE)
    set(HAVE_UI_LOADER OFF CACHE BOOL "" FORCE)

    if(QT_VERSION_MAJOR EQUAL 6)
        set(USE_QT6 ON CACHE BOOL "" FORCE)
    endif()

    FetchContent_Declare(
        LimeReport
        GIT_REPOSITORY https://github.com/fralx/LimeReport.git
        GIT_TAG e6210b58a46ab14c0ff8a409b5fa5d339937ea7c
    )
    FetchContent_MakeAvailable(LimeReport)

    if(TARGET limereport-qt${QT_VERSION_MAJOR})
        set(PB_SAFETY_HAS_LIMEREPORT ON)
    else()
        message(WARNING "LimeReport target was not created, report generation will be disabled.")
    endif()
endif()

set(TS_FILES pb-safety_ru_RU.ts)

set(PROJECT_SOURCES
    main.cpp
    database_connection.h
    database_connection.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    calculations_of_energy_potentials_window.h
    calculations_of_energy_potentials_window.cpp
    reports.qrc
    ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(pb-safety
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

    # qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(pb-safety SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(pb-safety
            ${PROJECT_SOURCES}
        )
    endif()

    # qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(pb-safety PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

if(PB_SAFETY_HAS_LIMEREPORT)
    target_link_libraries(pb-safety PRIVATE limereport-qt${QT_VERSION_MAJOR})
    target_compile_definitions(pb-safety PRIVATE PB_SAFETY_HAS_LIMEREPORT=1)
endif()

if(WIN32 AND PB_SAFETY_ENABLE_WINDOWS_DEPLOY)
    get_target_property(PB_SAFETY_QMAKE_EXECUTABLE Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
    if(PB_SAFETY_QMAKE_EXECUTABLE)
        get_filename_component(PB_SAFETY_QT_BIN_DIR "${PB_SAFETY_QMAKE_EXECUTABLE}" DIRECTORY)
    endif()

    find_program(PB_SAFETY_WINDEPLOYQT_EXECUTABLE
        NAMES windeployqt
        HINTS "${PB_SAFETY_QT_BIN_DIR}")

    if(PB_SAFETY_WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET pb-safety POST_BUILD
            COMMAND "${PB_SAFETY_WINDEPLOYQT_EXECUTABLE}" --verbose 0 --no-translations "$<TARGET_FILE:pb-safety>"
            COMMENT "Deploying Qt runtime dependencies with windeployqt"
            VERBATIM)
    else()
        message(WARNING "windeployqt not found: automatic Windows runtime deployment is disabled.")
    endif()

    if(EXISTS "${CMAKE_SOURCE_DIR}/PVB_DB.db")
        add_custom_command(TARGET pb-safety POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${CMAKE_SOURCE_DIR}/PVB_DB.db"
                    "$<TARGET_FILE_DIR:pb-safety>/PVB_DB.db"
            COMMENT "Copying PVB_DB.db next to the executable"
            VERBATIM)
    else()
        message(WARNING "PVB_DB.db was not found in project root and will not be copied next to the executable.")
    endif()
endif()

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.pb-safety)
endif()

set_target_properties(pb-safety PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
)

include(GNUInstallDirs)

install(TARGETS pb-safety
    BUNDLE DESTINATION . COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
)

if(EXISTS "${CMAKE_SOURCE_DIR}/PVB_DB.db")
    if(APPLE)
        install(FILES "${CMAKE_SOURCE_DIR}/PVB_DB.db"
            DESTINATION "pb-safety.app/Contents/MacOS"
            COMPONENT Runtime)
    else()
        install(FILES "${CMAKE_SOURCE_DIR}/PVB_DB.db"
            DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT Runtime)
    endif()
endif()

set(CPACK_PACKAGE_NAME "pb-safety")
set(CPACK_PACKAGE_VENDOR "pb-safety")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_COMPONENTS_ALL Runtime)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(pb-safety)
endif()

if(APPLE AND PB_SAFETY_ENABLE_MACOS_DMG)
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_DMG_VOLUME_NAME "pb-safety")
endif()

include(CPack)
