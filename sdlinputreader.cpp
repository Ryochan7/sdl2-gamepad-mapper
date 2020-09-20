#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h>
//#include <QDebug>

#include "sdlinputreader.h"

SDLInputReader::SDLInputReader(QObject *parent) : QObject(parent)
{
    pollRateTimer.setParent(this);
}

void SDLInputReader::start()
{
    initSDL();

    pollRateTimer.setInterval(4);
    pollRateTimer.setTimerType(Qt::PreciseTimer);

    QObject::connect(&pollRateTimer, &QTimer::timeout, this, &SDLInputReader::runInputPass, Qt::DirectConnection);

    QMetaObject::invokeMethod(&pollRateTimer, "start", Qt::AutoConnection);
    //pollRateTimer.start();
}

void SDLInputReader::stop()
{
    //pollRateTimer.stop();
    QMetaObject::invokeMethod(&pollRateTimer, "stop", Qt::AutoConnection);

    closeSDL();
}

void SDLInputReader::initSDL()
{
    //qDebug() << "STARTING SDL";

    SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS);
    SDL_JoystickEventState(SDL_ENABLE);

    int numJoysticks = SDL_NumJoysticks();
    if (SDL_NumJoysticks() > 0)
    {
        joyContainer.clearJoysticks();
        //joyContainer.establishControllers(numJoysticks);

        for (int i = 0; i < numJoysticks; i++)
        {
            SDL_Joystick *joy = SDL_JoystickOpen(i);
            joyContainer.establishJoystick(i, joy);
        }

        SDL_JoystickUpdate();
        for (int i = 0; i < numJoysticks; i++)
        {
            JoystickSDL* tempJoy = joyContainer.getJoystick(i);
            tempJoy->refreshInitialStateData();
        }
    }

    sdlOpen = true;
}

void SDLInputReader::closeSDL()
{
    //qDebug() << "CLOSING SDL";

    SDL_Event event;
    event.type = SDL_QUIT;
    SDL_PushEvent(&event);

    // Clear any pending events
    while (SDL_PollEvent(&event) > 0)
    {
    }

    QHashIterator<int, JoystickSDL*> iter(*joyContainer.getJoysticks());
    while (iter.hasNext())
    {
        JoystickSDL* temp = iter.next().value();
        if (temp->HasGameControllerSupport() && SDL_GameControllerGetAttached(temp->getSDLGameController()))
        {
            SDL_GameControllerClose(temp->getSDLGameController());
        }

        if (SDL_JoystickGetAttached(temp->getSDLJoystick()))
        {
            SDL_JoystickClose(temp->getSDLJoystick());
        }
    }

    SDL_Quit();

    sdlOpen = false;
}

void SDLInputReader::runInputPass()
{
    pollRateTimer.stop();

    //qDebug() << "RUN RUN";

    if (!sdlOpen)
    {
        return;
    }

    SDL_Event event;
    while (SDL_PollEvent(&event) > 0)
    {
        //qDebug() << "EVENT NUM: " << event.type;
        switch (event.type)
        {
            case SDL_JOYBUTTONDOWN:
            case SDL_JOYBUTTONUP:
            {
                JoystickSDL *dev = joyContainer.getJoysticks()->value(event.jbutton.which);
                if (dev != nullptr)
                {
                    dev->setButtonState(event.jbutton.button, event.jbutton.state == SDL_PRESSED ? true : false);
                    //qDebug() << "GOD DANG IT BOBBY" << " " << event.jbutton.which << " " << event.jbutton.button;
                }

                break;
            }

            case SDL_JOYAXISMOTION:
            {
                JoystickSDL *dev = joyContainer.getJoysticks()->value(event.jaxis.which);
                if (dev != nullptr)
                {
                    dev->setAxisState(event.jaxis.axis, event.jaxis.value);
                    //qDebug() << "DOn't you know about the bird" << " " << event.jaxis.axis << " " << event.jaxis.value;
                }

                break;
            }
            case SDL_JOYHATMOTION:
            {
                JoystickSDL *dev = joyContainer.getJoysticks()->value(event.jhat.which);
                if (dev != nullptr)
                {
                    dev->setHatState(event.jhat.hat, event.jhat.value);
                }

                break;
            }
            case SDL_CONTROLLERBUTTONDOWN:
            case SDL_CONTROLLERBUTTONUP:
            {
                JoystickSDL *dev = joyContainer.getJoysticks()->value(event.cbutton.which);
                if (dev != nullptr)
                {
                    dev->setControllerButtonState(event.cbutton.button, event.cbutton.state == SDL_PRESSED ? true : false);
                    //qDebug() << "GOD DANG IT BOBBY" << " " << event.cbutton.which << " " << event.cbutton.button;
                }

                break;
            }
            case SDL_CONTROLLERAXISMOTION:
            {
                JoystickSDL *dev = joyContainer.getJoysticks()->value(event.caxis.which);
                if (dev != nullptr)
                {
                    dev->setControllerAxisState(event.caxis.axis, event.caxis.value);
                    //qDebug() << "DOn't you know about the bird" << " " << event.caxis.axis << " " << event.caxis.value;
                }
                break;
            }

            break;
        }
    }

    pollRateTimer.start();
}

JoystickContainer* SDLInputReader::getJoyContainer()
{
    return &joyContainer;
}
