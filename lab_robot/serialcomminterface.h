#ifndef SERIALCOMMINTERFACE_H
#define SERIALCOMMINTERFACE_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QObject>
#include <QDebug>

typedef unsigned char BYTE;

typedef struct
{
    BYTE S1, S2, S3, S4, S5, S6;
}SerialServo;

typedef struct
{
    double S1, S2, S3, S4, S5, S6;
}SerialServoR;

class serialcomminterface : public QObject
{
    Q_OBJECT
    public:
        serialcomminterface();
//        ~serialcomminterface();
        void Send(SerialServo);
        bool Enabled;
    signals:
        void SerialSendSign(BYTE[]);

    private:
        QSerialPortInfo info;
        BYTE Buffer_O[18];
        QByteArray BufferArray;
        QSerialPort serial;
};

#endif // SERIALCOMMINTERFACE_H
