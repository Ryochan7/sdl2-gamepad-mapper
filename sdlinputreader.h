#ifndef SDLINPUTREADER_H
#define SDLINPUTREADER_H

#include <QObject>
#include <QTimer>

#include "joystickcontainer.h"

class SDLInputReader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(JoystickContainer* joypadContainer READ getJoyContainer CONSTANT)
public:
    explicit SDLInputReader(QObject *parent = nullptr);
    JoystickContainer* getJoyContainer();
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

private:
    JoystickContainer joyContainer;
    QTimer pollRateTimer;
    bool sdlOpen;

    void initSDL();
    void closeSDL();

signals:

public slots:
    void runInputPass();

};

#endif // SDLINPUTREADER_H
