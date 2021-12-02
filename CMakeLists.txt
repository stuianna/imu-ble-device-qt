cmake_minimum_required(VERSION 3.14)

project(imu-ble-app VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick Bluetooth REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Bluetooth REQUIRED)

set(PROJECT_SOURCES
        src/main.cpp
        src/worker.cpp
        src/ble/device_scanner.cpp
        src/ble/imu_device.cpp
        src/ble/notification_service.cpp
        qml/qml.qrc

        # https://github.com/bincrafters/community/issues/466#issuecomment-426265654
        # Header files that require QOBJECT should be added to the executable sources,
        # so that they are parsed by auto-moc. Otherwise a bunch of link-time errors occur.
        ${CMAKE_SOURCE_DIR}/include/ble/device_scanner.hpp
        ${CMAKE_SOURCE_DIR}/include/ble/imu_device.hpp
        ${CMAKE_SOURCE_DIR}/include/ble/notification_service.hpp
        ${CMAKE_SOURCE_DIR}/include/worker.hpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(imu-ble-app
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET imu-ble-app APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(imu-ble-app SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(imu-ble-app
          ${PROJECT_SOURCES}
        )
    endif()
endif()

target_compile_definitions(imu-ble-app
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(imu-ble-app
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Bluetooth)

target_include_directories(imu-ble-app PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/imu-ble-device-shared/include
)

set_target_properties(imu-ble-app PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(imu-ble-app)
    qt_finalize_executable(imu-ble-app)
endif()
