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

int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

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

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/sdl2mappercomponents/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
