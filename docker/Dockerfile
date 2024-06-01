# Requires downloading appimagetool-x86_64.AppImage,
# linuxdeploy-x86_64.AppImage, and
# linuxdeploy-plugin-qt-x86_64.AppImage prior.
# Placed here for reference. Cannot build Docker image
# from this directory.
#
# Create host volume to expose final AppImage to host system

# Using Fedora 37 as base system
FROM fedora:37

# Install base
RUN dnf -y install gcc gcc-c++ git qt6-qtbase qt6-qtbase-devel qt6-qtdeclarative qt6-qtdeclarative-devel qt6-qttranslations qt6-linguist SDL2-devel fuse-libs file

# Copy required files
RUN mkdir app
COPY appimagetool-x86_64.AppImage /usr/local/bin
COPY linuxdeploy-x86_64.AppImage /usr/local/bin
COPY linuxdeploy-plugin-qt-x86_64.AppImage /usr/local/bin
ADD sdl2-gamepad-mapper app/sdl2-gamepad-mapper
ADD buildscript.sh app

# Set working dir
WORKDIR app
# Make symlink for qmake
RUN ln -sf /usr/bin/qmake6 /usr/bin/qmake
#ENTRYPOINT /app/buildscript.sh
