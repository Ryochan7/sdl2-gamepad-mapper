#ifndef MAPPINGGCVIEWBACKEND_H
#define MAPPINGGCVIEWBACKEND_H

#include <QObject>
#include <QString>
#include <QList>
#include <QQmlEngine>

#include <SDL2/SDL_gamecontroller.h>

#include "joysticksdl.h"

class MappingGCViewBackend : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString cuckery MEMBER HIGH_CUCKERY CONSTANT)
    Q_PROPERTY(int currentMapIndex MEMBER mappingIndex)
    Q_PROPERTY(int sdlMaxButtons READ getSDLMaxButtons)
    Q_PROPERTY(int sdlMaxAxes READ getSDLMaxAxes)

    enum InputBindType
    {
        None,
        Button,
        Axis,
        AxisDir,
        Hat,
    };

    enum AxisDirType
    {
        AxisFull,
        AxisNeg,
        AxisPos,
    };

    struct GCBinding
    {
        int index = 0;
        QString bindName;
        InputBindType bindType = None;
        union
        {
            int button;
            struct
            {
                int axis;
                AxisDirType dir;
                bool invert;
                int base_value;
                int max_value;
            } axis;
            //int axis;
            struct
            {
                int hat;
                int hat_mask;
            } dpad;
        } bindData;

        int outputBind = 0;

        public:
            GCBinding()
            {
                index = 0;
                bindType = None;
                bindData = {0};
            }

            GCBinding(int index, QString name, InputBindType type, int outputBind)
            {
                this->index = index;
                this->bindName = name;
                this->bindType = type;
                bindData = {0};
                this->outputBind = outputBind;
            }

            void resetBind()
            {
                bindType = None;
                bindData = {0};
                outputBind = 0;
            }
    };

    struct InputAxisInfo
    {
        int index = 0;
        short axisValue = 0;
        short highValue = 0;
        bool isActive = false;

        InputAxisInfo(int index)
        {
            this->index = index;
        }
    };

    enum
    {
        SDL_CONTROLLER_BIND_AXIS_LEFTX_NEGATIVE = SDL_CONTROLLER_BUTTON_MAX,
        SDL_CONTROLLER_BIND_AXIS_LEFTX_POSITIVE,
        SDL_CONTROLLER_BIND_AXIS_LEFTY_NEGATIVE,
        SDL_CONTROLLER_BIND_AXIS_LEFTY_POSITIVE,
        SDL_CONTROLLER_BIND_AXIS_RIGHTX_NEGATIVE,
        SDL_CONTROLLER_BIND_AXIS_RIGHTX_POSITIVE,
        SDL_CONTROLLER_BIND_AXIS_RIGHTY_NEGATIVE,
        SDL_CONTROLLER_BIND_AXIS_RIGHTY_POSITIVE,
        SDL_CONTROLLER_BIND_AXIS_TRIGGERLEFT,
        SDL_CONTROLLER_BIND_AXIS_TRIGGERRIGHT,
    };

public:
    explicit MappingGCViewBackend(QObject *parent = nullptr);
    ~MappingGCViewBackend();
    Q_INVOKABLE void setActiveDevice(JoystickSDL* device);
    Q_INVOKABLE bool beginTracking();
    Q_INVOKABLE bool endTracking();
    bool establishConnections();
    bool disconnectConnections();
    Q_INVOKABLE QString generateSDLBindings();
    Q_INVOKABLE void resetMappings();
    Q_INVOKABLE void resetBinding(int index);
    int getSDLMaxButtons();
    int getSDLMaxAxes();

    static const QString HIGH_CUCKERY;
    static const int EMPTY_TRACKING_NUM = -1;
    static const int TRACK_AXIS_THRESHOLD = 28000;
    static const int TRACK_AXIS_DEAD = 18000;
    static const int TRACK_AXIS_DIFF_THRESHOLD = 10000;
    static const int MAX_NUMBER_BINDINGS = 25;

private:
    void createBindingData();
    bool skipAdjacentAxisMapping(int index, int outBindIdx);
    int rangeValue(int current);
    void refreshTrackedAxes();
    void deleteTrackedAxes();

    JoystickSDL* joyDevice;
    int trackingButton = EMPTY_TRACKING_NUM;
    int trackingAxis = EMPTY_TRACKING_NUM;
    int trackedAxisvalue = 0;
    bool trackingTrigger = false;
    int trackingHat = EMPTY_TRACKING_NUM;
    int trackingHatValue = 0;
    int mappingIndex = 0;
    QList<GCBinding*> mappedBindings;
    QList<InputAxisInfo*> trackedAxesStates;
    GCBinding* leftXNeg = nullptr;
    GCBinding* leftXPos = nullptr;
    GCBinding* leftYNeg = nullptr;
    GCBinding* leftYPos = nullptr;
    GCBinding* rightXNeg = nullptr;
    GCBinding* rightXPos = nullptr;
    GCBinding* rightYNeg = nullptr;
    GCBinding* rightYPos = nullptr;


signals:
    void bindingAssignned();
    void currentMapIndexChanged();

public slots:
    void handleButtonEvent(int index, bool state);
    void handleAxisEvent(int index, int value);
    void handleHatEvent(int index, int value);

private slots:
    void emptyCurrentTracked();
};

#endif // MAPPINGGCVIEWBACKEND_H
