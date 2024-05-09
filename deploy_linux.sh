#!/usr/bin/env bash

# Might need to include <QtDir>/bin in PATH if using a Qt installation
# not installed system wide. Also will likely need to define QMAKE to
# point to proper qmake version

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

if [ -z "$QTBUNDLEDIR" ]; then
  echo "Need to give directory for pre-built Qt version being used (\$QTBUNDLEDIR)."
  exit 1
fi

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
  AppImage/usr/{bin,lib,share/applications,icons/hicolor/512x512/apps}
cp build/sdl2-gamepad-mapper AppImage/usr/bin
cp sdl2-gamepad-mapper.desktop AppImage/usr/share/applications
cp sdl2-gamepad-mapper.png AppImage/usr/icons/hicolor/512x512/apps
cp sdl2-gamepad-mapper.desktop sdl2-gamepad-mapper.png \
  AppImage

# Need to copy needed QML files for appimage here
mkdir -p AppImage/usr/qml
cp -r "${QTBUNDLEDIR}/qml/QtQml" AppImage/usr/qml
cp -r "${QTBUNDLEDIR}/qml/QtQuick" AppImage/usr/qml
cp -r "${QTBUNDLEDIR}/qml/QtWayland" AppImage/usr/qml
cp "${QTBUNDLEDIR}/qml/builtins.qmltypes" AppImage/usr/qml
cp "${QTBUNDLEDIR}/qml/jsroot.qmltypes" AppImage/usr/qml

# Change dir to AppImage root dir
#cd AppImage

# Discover and bundle Qt dependencies. Build appimage file
#linuxdeployqt-continuous-x86_64.AppImage AppImage/usr/bin/sdl2-gamepad-mapper \
# -qmldir="." -appimage -unsupported-allow-new-glibc

# Discover and bundle Qt dependencies. Build appimage file.
# Need linuxdeploy-plugin-qt and linuxdeploy-plugin-appimage as well
linuxdeploy-x86_64.AppImage --appdir AppImage --plugin qt \
  --icon-file="AppImage/sdl2-gamepad-mapper.png" --create-desktop-file \
  --executable="AppImage/usr/bin/sdl2-gamepad-mapper" --output appimage
