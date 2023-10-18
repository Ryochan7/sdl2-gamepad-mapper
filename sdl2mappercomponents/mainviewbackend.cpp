#include <QtGlobal>
//#include <QDebug>
#include <QClipboard>
#include <QGuiApplication>
#include <QThread>
#include <QStandardPaths>
#include <QDir>
#include <QTextStream>
#include <QSettings>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#ifdef Q_OS_WINDOWS
#include <windows.h>
#include <winbase.h>
#endif

#include <SDL2/SDL.h>

#include "mainviewbackend.h"
#include "joysticksdl.h"

static const char* SDL_ENVVAR_NAME = "SDL_GAMECONTROLLERCONFIG";
QString MainViewBackend::PROGRAM_VERSION = "0.0.4";

static const char* upstreamGameControllerDBUrl ="https://raw.githubusercontent.com/gabomdq/SDL_GameControllerDB/master/gamecontrollerdb.txt";
static const char* upstreamGameControllerAPICommitCheck = "https://api.github.com/repos/gabomdq/SDL_GameControllerDB/commits?path=gamecontrollerdb.txt&page=1&per_page=1";

MainViewBackend::MainViewBackend(QObject *parent) : QObject(parent)
{
    sdlGCEnvVar = false;
    manager = new QNetworkAccessManager(this);

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
    Q_UNUSED(result);
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
    stream << mapping << Qt::endl;
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

bool MainViewBackend::checkLatestMappingFile()
{
    bool result = false;

    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QStringList tempList(appDataPath);
    tempList.append("gamecontrollerdb.txt");
    QString localMappingPath = QFileInfo(tempList.join("/")).absoluteFilePath();
    QSettings appSettings(QString("%1/%2").arg(appDataPath).arg("settings.ini"), QSettings::IniFormat);
    QDateTime lastModified = appSettings.value("remoteMappingLastChecked",
                                                   QDateTime::currentDateTime().addDays(-2)).toDateTime();

    bool fetchRemote = false;
    if (!QFile::exists(localMappingPath))
    {
        fetchRemote = true;
    }
    else
    {
        if (QDateTime::currentDateTime() > lastModified.addDays(1))
        {
            fetchRemote = true;
        }
    }

    if (fetchRemote)
    {
        result = true;
        appSettings.setValue("remoteMappingLastChecked", QDateTime::currentDateTime());
    }

    return result;
}

void MainViewBackend::requestLatestMappingInfo()
{
    QUrl controllerDBAPICall(upstreamGameControllerAPICommitCheck);
    QNetworkRequest request(controllerDBAPICall);
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &MainViewBackend::remoteMappingAPICallReplyFinished);
}

void MainViewBackend::requestLatestMappingFile()
{
    QUrl controllerDbURL(upstreamGameControllerDBUrl);
    QNetworkRequest request(controllerDbURL);
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &MainViewBackend::remoteMappingCheckReplyFinished);
}

void MainViewBackend::remoteMappingAPICallReplyFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    m_errorString = reply->errorString();
    if (reply->error() == QNetworkReply::NoError)
    {
        QString khantent = reply->readAll();
        QByteArray tempBytes = khantent.toUtf8();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(tempBytes);
        QJsonArray jsonArray = jsonDoc.array();

        if (jsonArray.count() > 0)
        {
            QJsonObject jsonObj = jsonArray[0].toObject();
            QVariantMap rootMap = jsonObj.toVariantMap();
            QVariant dateVariant = jsonObj.value("commit").toObject().value("committer").toObject().value("date").toVariant();
            if (!dateVariant.isNull())
            {
                QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
                QStringList tempList(appDataPath);
                tempList.append("gamecontrollerdb.txt");
                QString localMappingPath = QFileInfo(tempList.join("/")).absoluteFilePath();

                QDateTime lastDateTime = dateVariant.toDateTime();
                QFileInfo tempInfo = QFileInfo(localMappingPath);
                if (lastDateTime > tempInfo.lastModified())
                {
                    requestLatestMappingFile();
                }
                else
                {
                    emit upstreamMappingCheckFinished();
                }
            }
        }
        else
        {
            m_errorString = tr("JSON Parsing Failed");
            emit upstreamMappingCheckFinished();
        }

    }
    else
    {
        emit upstreamMappingCheckFinished();
    }
}

void MainViewBackend::remoteMappingCheckReplyFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    m_errorString = reply->errorString();
    if (reply->error() == QNetworkReply::NoError)
    {
        QString khantent = reply->readAll();

        QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QStringList tempList(appDataPath);
        tempList.append("gamecontrollerdb.txt");
        QFileInfo tempInfo = QFileInfo(tempList.join("/"));
        QString localMappingPath = tempInfo.absoluteFilePath();

        QFile tempFile(localMappingPath);
        tempFile.open(QIODevice::WriteOnly | QIODevice::Text);

        QTextStream stream(&tempFile);
        stream << khantent << Qt::endl;
        stream.flush();

        tempFile.close();

        reply->close();
    }

    emit upstreamMappingCheckFinished();
}

void MainViewBackend::loadExistingMappingFiles()
{
    attemptReadMainMappingFile();
    attemptReadLocalMappingFile();
}

void MainViewBackend::attemptReadMainMappingFile()
{
    // Attempt to read base Game Controller mappings files
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QStringList tempMainList(appDataPath);
    tempMainList.append("gamecontrollerdb.txt");
    QString testMappingPath = QFileInfo(tempMainList.join("/")).absoluteFilePath();

    if (QFileInfo::exists(testMappingPath))
    {
        SDL_GameControllerAddMappingsFromFile(testMappingPath.toStdString().c_str());
    }
}

void MainViewBackend::attemptReadLocalMappingFile()
{
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    // Attempt to read user generated Game Controller mappings file
    QStringList tempList(appDataPath);
    tempList.append("gamecontrollerdb.local.txt");
    QString testMappingPath = QFileInfo(tempList.join("/")).absoluteFilePath();
    SDL_GameControllerAddMappingsFromFile(testMappingPath.toStdString().c_str());

    if (QFileInfo::exists(testMappingPath))
    {
        SDL_GameControllerAddMappingsFromFile(testMappingPath.toStdString().c_str());
    }
}
