#ifndef MAINVIEWBACKEND_H
#define MAINVIEWBACKEND_H

#include <QObject>

#include "sdl2mapperbackend.h"
#include "joystickcontainermodel.h"

class MainViewBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SDL2MapperBackend* mapperBack MEMBER mapperBackend)
    Q_PROPERTY(bool hasSDLEnvVar MEMBER sdlGCEnvVar NOTIFY sdlGCEnvVarChanged)
    Q_PROPERTY(JoystickContainerModel* joyComboModel READ getJoyComboModel NOTIFY joyComboModelChanged)

public:
    explicit MainViewBackend(QObject *parent = nullptr);
    Q_INVOKABLE void copyGUIDToClipboard(QString guid);
    Q_INVOKABLE void copyMappingString(QString mapping);
    Q_INVOKABLE void createMappingEnvvar(QString mapping);
    Q_INVOKABLE void removeMappingEnvvar();
    //Q_INVOKABLE QVariant getJoyComboModel();
    Q_INVOKABLE JoystickContainerModel* getJoyComboModel();
    Q_INVOKABLE void createJoyComboModel();
    Q_INVOKABLE bool writeMappingString(QString mapping);
    Q_INVOKABLE QString generateGUIDStringDetails(JoystickSDL* joyDev);
    Q_INVOKABLE bool hasGameControllerEnvvar();
    Q_INVOKABLE QString generateSDLVersionText();

private:
    void checkForEnvvar();

    SDL2MapperBackend* mapperBackend;
    bool sdlGCEnvVar;
    JoystickContainerModel* joyContainer;

signals:
    void sdlGCEnvVarChanged();
    void joyComboModelChanged();
};

#endif // MAINVIEWBACKEND_H
