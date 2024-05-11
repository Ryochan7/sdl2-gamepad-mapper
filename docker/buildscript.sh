#!/usr/bin/env bash

cd sdl2-gamepad-mapper
USE_DOCKER=1 QML_SOURCES_PATHS="${PWD}/sdl2mappercomponents" ./deploy_linux.sh
