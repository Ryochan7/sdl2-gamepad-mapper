#ifndef JOYSTICKCONTAINER_H
#define JOYSTICKCONTAINER_H

#include <QObject>
#include <QHash>
#include <QList>
#include <QVariant>

#include "joysticksdl.h"

class JoystickContainer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int numPads READ getNumberJoysticks NOTIFY numPadsChanged)
public:
    explicit JoystickContainer(QObject *parent = nullptr);
    ~JoystickContainer();
    void clearJoysticks();
    void establishJoystick(int index, SDL_Joystick* joy);
    int getNumberJoysticks();
    Q_INVOKABLE JoystickSDL* getJoystick(int index);
    QHash<int, JoystickSDL*>* getJoysticks();

private:
    QHash<int, JoystickSDL*>* joysticks;
    QList<JoystickSDL*>* tempJoyList;

signals:
    void numPadsChanged(int count);
};


#endif // JOYSTICKCONTAINER_H
