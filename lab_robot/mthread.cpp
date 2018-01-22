#include "mthread.h"
/*
MThread::MThread()
{

}
*/
void MThread::run()
{
    while(this->isRunning())
    {
        emit this->tick();
        this->msleep(10);
    }
}
