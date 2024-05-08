#include "sdl2mapperbackend.h"

SDL2MapperBackend::SDL2MapperBackend(QObject *parent) : QObject(parent)
{
    inputReader = new SDLInputReader();
    inputThread = new QThread();
    inputReader->moveToThread(inputThread);

}

void SDL2MapperBackend::start()
{
    inputReader->start();
    inputThread->start(QThread::HighPriority);
}

void SDL2MapperBackend::stop()
{
    inputReader->stop();
    inputReader->getJoyContainer()->clearJoysticks();
}

SDLInputReader* SDL2MapperBackend::getInputReader()
{
    return inputReader;
}
