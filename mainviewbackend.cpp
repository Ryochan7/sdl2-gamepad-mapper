#include <QtGlobal>
//#include <QDebug>
#include <QClipboard>
#include <QGuiApplication>
#include <QThread>
#include <QStandardPaths>
#include <QDir>
#include <QTextStream>
#include <QSettings>

#ifdef Q_OS_WINDOWS
#include <windows.h>
#include <winbase.h>
#endif

#include <SDL2/SDL.h>

#include "mainviewbackend.h"
#include "joysticksdl.h"

static const char* SDL_ENVVAR_NAME = "SDL_GAMECONTROLLERCONFIG";
QString MainViewBackend::PROGRAM_VERSION = "0.0.1";

MainViewBackend::MainViewBackend(QObject *parent) : QObject(parent)
{
    sdlGCEnvVar = false;

    checkForEnvvar();
}

void MainViewBackend::checkForEnvvar()
{
    sdlGCEnvVar = false;
    //QString temp = qEnvironmentVariable(SDL_ENVVAR_NAME);
    bool exists = qEnvironmentVariableIsSet(SDL_ENVVAR_NAME);
    QByteArray tempArray = qgetenv(SDL_ENVVAR_NAME);
    //QString temp(tempArray);
    if (!tempArray.isEmpty())
    {
        sdlGCEnvVar = true;
    }

    sdlGCEnvVar = exists;
}

void MainViewBackend::copyGUIDToClipboard(QString guid)
{
    QClipboard* clipboard = QGuiApplication::clipboard();

    clipboard->setText(guid, QClipboard::Clipboard);

    if (clipboard->supportsSelection()) {
        clipboard->setText(guid, QClipboard::Selection);
    }

#if defined(Q_OS_LINUX)
    QThread::msleep(1); //workaround for copied text not being available...
#endif
}

void MainViewBackend::copyMappingString(QString mapping)
{
    QClipboard* clipboard = QGuiApplication::clipboard();

    clipboard->setText(mapping, QClipboard::Clipboard);

    if (clipboard->supportsSelection()) {
        clipboard->setText(mapping, QClipboard::Selection);
    }

#if defined(Q_OS_LINUX)
    QThread::msleep(1); //workaround for copied text not being available...
#endif
}

void MainViewBackend::createMappingEnvvar(QString mapping)
{

    QByteArray tempArray = mapping.toLocal8Bit();
    bool result = qputenv(SDL_ENVVAR_NAME, tempArray);
    QSettings settings("HKEY_CURRENT_USER\\Environment", QSettings::NativeFormat);
    settings.setValue(SDL_ENVVAR_NAME, mapping);
    settings.sync();
    //qDebug() << "ENVVAR: " << result;
    checkForEnvvar();
    emit sdlGCEnvVarChanged();
}

void MainViewBackend::removeMappingEnvvar()
{
    qunsetenv(SDL_ENVVAR_NAME);

    QSettings settings("HKEY_CURRENT_USER\\Environment", QSettings::NativeFormat);
    settings.remove(SDL_ENVVAR_NAME);
    settings.sync();

    checkForEnvvar();
    emit sdlGCEnvVarChanged();
}

/*QVariant MainViewBackend::getJoyComboModel()
{
    return QVariant::fromValue(joyContainer);
}
*/

JoystickContainerModel* MainViewBackend::getJoyComboModel()
{
    return joyContainer;
}

void MainViewBackend::createJoyComboModel()
{
    if (mapperBackend != nullptr)
    {
        joyContainer = new JoystickContainerModel(mapperBackend->getInputReader()->getJoyContainer(), this);
        emit joyComboModelChanged();
    }
}

bool MainViewBackend::writeMappingString(QString mapping)
{
    bool result = false;
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QStringList tempList(appDataPath);
    tempList.append("gamecontrollerdb.local.txt");
    QString testMappingPath = tempList.join("/");
    QFile tempFile(testMappingPath);

    //qDebug() << QDir::current();
    if (!tempFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        return result;
    }

    QTextStream stream(&tempFile);
    stream << mapping << endl;
    stream.flush();

    tempFile.close();

    result = true;
    return result;
}

QString MainViewBackend::generateGUIDStringDetails(JoystickSDL* joyDev)
{
    QString result;
    result = tr("The GUID for \"%1\" is \"%2\"").arg(joyDev->getMappedName()).arg(joyDev->getDeviceGUID());
    return result;
}

bool MainViewBackend::hasGameControllerEnvvar()
{
    bool exists = qEnvironmentVariableIsSet(SDL_ENVVAR_NAME);
    return exists;
}

QString MainViewBackend::generateSDLVersionText()
{
    QString versionText = tr("Compiled with SDL %1, DLL version %2");
    SDL_version compiledVersion;
    SDL_GetVersion(&compiledVersion);
    QString compiledVersionText = QString("%1.%2.%3").arg(compiledVersion.major)
                                                     .arg(compiledVersion.minor)
                                                     .arg(compiledVersion.patch);

    SDL_version dllVersion;
    SDL_VERSION(&dllVersion);
    QString dllVersionText = QString("%1.%2.%3").arg(dllVersion.major)
                                                .arg(dllVersion.minor)
                                                .arg(dllVersion.patch);

    versionText = versionText.arg(compiledVersionText).arg(dllVersionText);
    return versionText;
}
