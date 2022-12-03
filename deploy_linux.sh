#!/usr/bin/env bash

# Might need to include <QtDir>/bin in PATH if using a Qt installation
# not installed system wide

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

# Change dir to script dir
cd $SCRIPT_DIR
#echo $SCRIPT_DIR

# Build app in dedicated build dir
mkdir build
cd build
qmake ../ CONFIG+=release
make

# Change dir back to main source dir
cd $SCRIPT_DIR

# Copy files for appimage structure
mkdir AppImage
mkdir -p \
  AppImage/usr/{bin,lib,share/applications,icons/hicolor/}
cp build/sdl2-gamepad-mapper AppImage/usr/bin
cp sdl2-gamepad-mapper.desktop AppImage/usr/share/applications
cp sdl2-gamepad-mapper.png AppImage/usr/icons/hicolor
cp sdl2-gamepad-mapper.desktop sdl2-gamepad-mapper.png \
  AppImage

# Change dir to AppImage root dir
#cd AppImage

# Discover and bundle Qt dependencies. Build appimage file
linuxdeployqt-continuous-x86_64.AppImage AppImage/usr/bin/sdl2-gamepad-mapper \
 -qmldir="." -appimage -unsupported-allow-new-glibc
