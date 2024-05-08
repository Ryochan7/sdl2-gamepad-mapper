#ifndef APPLOGGERSINGLETON_H
#define APPLOGGERSINGLETON_H

#include <QObject>

#include "applogger.h"

class AppLoggerSingleton : public QObject
{
    Q_OBJECT

public:
    static AppLoggerSingleton* getInstance();
    AppLogger* getLogger();

private:
    explicit AppLoggerSingleton(QObject *parent = nullptr);
    ~AppLoggerSingleton();

    static AppLoggerSingleton* m_instance;
    AppLogger* loggerInstance = nullptr;

signals:

};

#endif // APPLOGGERSINGLETON_H
