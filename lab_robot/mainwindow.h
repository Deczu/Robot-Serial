#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QFile>
#include <QFileDialog>
#include <QByteArray>
#include <QDebug>
#include <QtMath>
#include "comminterface.h"
#include "mthread.h"
#include "kineticthread.h"
#include "serialcomminterface.h"
#include <QGraphicsView>
#include <QGraphicsScene>

#define MX 40
#define MY 20
#define TX 10
#define TY 16
#define MAXPOINTS 2000
#define MAXSTEP 1000

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QGraphicsScene *scene;
    QGraphicsScene *scene2;
    QGraphicsLineItem *testline;
    QGraphicsLineItem *testline2;
    QPen testpen;
    QPen testpen2;
    QPen testpen3;
    QPen testpen4;
    CommInterface *comm;
    serialcomminterface *serialcomm;
    QSerialPort *serial;
    Servo Pin, Pout;
    Servo Srv[MAXPOINTS];
    Servo Program[MAXSTEP];
    ServoR Pos, dx;
    SerialServo SerialPin, SerialPout;
    SerialServo SerialSrv[MAXPOINTS];
    SerialServo SerialProgram[MAXSTEP];
    SerialServoR SerialPos, Serialdx;
    int ProgramCounter;
    int StepCount;
    int gcnt;
    int scnt;
    int pcnt;
    int gflag;
    int Runf;
    int H;
    int L1;
    int L2;
    int L3;

    int getX();
    int getY();
    int getZ();
    int getP();

private slots:
    void on_horizontalSlider_1_valueChanged(int value);
    void on_horizontalSlider_2_valueChanged(int value);
    void on_horizontalSlider_3_valueChanged(int value);
    void on_horizontalSlider_4_valueChanged(int value);
    void on_horizontalSlider_5_valueChanged(int value);
    void on_horizontalSlider_6_valueChanged(int value);

    void ex_th_tick();
    void ustaw_serwa();

    void on_actionNew_triggered();
    void on_actionAdd_Step_triggered();
    void on_actionEnd_Step_triggered();
    void on_actionRun_Program_triggered();

    void on_actionSave_triggered();
    void on_actionOpen_triggered();

    void SerialSend(BYTE[]);

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_7_valueChanged(int value);

    void on_horizontalSlider_8_valueChanged(int value);

    void on_dial_P_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent *);
    MThread *th;
    kineticthread *kineticth;
};

#endif // MAINWINDOW_H
