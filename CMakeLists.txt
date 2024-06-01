cmake_minimum_required(VERSION 3.16)

project(sdl2-gamepad-mapper VERSION 0.0.8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick QuickControls2 Network Gui)

qt_standard_project_setup()

if (WIN32)
    set(QT_PATH ${CMAKE_PREFIX_PATH}/../..)
endif()

if (!WIN32)
    find_package(SDL2 REQUIRED)
endif()

qt_add_executable(sdl2-gamepad-mapper
    main.cpp
    sdl2-gamepad-mapper_en_US.ts
)

qt_add_resources(sdl2-gamepad-mapper "qml"
    PREFIX "/"
    FILES
        main.qml
)

qt_add_resources(sdl2-gamepad-mapper "resources"
    PREFIX "/"
    FILES
        images/axis.png
        images/button.png
        images/controllermap.png
        images/down-arrow-small.png
        images/left-arrow-small.png
        images/right-arrow-small.png
        images/up-arrow-small.png
)

qt_add_qml_module(sdl2-gamepad-mapper
    URI sdl2mappercomponents
    VERSION 1.0
    SOURCES
        sdl2mappercomponents/applogger.cpp
        sdl2mappercomponents/applogger.h
        sdl2mappercomponents/apploggersingleton.cpp
        sdl2mappercomponents/apploggersingleton.h
        sdl2mappercomponents/displaymappedjoystickbackend.cpp
        sdl2mappercomponents/displaymappedjoystickbackend.h
        sdl2mappercomponents/joystickcontainer.cpp
        sdl2mappercomponents/joystickcontainer.h
        sdl2mappercomponents/joystickcontainermodel.cpp
        sdl2mappercomponents/joystickcontainermodel.h
        sdl2mappercomponents/joysticksdl.cpp
        sdl2mappercomponents/joysticksdl.h
        sdl2mappercomponents/mainviewbackend.cpp
        sdl2mappercomponents/mainviewbackend.h
        sdl2mappercomponents/mappinggcviewbackend.cpp
        sdl2mappercomponents/mappinggcviewbackend.h
        sdl2mappercomponents/sdl2mapperbackend.cpp
        sdl2mappercomponents/sdl2mapperbackend.h
        sdl2mappercomponents/sdlinputreader.cpp
        sdl2mappercomponents/sdlinputreader.h
    QML_FILES
        sdl2mappercomponents/DisplayMappedJoystickItem.qml
        sdl2mappercomponents/MappingGCItem.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(sdl2-gamepad-mapper PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appsdl2-gamepad-mapper-cmake
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(sdl2-gamepad-mapper
    PRIVATE Qt6::Quick
    PRIVATE Qt6::QuickControls2
    PRIVATE Qt6::Network
    PRIVATE Qt6::Gui
)

include_directories(${CMAKE_SOURCE_DIR}/sdl2mappercomponents)

if (WIN32)
    #message("IN WIN32")
    target_link_libraries(sdl2-gamepad-mapper
       PUBLIC -L${CMAKE_SOURCE_DIR}/SDL2/lib/
       PUBLIC -llibSDL2
       PUBLIC -L${QT_PATH}/Tools/OpenSSL/Win_x64/lib
       PUBLIC -llibssl
       PUBLIC -llibcrypto
    )
    include_directories(${CMAKE_SOURCE_DIR}/SDL2/include/)
    target_include_directories(sdl2-gamepad-mapper
        PUBLIC "${QT_PATH}/Tools/OpenSSL/Win_x64/include"
    )
else()
    target_link_libraries(sdl2-gamepad-mapper
        PUBLIC SDL2
    )
endif()

include(GNUInstallDirs)
install(TARGETS sdl2-gamepad-mapper
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
