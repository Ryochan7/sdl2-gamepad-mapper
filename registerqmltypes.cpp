#include <QQmlEngine>

#include "registerqmltypes.h"

/*#include "sdl2mapperbackend.h"
#include "sdlinputreader.h"
#include "mainviewbackend.h"
#include "mappinggcviewbackend.h"
#include "displaymappedjoystickbackend.h"
#include "applogger.h"
*/
#include "sdl2mappercomponents/joysticksdl.h"

void RegisterQMLTypes::RegisterTypesForQML()
{
    /*qmlRegisterType<SDL2MapperBackend>("sdl2mappercomponents", 1, 0, "SDL2MapperBackend");
    qmlRegisterType<SDLInputReader>("sdl2mappercomponents", 1, 0, "SDLInputReader");
    qmlRegisterType<JoystickContainer>("sdl2mappercomponents", 1, 0, "JoystickContainer");
    qmlRegisterType<MainViewBackend>("sdl2mappercomponents", 1, 0, "MainViewBackend");
    qmlRegisterType<MappingGCViewBackend>("sdl2mappercomponents", 1, 0, "MappingGCViewBackend");
    qmlRegisterType<DisplayMappedJoystickBackend>("sdl2mappercomponents", 1, 0, "DisplayMappedJoystickBackend");
    qmlRegisterType<AppLogger>("sdl2mappercomponents", 1, 0, "AppLogger");
    */
    //qRegisterMetaType<JoystickSDL*>("JoystickSDL*");
}
