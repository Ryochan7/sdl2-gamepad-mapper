#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
//#undef main

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
//#include <QDebug>
#include <QList>
#include <QStringList>
#include <QDir>
#include <QStandardPaths>
#include <QSettings>

#include "registerqmltypes.h"


void RegisterQMLTypes()
{
    RegisterQMLTypes::RegisterTypesForQML();
}

int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    //QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setOrganizationName("AutismPowered");
    app.setApplicationName("SDL2GameControllerMapper");

    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir;
    if (!dir.exists(appDataPath))
    {
        dir.mkpath(appDataPath);
    }

    QString appStyle = QQuickStyle::name();
    //qDebug() << "APP STYLE: " << appStyle;
    if (appStyle.isEmpty())
    {
        QQuickStyle::setStyle("Fusion");
    }

    RegisterQMLTypes();

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
