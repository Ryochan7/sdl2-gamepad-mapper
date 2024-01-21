# Requires downloading appimagetool-x86_64.AppImage and
# linuxdeployqt-continuous-x86_64.AppImage prior.
# Placed here for reference. Cannot build Docker image
# from this directory.
#
# Create host volume to expose final AppImage to host system

# Using Fedora 35 as base system
FROM fedora:35

# Install base
RUN dnf -y install gcc gcc-c++ git qt5-qtbase qt5-qtbase-devel qt5-qtdeclarative qt5-qtdeclarative-devel qt5-qtquickcontrols2 qt5-qtquickcontrols2-devel qt5-qtquickcontrols qt5-qttranslations qt5-linguist SDL2-devel fuse-libs file

# Copy required files
RUN mkdir app
COPY appimagetool-x86_64.AppImage /usr/local/bin
COPY linuxdeployqt-continuous-x86_64.AppImage /usr/local/bin
ADD sdl2-gamepad-mapper app/sdl2-gamepad-mapper
ADD buildscript.sh app

# Set working dir
WORKDIR app
# Make symlink for qmake
RUN ln -sf /usr/bin/qmake-qt5 /usr/bin/qmake
#ENTRYPOINT /app/buildscript.sh
