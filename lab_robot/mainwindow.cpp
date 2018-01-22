#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    comm = new CommInterface();
//    serial = new QSerialPort(this); //wbudowane do wysylania na port
    serialcomm = new serialcomminterface(); //pomocnicze do przesylania danych do serial
//    serial->setPortName("COM2");
//    serial->setBaudRate(QSerialPort::Baud9600);
//    serial->setDataBits(QSerialPort::Data8);
//    serial->setParity(QSerialPort::NoParity);
//    serial->setStopBits(QSerialPort::OneStop);
//    serial->setFlowControl(QSerialPort::NoFlowControl);
//    serial->open(QIODevice::ReadWrite);

    connect(serialcomm, SIGNAL(SerialSendSign(BYTE[])), this, SLOT(SerialSend(BYTE[])));

    gflag=0;
    Runf=0;
    StepCount=100;
    ProgramCounter=0;
    scnt=0;
    pcnt=0;
    gcnt=0;
    Pos.S1=SerialPos.S1=128;
    Pos.S2=SerialPos.S2=128;
    Pos.S3=SerialPos.S3=128;
    Pos.S4=SerialPos.S4=128;
    Pos.S5=SerialPos.S5=128;
    Pos.S6=SerialPos.S6=128;
    Pout.S1=SerialPout.S1=128;
    Pout.S2=SerialPout.S2=128;
    Pout.S3=SerialPout.S3=128;
    Pout.S4=SerialPout.S4=128;
    Pout.S5=SerialPout.S5=128;
    Pout.S6=SerialPout.S6=128;
    SerialProgram[0]=SerialPout;
    SerialSrv[0]=SerialPout;

    th=new MThread();
    kineticth = new kineticthread();
    connect(th,SIGNAL(tick()),this,SLOT(ex_th_tick()));
    connect(kineticth, SIGNAL(ready_data()), this, SLOT(ustaw_serwa()));
    th->start(th->HighPriority);
    kineticth->X = ui->horizontalSlider->value();
    kineticth->Y = ui->horizontalSlider_7->value();
    kineticth->Z = ui->horizontalSlider_8->value();
    kineticth->P = ui->dial_P->value();
    kineticth->start(kineticth->HighPriority);

    gflag=1;

    H=0;
    L1=L2=8;
    L3=20;

    ui->text_x->setText(QString().setNum(ui->horizontalSlider->value()));
    ui->text_y->setText(QString().setNum(ui->horizontalSlider_7->value()));
    ui->text_z->setText(QString().setNum(ui->horizontalSlider_8->value()));
    ui->text_p->setText(QString().setNum(ui->dial_P->value()));

    testpen.setColor(Qt::black);
    testpen.setWidth(2);
    testpen2.setColor(Qt::red);
    testpen2.setWidth(2);
    testpen3.setColor(Qt::blue);
    testpen3.setWidth(2);
    testpen4.setColor(Qt::green);
    testpen4.setWidth(2);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    scene2 = new QGraphicsScene(this);
    ui->graphicsView_2->setScene(scene2);

    double m = 2.8;
    scene->clear();
    testline = scene->addLine(0,0,0, -8*m,testpen2);
    testline = scene->addLine(0,-8*m,0,-16*m,testpen3);
    testline = scene->addLine(0,-16*m,0,-36*m,testpen4);
    testline = scene->addLine(-10,0,10,0,testpen);
    for(int i=0;i<24;i=i+4){
        testline = scene->addLine(-10+i,0,-12+i,10,testpen);

    }
    scene2->clear();
    double tmp_value;
    tmp_value = M_PI/2;
    testline2 = scene2->addLine(-2,0,2,0,testpen);
    testline2 = scene2->addLine(0,0,-qCos(tmp_value)*100,-qSin(tmp_value)*100,testpen2);

}

MainWindow::~MainWindow()
{
    th->terminate();
    kineticth->terminate();
    delete comm;
    delete serialcomm;
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    int sx=centralWidget()->x()+MX;
    int sy=centralWidget()->y()+MY;
    int ex=centralWidget()->width()+sx-2*MX;
    int ey=centralWidget()->height()+sy-2*MY;
    double dx=(ex-sx)/(double)TX;
    double dy=(ey-sy)/(double)TY;
    QPainter painter(this);
    QPen pen;
    pen.setWidth(1);
    painter.setRenderHint(QPainter::Antialiasing, true);
    pen.setColor(QColor(255,255,255,255 ));
    painter.setPen(pen);
    painter.setBrush(QColor( 255,255,255,255 ));
    painter.drawRect(centralWidget()->x(), centralWidget()->y(),
    centralWidget()->width(), centralWidget()->height());
    pen.setStyle(Qt::DashLine);
    pen.setColor(QColor( 192,192,192,255 ));
    painter.setPen(pen);
    QFont font;
    font.setPointSize(8);
    painter.setFont(font);
    // ----- siatka ------------
    for(int x=0; x<=TX; x++)
    painter.drawLine(QLineF(sx+x*dx, sy, sx+x*dx, ey));
    for(int y=0; y<=TY; y++)
    painter.drawLine(QLineF(sx, sy+y*dy, ex, sy+y*dy));
    // ------- opis ----------
    pen.setColor(QColor( 0,0,0,255 ));
    painter.setPen(pen);
    for(int x=0; x<=TX; x++)
    painter.drawText(QPoint(sx-4+x*dx, ey+font.pointSize()),
    QString("%1").arg(x));
    for(int y=0; y<=TY; y++)
    painter.drawText(QPoint(sx-20, sy+(font.pointSize()/2)+y*dy),
    QString("%1").arg(256-(256*y/TY)));

    //------ plot -------------
    double lx=(ex-sx)/1000.0;
    double ly=(ey-sy)/256.0;
    if(gflag)
    {
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(2);
        pen.setColor(QColor( 255,0,0,255 ));
        painter.setPen(pen);
        for ( int i=1; i<gcnt; i++ )
            painter.drawLine(QLineF(sx+(i-1)*lx, ey-SerialSrv[i-1].S1*ly, sx+i*lx, ey-SerialSrv[i].S1*ly));

        pen.setColor(QColor( 255,255,0,255 ));
        painter.setPen(pen);
        for ( int i=1; i<gcnt; i++ )
            painter.drawLine(QLineF(sx+(i-1)*lx, ey-SerialSrv[i-1].S2*ly, sx+i*lx, ey-SerialSrv[i].S2*ly));

        pen.setColor(QColor( 0,255,0,255 ));
        painter.setPen(pen);
        for ( int i=1; i<gcnt; i++ )
            painter.drawLine(QLineF(sx+(i-1)*lx, ey-SerialSrv[i-1].S3*ly, sx+i*lx, ey-SerialSrv[i].S3*ly));

        pen.setColor(QColor( 0,255,255,255 ));
        painter.setPen(pen);
        for ( int i=1; i<gcnt; i++ )
            painter.drawLine(QLineF(sx+(i-1)*lx, ey-SerialSrv[i-1].S4*ly, sx+i*lx, ey-SerialSrv[i].S4*ly));

        pen.setColor(QColor( 255,128,0,255 ));
        painter.setPen(pen);
        for ( int i=1; i<gcnt; i++ )
            painter.drawLine(QLineF(sx+(i-1)*lx, ey-SerialSrv[i-1].S5*ly, sx+i*lx, ey-SerialSrv[i].S5*ly));

        pen.setColor(QColor( 0,0,255,255 ));
        painter.setPen(pen);
        for ( int i=1; i<gcnt; i++ )
            painter.drawLine(QLineF(sx+(i-1)*lx, ey-SerialSrv[i-1].S6*ly, sx+i*lx, ey-SerialSrv[i].S6*ly));
    }

    QPainter robotpainter(this);
    QPen robotpen;
}

void MainWindow::on_horizontalSlider_1_valueChanged(int value)
{
    ui->label_1->setText(QString().setNum(value));
    Pout.S1=SerialPout.S1=value;
    if(Runf==0){
        serialcomm->Send(SerialPout);
//        comm->Send(Pout);
    }
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    ui->label_2->setText(QString().setNum(value));
    Pout.S2=SerialPout.S2=value;
    if(Runf==0){
        serialcomm->Send(SerialPout);
//        comm->Send(Pout);
    }
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    ui->label_3->setText(QString().setNum(value));
    Pout.S3=SerialPout.S3=value;
    if(Runf==0){
        serialcomm->Send(SerialPout);
//        comm->Send(Pout);
    }
}

void MainWindow::on_horizontalSlider_4_valueChanged(int value)
{
    ui->label_4->setText(QString().setNum(value));
    Pout.S4=SerialPout.S4=value;
    if(Runf==0){
        serialcomm->Send(SerialPout);
//        comm->Send(Pout);
    }
}

void MainWindow::on_horizontalSlider_5_valueChanged(int value)
{
    ui->label_5->setText(QString().setNum(value));
    Pout.S5=SerialPout.S5=value;
    if(Runf==0){
        serialcomm->Send(SerialPout);
        //comm->Send(Pout);
    }
}

void MainWindow::on_horizontalSlider_6_valueChanged(int value)
{
    ui->label_6->setText(QString().setNum(value));
    Pout.S6=SerialPout.S6=value;
    if(Runf==0){
        //qDebug() << "\n1. przed wyslaniem serialcomm->Send";
        serialcomm->Send(SerialPout);
        //qDebug() << "2. miedzy Sendami";
        //comm->Send(Pout);
        //qDebug() << "3. po wyslaniu serialcomm->Send";
    }
}

void MainWindow::ex_th_tick()
{
    // static int counter=0;

    if(Runf==1)
    {
        ui->label_7->setText(QString("Step: %1").arg(pcnt+1));
        if(scnt==0)
        {
            SerialPos.S1= Pos.S1=(double)Pout.S1;
            Serialdx.S1 = dx.S1= (SerialProgram[pcnt].S1-Pout.S1)/(double)StepCount;
            SerialPos.S2= Pos.S2=(double)Pout.S2;
            Serialdx.S2 = dx.S2= (SerialProgram[pcnt].S2-Pout.S2)/(double)StepCount;
            SerialPos.S3= Pos.S3=(double)Pout.S3;
            Serialdx.S3 = dx.S3= (SerialProgram[pcnt].S3-Pout.S3)/(double)StepCount;
            SerialPos.S4= Pos.S4=(double)Pout.S4;
            Serialdx.S4 = dx.S4= (SerialProgram[pcnt].S4-Pout.S4)/(double)StepCount;
            SerialPos.S5= Pos.S5=(double)Pout.S5;
            Serialdx.S5 = dx.S5= (SerialProgram[pcnt].S5-Pout.S5)/(double)StepCount;
            SerialPos.S6= Pos.S6=(double)Pout.S6;
            Serialdx.S6 = dx.S6= (SerialProgram[pcnt].S6-Pout.S6)/(double)StepCount;
        }
        Pos.S1+=dx.S1;
        Pout.S1=(BYTE)Pos.S1;
        SerialPos.S1 = Pos.S1;
        SerialPout.S1= Pout.S1;
        Pos.S2+=dx.S2;
        Pout.S2=(BYTE)Pos.S2;
        SerialPos.S2 = Pos.S2;
        SerialPout.S2= Pout.S2;
        Pos.S3+=dx.S3;
        Pout.S3=(BYTE)Pos.S3;
        SerialPos.S3 = Pos.S3;
        SerialPout.S3= Pout.S3;
        Pos.S4+=dx.S4;
        Pout.S4=(BYTE)Pos.S4;
        SerialPos.S4 = Pos.S4;
        SerialPout.S4= Pout.S4;
        Pos.S5+=dx.S5;
        Pout.S5=(BYTE)Pos.S5;
        SerialPos.S5 = Pos.S5;
        SerialPout.S5= Pout.S5;
        Pos.S6+=dx.S6;
        Pout.S6=(BYTE)Pos.S6;
        SerialPos.S6 = Pos.S6;
        SerialPout.S6= Pout.S6;

        //comm->Send(Pout);
        serialcomm->Send(SerialPout);

        ui->horizontalSlider_1->setValue(SerialPout.S1);
        ui->horizontalSlider_2->setValue(SerialPout.S2);
        ui->horizontalSlider_3->setValue(SerialPout.S3);
        ui->horizontalSlider_4->setValue(SerialPout.S4);
        ui->horizontalSlider_5->setValue(SerialPout.S5);
        ui->horizontalSlider_6->setValue(SerialPout.S6);

        scnt++;
        if(scnt>=StepCount)
        {
            scnt=0;
            if(pcnt<ProgramCounter)
                pcnt++;
        }
        if(pcnt>=ProgramCounter)
        {
            Runf=0;
            ui->actionRun_Program->setEnabled(true);
        }
        SerialSrv[gcnt] = SerialPout;

        gcnt++;
        ui->centralWidget->repaint();
        if(gcnt > StepCount*10)
            gcnt=0;
    }
}

void MainWindow::on_actionNew_triggered()
{
    ui->actionRun_Program->setEnabled(false);
    ui->textBrowser->clear();
    gcnt=0;
    ProgramCounter=0;
    pcnt=0;
    ui->label_7->setText(QString("Step: %1").arg(ProgramCounter+1));
    ui->horizontalSlider_1->setValue(128);
    ui->horizontalSlider_2->setValue(128);
    ui->horizontalSlider_3->setValue(128);
    ui->horizontalSlider_4->setValue(128);
    ui->horizontalSlider_5->setValue(128);
    ui->horizontalSlider_6->setValue(128);

    ui->dockWidget_1->setEnabled(true);

    ui->actionAdd_Step->setEnabled(true);
    ui->actionEnd_Step->setEnabled(false);
    ui->actionRun_Program->setEnabled(false);

    Pout.S1=SerialPout.S1=128;
    Pout.S2=SerialPout.S2=128;
    Pout.S3=SerialPout.S3=128;
    Pout.S4=SerialPout.S4=128;
    Pout.S5=SerialPout.S5=128;
    Pout.S6=SerialPout.S6=128;

    SerialProgram[0]=SerialPout;
    SerialSrv[0]=SerialPout;
}

void MainWindow::on_actionAdd_Step_triggered()
{
    SerialProgram[ProgramCounter]=SerialPout;
    ui->label_7->setText(QString("Step: %1").arg(ProgramCounter+1));
    ui->textBrowser->insertPlainText(QString().sprintf("%4d) %03d %03d %03d %03d %03d %03d\n", ProgramCounter+1, Pout.S1, Pout.S2, Pout.S3, Pout.S4, Pout.S5, Pout.S6));

    ui->actionEnd_Step->setEnabled(true);
    ui->actionRun_Program->setEnabled(false);

    if(ProgramCounter<MAXSTEP)
        ProgramCounter++;
}

void MainWindow::on_actionEnd_Step_triggered()
{
    ui->actionRun_Program->setEnabled(true);
    ui->actionEnd_Step->setEnabled(false);
}

void MainWindow::on_actionRun_Program_triggered()
{
    scnt=0;
    pcnt=0;
    gcnt=0;
    Runf=1;
    ui->actionRun_Program->setEnabled(false);
}

void MainWindow::on_actionSave_triggered()
{
    QString FileName;
    FileName = QFileDialog::getSaveFileName(this, tr("Open File"),"/home/robot", tr("Robot Files (*.txt *.rob)"));
    QFile file(FileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    file.write((char*)SerialProgram,sizeof(Servo)*ProgramCounter);
    file.close();
}

void MainWindow::on_actionOpen_triggered()
{
    on_actionNew_triggered();
    QString FileName;
    FileName = QFileDialog::getOpenFileName(this, tr("Open File"),"/home/robot", tr("Robot Files (*.txt *.rob)"));
    QFile file(FileName);
    if (!file.open(QIODevice::ReadOnly))
        return;
    QByteArray text = file.readAll();
    qDebug() << text << '\n';

    int ilosc = text.size()/sizeof(Servo);
    for (int i=0; i<ilosc; i++){
        SerialPout.S1 = (int)text.at(sizeof(SerialServo)*i);
        SerialPout.S2 = (int)text.at(sizeof(SerialServo)*i+1);
        SerialPout.S3 = (int)text.at(sizeof(SerialServo)*i+2);
        SerialPout.S4 = (int)text.at(sizeof(SerialServo)*i+3);
        SerialPout.S5 = (int)text.at(sizeof(SerialServo)*i+4);
        SerialPout.S6 = (int)text.at(sizeof(SerialServo)*i+5);
        SerialProgram[ProgramCounter]=SerialPout;
        ui->label_7->setText(QString("Step: %1").arg(ProgramCounter+1));
        ui->textBrowser->insertPlainText(QString().sprintf("%4d) %03d %03d %03d %03d %03d %03d\n",
        ProgramCounter+1, Pout.S1, Pout.S2, Pout.S3, Pout.S4, Pout.S5, Pout.S6));
        ProgramCounter++;
    }
    ui->actionRun_Program->setEnabled(true);
    ui->actionEnd_Step->setEnabled(false);

    file.close();
}

void MainWindow::SerialSend(BYTE *tablica){
    qDebug() << "slot SerialSend";
    qDebug() << "serial length: "<< sizeof(*tablica);
    qDebug() << "serial: "<< (int)tablica<<"\n";
//    for (int i=0; i<18; i++){
//        qDebug() << "petla: "<< (BYTE*)*tablica<<" ";
//    }
//    serial->write("tablica");
}


int MainWindow::getX(){
    return ui->horizontalSlider->value();
}
int MainWindow::getY(){
    return ui->horizontalSlider_7->value();
}
int MainWindow::getZ(){
    return ui->horizontalSlider_8->value();
}
int MainWindow::getP(){
    return ui->dial_P->value();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    kineticth->X = ui->horizontalSlider->value();
    kineticth->changed = true;
    ui->text_x->setText(QString().setNum(value));
}

void MainWindow::on_horizontalSlider_7_valueChanged(int value)
{
    kineticth->Y = ui->horizontalSlider_7->value();
    kineticth->changed = true;
    ui->text_y->setText(QString().setNum(value));
}

void MainWindow::on_horizontalSlider_8_valueChanged(int value)
{
    kineticth->Z = ui->horizontalSlider_8->value();
    kineticth->changed = true;
    ui->text_z->setText(QString().setNum(value));
}

void MainWindow::on_dial_P_valueChanged(int value)
{
    kineticth->P = ui->dial_P->value();
    kineticth->changed = true;
    ui->text_p->setText(QString().setNum(value));
}

void MainWindow::ustaw_serwa(){
    // qDebug() << "ustawienie_serwa";
    ui->horizontalSlider_3->setValue(kineticth->S3);
    ui->horizontalSlider_4->setValue(kineticth->S4);
    ui->horizontalSlider_5->setValue(kineticth->S5);
    ui->horizontalSlider_6->setValue(kineticth->S6);

    double m = 2.8;
    scene->clear();
    testline = scene->addLine(0,0,kineticth->xy_P1[0]*m, -kineticth->xy_P1[1]*m,testpen2);
    testline = scene->addLine(kineticth->xy_P1[0]*m,-kineticth->xy_P1[1]*m,kineticth->xy_P2[0]*m,-kineticth->xy_P2[1]*m,testpen3);
    testline = scene->addLine(kineticth->xy_P2[0]*m,-kineticth->xy_P2[1]*m,kineticth->xy_P3[0]*m,-kineticth->xy_P3[1]*m,testpen4);
    testline = scene->addLine(-10,0,10,0,testpen);
    for(int i=0;i<24;i=i+4){
        testline = scene->addLine(-10+i,0,-12+i,10,testpen);

    }
    scene2->clear();
    double tmp_value;
    tmp_value = ((kineticth->S6-128)*M_PI/256)+(M_PI/2);
    testline2 = scene2->addLine(-2,0,2,0,testpen);
    testline2 = scene2->addLine(0,0,-qCos(tmp_value)*100,-qSin(tmp_value)*100,testpen2);

    ui->label_10->setText(QString().setNum(kineticth->xy_P1[0]));
    ui->label_11->setText(QString().setNum(kineticth->xy_P1[1]));
    ui->label_12->setText(QString().setNum(kineticth->xy_P2[0]));
    ui->label_13->setText(QString().setNum(kineticth->xy_P2[1]));
    ui->label_14->setText(QString().setNum(kineticth->xy_P3[0]));
    ui->label_15->setText(QString().setNum(kineticth->xy_P3[1]));

}
