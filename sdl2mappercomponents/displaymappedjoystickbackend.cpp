#include <QListIterator>

#include "SDL2/SDL_gamecontroller.h"

#include "displaymappedjoystickbackend.h"

DisplayMappedJoystickBackend::DisplayMappedJoystickBackend(QObject *parent) : QObject(parent)
{
    joyDevice = 0;
    for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
    {
        controllerButtonsStates.append(false);
    }

    for (int i = 0; i < SDL_CONTROLLER_AXIS_MAX; i++)
    {
        controllerAxisStates.append(new ControllerAxisInfo(i));
    }
}

void DisplayMappedJoystickBackend::setActiveDevice(JoystickSDL* device)
{
    if (joyDevice)
    {
        disconnectConnections();
    }

    this->joyDevice = device;
}

bool DisplayMappedJoystickBackend::establishConnections()
{
    bool result = false;

    if (joyDevice)
    {
        connect(joyDevice, &JoystickSDL::gameControllerButtonEvent, this, &DisplayMappedJoystickBackend::handleButtonEvent);
        connect(joyDevice, &JoystickSDL::gameControllerAxisEvent, this, &DisplayMappedJoystickBackend::handleAxisEvent);

        result = true;
    }

    return result;
}

bool DisplayMappedJoystickBackend::disconnectConnections()
{
    bool result = false;

    if (joyDevice)
    {
        disconnect(joyDevice, &JoystickSDL::gameControllerButtonEvent, this, nullptr);
        disconnect(joyDevice, &JoystickSDL::gameControllerAxisEvent, this, nullptr);

        result = true;
    }

    return result;
}

void DisplayMappedJoystickBackend::handleButtonEvent(int index, bool state)
{
    if (state)
    {
        controllerButtonsStates[index] = state;
        emit bindExecuted(index);
    }
    else
    {
        controllerButtonsStates[index] = state;
        emit bindReleased(index);
    }
}

void DisplayMappedJoystickBackend::handleAxisEvent(int index, int value)
{
    bool isTtrigger = index == SDL_CONTROLLER_AXIS_TRIGGERLEFT ||
                       index == SDL_CONTROLLER_AXIS_TRIGGERRIGHT;

    ControllerAxisInfo* tempInfo = controllerAxisStates.at(index);
    tempInfo->axisValue = value;
    if (!isTtrigger)
    {
        // Check for release threshold first. Skips past deadzone skip
        if (tempInfo->isActive &&
            qAbs(tempInfo->axisValue - tempInfo->highValue) >= TRACK_AXIS_DIFF_THRESHOLD)
        {
            tempInfo->isActive = false;
            tempInfo->highValue = 0;

            bool negative = value < 0;
            int bindAxis = (2 * index) + (negative ? 0 : 1) + SDL_CONTROLLER_BUTTON_MAX;
            emit bindReleased(bindAxis);
        }

        // Check if range went past axis threshold
        if (!tempInfo->isActive && qAbs(value) >= TRACK_AXIS_THRESHOLD)
        {
            tempInfo->isActive = true;
            tempInfo->highValue = value;

            bool negative = value < 0;
            int bindAxis = (2 * index) + (negative ? 0 : 1) + SDL_CONTROLLER_BUTTON_MAX;
            emit bindExecuted(bindAxis);
        }
    }
    else
    {
        if (!tempInfo->isActive && value >= TRACK_AXIS_THRESHOLD)
        {
            tempInfo->isActive = true;
            tempInfo->highValue = value;

            int bindAxis = (2 * SDL_CONTROLLER_AXIS_RIGHTY + 2) + SDL_CONTROLLER_BUTTON_MAX +
                    (index - SDL_CONTROLLER_AXIS_TRIGGERLEFT);
            emit bindExecuted(bindAxis);
        }
        else if (tempInfo->isActive && value <= TRACK_AXIS_DEAD)
        {
            tempInfo->isActive = false;
            tempInfo->highValue = 0;

            int bindAxis = (2 * SDL_CONTROLLER_AXIS_RIGHTY + 2) + SDL_CONTROLLER_BUTTON_MAX +
                    (index - SDL_CONTROLLER_AXIS_TRIGGERLEFT);
            emit bindReleased(bindAxis);
        }
    }
}

int DisplayMappedJoystickBackend::getSDLMaxButtons()
{
    return SDL_CONTROLLER_BUTTON_MAX;
}

int DisplayMappedJoystickBackend::getSDLMaxAxes()
{
    return SDL_CONTROLLER_AXIS_MAX;
}

DisplayMappedJoystickBackend::~DisplayMappedJoystickBackend()
{
    QListIterator<ControllerAxisInfo*> iter(controllerAxisStates);
    while (iter.hasNext())
    {
        ControllerAxisInfo* tempInfo = iter.next();
        delete tempInfo;
        tempInfo = nullptr;
    }

    controllerAxisStates.clear();
}
