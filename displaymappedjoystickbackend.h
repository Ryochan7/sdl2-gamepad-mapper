#ifndef DISPLAYMAPPEDJOYSTICKBACKEND_H
#define DISPLAYMAPPEDJOYSTICKBACKEND_H

#include <QObject>
#include <QList>

#include <SDL2/SDL_gamecontroller.h>

#include "joysticksdl.h"

class DisplayMappedJoystickBackend : public QObject
{
    Q_OBJECT

    struct ControllerAxisInfo
    {
        int index = 0;
        short axisValue = 0;
        short highValue = 0;
        bool isActive = false;

        ControllerAxisInfo(int index)
        {
            this->index = index;
        }
    };

public:
    explicit DisplayMappedJoystickBackend(QObject *parent = nullptr);
    ~DisplayMappedJoystickBackend();
    Q_INVOKABLE void setActiveDevice(JoystickSDL* device);
    Q_INVOKABLE bool establishConnections();
    Q_INVOKABLE bool disconnectConnections();

    static const int EMPTY_TRACKING_NUM = -1;
    static const int TRACK_AXIS_THRESHOLD = 28000;
    static const int TRACK_AXIS_DEAD = 18000;
    static const int TRACK_AXIS_DIFF_THRESHOLD = 10000;
    static const int MAX_NUMBER_BINDINGS = 25;

private:
    JoystickSDL* joyDevice;
    QList<bool> controllerButtonsStates;
    QList<ControllerAxisInfo*> controllerAxisStates;


signals:
    void bindExecuted(int bindIndex);
    void bindReleased(int bindIndex);

public slots:
    void handleButtonEvent(int index, bool state);
    void handleAxisEvent(int index, int value);

};

#endif // DISPLAYMAPPEDJOYSTICKBACKEND_H
