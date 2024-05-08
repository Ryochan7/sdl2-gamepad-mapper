#include "applogger.h"

AppLogger::AppLogger(QObject *parent) : QObject(parent)
{

}

 void AppLogger::log(QString message)
 {
    emit logMessage(message);
 }
