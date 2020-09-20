#ifndef APPLOGGER_H
#define APPLOGGER_H

#include <QObject>

class AppLogger : public QObject
{
    Q_OBJECT
public:
    explicit AppLogger(QObject *parent = nullptr);
    Q_INVOKABLE void log(QString message);

signals:
    void logMessage(QString message);
};

#endif // APPLOGGER_H
