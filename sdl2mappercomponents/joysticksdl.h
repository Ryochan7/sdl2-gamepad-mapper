#ifndef JOYSTICKSDL_H
#define JOYSTICKSDL_H

#include <QObject>
#include <QString>
#include <QList>
#include <QQmlEngine>

#include <SDL2/SDL_joystick.h>
#include <SDL2/SDL_gamecontroller.h>


class JoystickSDL : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
    Q_PROPERTY(QString GUID MEMBER sdlGUID CONSTANT)
    Q_PROPERTY(QString gameContMapping READ getMappingString)
    Q_PROPERTY(QString gameContDevName READ getDeviceName CONSTANT)
    Q_PROPERTY(QString gameMappedName READ getMappedName WRITE setMappedName NOTIFY mappedNameChanged)
    Q_PROPERTY(bool isGameController MEMBER gameController)

    struct AxisInfo
    {
        int index = 0;
        short axisValue = 0;
        bool firstEvent = true;
        bool isTrigger = false;
    };

public:
    enum DPadDir
    {
        Centered = SDL_HAT_CENTERED,
        Up = SDL_HAT_UP,
        Right = SDL_HAT_RIGHT,
        Down = SDL_HAT_DOWN,
        Left = SDL_HAT_LEFT,

        UpRight = SDL_HAT_UP | SDL_HAT_RIGHT,
        DownRight = SDL_HAT_DOWN | SDL_HAT_RIGHT,
        DownLeft = SDL_HAT_DOWN | SDL_HAT_LEFT,
        UpLeft = SDL_HAT_UP | SDL_HAT_LEFT,
    };

    explicit JoystickSDL(int index, SDL_Joystick* joy, QObject *parent = nullptr);
    ~JoystickSDL();

    SDL_Joystick* getSDLJoystick();
    SDL_GameController* getSDLGameController();
    SDL_JoystickID getSDLJoystickID();
    void setButtonState(int index, bool state);
    void setAxisState(int index, int value);
    void setControllerButtonState(int index, bool state);
    void setControllerAxisState(int index, int value);
    void setHatState(int index, int value);
    Q_INVOKABLE bool setMappingString(QString mapping);
    QString getMappingString();
    QString getDeviceName();
    QString getDeviceGUID();
    QString getMappedName();
    QString getPlatformName();
    int getJoyIndex();
    int getJoyNumButtons();
    int getJoyNumAxes();
    int getJoyNumHats();
    void setMappedName(QString name);
    bool isMappingAvailable();
    bool HasGameControllerSupport();
    bool isAxisTrigger(int index);
    void refreshInitialStateData();
    Q_INVOKABLE void refreshGameController();
    void closeDevice();

    static const int SLACK_RANGE = 20000;
    static const int INITIAL_TRIGGER_CHECK = -20000;

private:
    void parseSDLDetails();

    SDL_Joystick *sdlJoy = nullptr;
    SDL_GameController* mappedGameController = nullptr;
    QString mappedName;
    bool mappingAvailable;
    QString mappingString;
    QString sdlGUID;
    QString deviceName;
    int index;
    SDL_JoystickID sdlJoyID = 0;
    bool gameController;
    QList<bool> buttonStates;
    QList<AxisInfo*> axisStates;
    QList<int> hatStates;

    QList<bool> controllerButtonStates;
    QList<AxisInfo*> controllerAxisStates;


signals:
    void buttonEvent(int index, bool state);
    void axisEvent(int index, int value);
    void hatEvent(int index, int value);

    void gameControllerButtonEvent(int index, bool state);
    void gameControllerAxisEvent(int index, int value);
    void mappedNameChanged();
};

//Q_DECLARE_METATYPE(JoystickSDL*)
//QML_DECLARE_TYPE(JoystickSDL)

#endif // JOYSTICKSDL_H
