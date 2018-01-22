#include "serialcomminterface.h"



serialcomminterface::serialcomminterface(){
    foreach (const QSerialPortInfo info, QSerialPortInfo::availablePorts())
    {
      qDebug() << "Name : " << info.portName();
      qDebug() << "Description : " << info.description();
      qDebug() << "Manufacturer: " << info.manufacturer();
      if(info.manufacturer() == "FTDI" && info.description() == "USB Serial Port") serial.setPortName(info.portName());

    }


    //serial.setPortName("COM6");
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    serial.open(QIODevice::ReadWrite);
}

void serialcomminterface::Send(SerialServo data)
{
    Buffer_O[0]=255;
    Buffer_O[1]=0;
    Buffer_O[2]=data.S1;
    Buffer_O[3]=255;
    Buffer_O[4]=1;
    Buffer_O[5]=data.S2;
    Buffer_O[6]=255;
    Buffer_O[7]=2;
    Buffer_O[8]=data.S3;
    Buffer_O[9]=255;
    Buffer_O[10]=3;
    Buffer_O[11]=data.S4;
    Buffer_O[12]=255;
    Buffer_O[13]=4;
    Buffer_O[14]=data.S5;
    Buffer_O[15]=255;
    Buffer_O[16]=5;
    Buffer_O[17]=data.S6;

    BufferArray = QByteArray((char*)Buffer_O, 18);
    serial.write(BufferArray);
    serial.flush();
//    emit SerialSendSign(Buffer_O);
//    qDebug() <<"\nBuffer[0]: ";
//    for (int i=0; i<sizeof(Buffer_O); i++)
//        qDebug() << "BUFFER: "<<(BYTE*)Buffer_O[i];
}

