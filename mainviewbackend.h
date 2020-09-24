#ifndef MAINVIEWBACKEND_H
#define MAINVIEWBACKEND_H

#include <QObject>
#include <QNetworkAccessManager>

#include "sdl2mapperbackend.h"
#include "joystickcontainermodel.h"

class MainViewBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SDL2MapperBackend* mapperBack MEMBER mapperBackend)
    Q_PROPERTY(bool hasSDLEnvVar MEMBER sdlGCEnvVar NOTIFY sdlGCEnvVarChanged)
    Q_PROPERTY(JoystickContainerModel* joyComboModel READ getJoyComboModel NOTIFY joyComboModelChanged)
    Q_PROPERTY(QString progVersion MEMBER PROGRAM_VERSION CONSTANT)
    Q_PROPERTY(QString errorString MEMBER m_errorString CONSTANT)

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
    Q_INVOKABLE bool checkLatestMappingFile();
    Q_INVOKABLE void requestLatestMappingFile();

    static QString PROGRAM_VERSION;

private:
    void checkForEnvvar();
    //void remoteMappingCheckReplyFinished(QNetworkReply *reply);

    SDL2MapperBackend* mapperBackend;
    bool sdlGCEnvVar;
    JoystickContainerModel* joyContainer;
    QNetworkAccessManager* manager;
    QString m_errorString;

signals:
    void sdlGCEnvVarChanged();
    void joyComboModelChanged();
    void upstreamMappingCheckFinished();

private slots:
    //void remoteMappingHeaderReplyFinished();
    void remoteMappingCheckReplyFinished();
};

#endif // MAINVIEWBACKEND_H
