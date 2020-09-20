#ifndef SDL2MAPPERBACKEND_H
#define SDL2MAPPERBACKEND_H

#include <QObject>
#include <QThread>

#include "sdlinputreader.h"

class SDL2MapperBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SDLInputReader* reader MEMBER inputReader CONSTANT)

public:
    explicit SDL2MapperBackend(QObject *parent = nullptr);
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    SDLInputReader* getInputReader();

private:
    SDLInputReader* inputReader;
    QThread* inputThread;

signals:

};

#endif // SDL2MAPPERBACKEND_H
