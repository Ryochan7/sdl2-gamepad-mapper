//#include <QDebug>
#include <QtGlobal>
#include <QListIterator>

#include <SDL2/SDL_gamecontroller.h>

#include "joysticksdl.h"

JoystickSDL::JoystickSDL(int index, SDL_Joystick *joy, QObject *parent) : QObject(parent)
{
    sdlJoy = joy;
    this->index = index;
    parseSDLDetails();

    refreshGameController();
}

void JoystickSDL::parseSDLDetails()
{
    SDL_JoystickGUID tempGUID = SDL_JoystickGetGUID(sdlJoy);
    char guidString[65] = {'0'};
    SDL_JoystickGetGUIDString(tempGUID, guidString, sizeof(guidString));
    sdlGUID = QString(guidString);

    sdlJoyID = SDL_JoystickInstanceID(sdlJoy);

    char* temp = SDL_GameControllerMappingForGUID(tempGUID);
    if (temp != nullptr)
    {
        mappingString = QString::fromLocal8Bit(temp).trimmed();
        mappingAvailable = true;
    }

    deviceName = QString::fromLocal8Bit(SDL_JoystickName(sdlJoy));
    const char *name = SDL_GameControllerNameForIndex(index);
    if (name != nullptr)
    {
        mappedName = QString::fromLocal8Bit(name);
    }
    else
    {
        mappedName = deviceName;
    }

    gameController = SDL_IsGameController(index);
    //refreshGameController();

    for (int i = 0; i < SDL_JoystickNumButtons(sdlJoy); i++)
    {
        buttonStates.append(SDL_JoystickGetButton(sdlJoy, i));
    }

    for (int i = 0; i < SDL_JoystickNumAxes(sdlJoy); i++)
    {
        AxisInfo* tempInfo = new AxisInfo();
        tempInfo->axisValue = SDL_JoystickGetAxis(sdlJoy, i);
        axisStates.append(tempInfo);
        //axisStates.append(SDL_JoystickGetAxis(sdlJoy, i));
        //isTrigger.append(false);
    }

    for (int i = 0; i < SDL_JoystickNumHats(sdlJoy); i++)
    {
        hatStates.append(SDL_JoystickGetHat(sdlJoy, i));
    }
}

void JoystickSDL::refreshInitialStateData()
{
    for (int i = 0; i < SDL_JoystickNumButtons(sdlJoy); i++)
    {
        buttonStates[i] = SDL_JoystickGetButton(sdlJoy, i);
    }

    /*for (int i = 0; i < SDL_JoystickNumButtons(sdlJoy); i++)
    {
        int16_t current = 0;
        bool hasInitialState = SDL_JoystickGetAxisInitialState(sdlJoy, i, &current);
        qDebug() << "HAS INITIAL: " << hasInitialState;
        int16_t fucker = SDL_JoystickGetAxis(sdlJoy, i);
        qDebug() << "FUCKER: " << fucker;
        if (hasInitialState && qAbs(current) >= SLACK_RANGE)
        {
            isTrigger[i] = true;
            qDebug() << "FOUND TRIGGER: " << i;
        }
        else
        {
            qDebug() << "NOT TRIGGER: " << i << " " << current;
            isTrigger[i] = false;
        }
    }
    */

    for (int i = 0; i < SDL_JoystickNumAxes(sdlJoy); i++)
    {
        axisStates[i]->axisValue = SDL_JoystickGetAxis(sdlJoy, i);
    }

    for (int i = 0; i < SDL_JoystickNumHats(sdlJoy); i++)
    {
        hatStates[i] = SDL_JoystickGetHat(sdlJoy, i);
    }
}

void JoystickSDL::refreshGameController()
{
    gameController = SDL_IsGameController(index);
    if (gameController && !mappedGameController)
    {
        mappedGameController = SDL_GameControllerOpen(index);

        for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
        {
            controllerButtonStates.append(SDL_GameControllerGetButton(mappedGameController, static_cast<SDL_GameControllerButton>(i)));
        }

        for (int i = 0; i < SDL_CONTROLLER_AXIS_MAX; i++)
        {
            AxisInfo* tempInfo = new AxisInfo();
            tempInfo->axisValue = SDL_GameControllerGetAxis(mappedGameController, static_cast<SDL_GameControllerAxis>(i));
            controllerAxisStates.append(tempInfo);
        }
    }
}

SDL_Joystick* JoystickSDL::getSDLJoystick()
{
    return sdlJoy;
}

SDL_GameController* JoystickSDL::getSDLGameController()
{
    return mappedGameController;
}

SDL_JoystickID JoystickSDL::getSDLJoystickID()
{
    return sdlJoyID;
}

void JoystickSDL::setButtonState(int index, bool state)
{
    if (index >= 0 && index < buttonStates.count())
    {
        bool current = buttonStates.at(index);
        if (current != state)
        {
            buttonStates[index] = state;
            emit buttonEvent(index, state);
        }
    }

}

void JoystickSDL::setAxisState(int index, int value)
{
    if (index >= 0 && index < axisStates.count())
    {
        AxisInfo* current = axisStates.at(index);
        //int current = axisStates.at(index);
        if (current->axisValue != value)
        {
            if (current->firstEvent)
            {
                current->firstEvent = false;
                if (value <= INITIAL_TRIGGER_CHECK)
                {
                    //qDebug() << "THE IBLIS TRIGGER: " << index;
                    current->isTrigger = true;
                }
            }

            current->axisValue = value;
            emit axisEvent(index, value);
        }
    }
}

void JoystickSDL::setHatState(int index, int value)
{
    if (index >= 0 && index < hatStates.count())
    {
        int current = hatStates.at(index);
        if (current != value)
        {
            hatStates[index] = qBound(0, value, 12);
            emit hatEvent(index, value);
        }
    }
}

void JoystickSDL::setControllerButtonState(int index, bool state)
{
    if (index >= 0 && index < controllerButtonStates.count())
    {
        bool current = controllerButtonStates.at(index);
        if (current != state)
        {
            controllerButtonStates[index] = state;
            emit gameControllerButtonEvent(index, state);
        }
    }

}

void JoystickSDL::setControllerAxisState(int index, int value)
{
    if (index >= 0 && index < controllerAxisStates.count())
    {
        AxisInfo* current = controllerAxisStates.at(index);
        //int current = axisStates.at(index);
        if (current->axisValue != value)
        {
            if (current->firstEvent)
            {
                current->firstEvent = false;
                if (value <= INITIAL_TRIGGER_CHECK)
                {
                    //qDebug() << "THE IBLIS TRIGGER: " << index;
                    current->isTrigger = true;
                }
            }

            current->axisValue = value;
            emit gameControllerAxisEvent(index, value);
        }
    }

}

bool JoystickSDL::setMappingString(QString mapping)
{
    bool result = false;
    std::string tempMap = mapping.toStdString();
    int mappingResult = SDL_GameControllerAddMapping(tempMap.c_str());
    if (mappingResult >= 0)
    {
        mappingString = mapping.trimmed();
        mappingAvailable = true;
        result = true;
    }

    return result;
}

QString JoystickSDL::getMappingString()
{
    return mappingString;
}

QString JoystickSDL::getDeviceName()
{
    return deviceName;
}

QString JoystickSDL::getDeviceGUID()
{
    return sdlGUID;
}

QString JoystickSDL::getPlatformName()
{
    return QString(SDL_GetPlatform());
}

QString JoystickSDL::getMappedName()
{
    return mappedName;
}

int JoystickSDL::getJoyIndex()
{
    return index;
}

int JoystickSDL::getJoyNumButtons()
{
    return buttonStates.size();
}

int JoystickSDL::getJoyNumAxes()
{
    return axisStates.size();
}

int JoystickSDL::getJoyNumHats()
{
    return hatStates.size();
}

void JoystickSDL::setMappedName(QString name)
{
    if (mappedName != name)
    {
        mappedName = name;
        emit mappedNameChanged();
    }
}

bool JoystickSDL::isMappingAvailable()
{
    return mappingAvailable;
}

bool JoystickSDL::isAxisTrigger(int index)
{
    bool result = false;
    if (index >= 0 && index < axisStates.count())
    {
        result = axisStates.at(index)->isTrigger;
    }

    return result;
}

bool JoystickSDL::HasGameControllerSupport()
{
    return gameController;
}

JoystickSDL::~JoystickSDL()
{
    QListIterator<AxisInfo*> iter(axisStates);
    while (iter.hasNext())
    {
        AxisInfo* currentInfo = iter.next();
        delete currentInfo;
        currentInfo = nullptr;
    }

    axisStates.clear();

    /*if (gameController && SDL_GameControllerGetAttached(mappedGameController))
    {
        SDL_GameControllerClose(mappedGameController);
    }

    mappedGameController = nullptr;

    if (SDL_JoystickGetAttached(sdlJoy))
    {
        SDL_JoystickClose(sdlJoy);
    }
    */

    mappedGameController = nullptr;
    sdlJoy = nullptr;
}
