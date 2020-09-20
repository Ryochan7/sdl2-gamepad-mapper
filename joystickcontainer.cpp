#include <QHashIterator>
#include <QMapIterator>

#include "joystickcontainer.h"

JoystickContainer::JoystickContainer(QObject *parent) : QObject(parent)
{
    joysticks = new QHash<int, JoystickSDL*>();
    tempJoyList = new QList<JoystickSDL*>();
}

void JoystickContainer::clearJoysticks()
{
    QHashIterator<int, JoystickSDL*> iter(*joysticks);
    while (iter.hasNext())
    {
        iter.next();
        JoystickSDL* joy = iter.value();
        delete joy;
        joy = nullptr;
    }

    joysticks->clear();
    emit numPadsChanged(0);
}

void JoystickContainer::establishJoystick(int index, SDL_Joystick *joy)
{
    JoystickSDL* tempJoy = new JoystickSDL(index, joy, this);
    SDL_JoystickID sdlJoyId = static_cast<SDL_JoystickID>(tempJoy->getSDLJoystickID());
    joysticks->insert(sdlJoyId, tempJoy);
    emit numPadsChanged(joysticks->count());
}

QHash<int, JoystickSDL*>* JoystickContainer::getJoysticks()
{
    return joysticks;
}

int JoystickContainer::getNumberJoysticks()
{
    return joysticks->count();
}

JoystickSDL* JoystickContainer::getJoystick(int index)
{
    return joysticks->value(index);
}

JoystickContainer::~JoystickContainer()
{
    tempJoyList->clear();
    delete tempJoyList;

    QHashIterator<int, JoystickSDL*> iter(*joysticks);
    while (iter.hasNext())
    {
        iter.next();
        JoystickSDL* joy = iter.value();
        delete joy;
        joy = nullptr;
    }

    joysticks->clear();
    delete joysticks;
}
