#include "apploggersingleton.h"

AppLoggerSingleton* AppLoggerSingleton::m_instance = nullptr;

AppLoggerSingleton::AppLoggerSingleton(QObject *parent) : QObject(parent)
{
    loggerInstance = new AppLogger(this);
}

AppLoggerSingleton* AppLoggerSingleton::getInstance()
{
    if (!m_instance)
    {
        m_instance = new AppLoggerSingleton();
    }

    return m_instance;
}


AppLogger* AppLoggerSingleton::getLogger()
{
    return loggerInstance;
}

AppLoggerSingleton::~AppLoggerSingleton()
{
    if (loggerInstance)
    {
        delete loggerInstance;
        loggerInstance = nullptr;
    }

    m_instance = nullptr;
}
