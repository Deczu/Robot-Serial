#ifndef KINETICTHREAD_H
#define KINETICTHREAD_H

#include <QThread>
#include <QtMath>

#define MAX_ALFA 146.3

typedef unsigned char BYTE;

class kineticthread : public QThread
{
    Q_OBJECT

    protected:
        void run();
        // do kinematyki z moodla

        double H = 0;
        double L1 = 8;
        double L2 = 8;
        double L3 = 20;
        double Xb;
        double Zb;
        double Q;
        double P_1;
        double P_2;
        double T_1;
        double T_2;
        double T_3;
        double Ph1;
        double Ph2;
        double Ph3;
        double Ph0;
        double R0[4][4] = {{0}};
        double R1[4][4] = {{0}};
        double R2[4][4] = {{0}};
        double R3[4][4] = {{0}};
        double T0[4][4] = {{1, 0, 0, 0},
                           {0, 1, 0, H},
                           {0, 0, 1, 0},
                           {0, 0, 0, 1}};
        double T1[4][4] = {{1, 0, 0, L1},
                           {0, 1, 0, 0},
                           {0, 0, 1, 0},
                           {0, 0, 0, 1}};
        double T2[4][4] = {{1, 0, 0, L2},
                           {0, 1, 0, 0},
                           {0, 0, 1, 0},
                           {0, 0, 0, 1}};
        double T3[4][4] = {{1, 0, 0, L3},
                           {0, 1, 0, 0},
                           {0, 0, 1, 0},
                           {0, 0, 0, 1}};
        double A0[4][4] = {{0}};
        double A1[4][4] = {{0}};
        double A2[4][4] = {{0}};
        double A3[4][4] = {{0}};
        double P0[4][4] = {{0}};
        double P1[4][4] = {{0}};
        double P2[4][4] = {{0}};
        double P3[4][4] = {{0}};
        double tmp_array[4][4] = {{0}};


        //kinematyka v2
        double c;
        double tmp_c;
        double tmp_angle;
        double tmp_val;
        int rotated;
        double dS5;
        double dS4;
        double dS3;
        double zS5;
        double zS4;
        double zS3;
        double P1_angle;
        double P2_angle;
        double P3_angle;

    public:
        int X;
        int Y;
        int Z;
        int P;
        bool changed = false;
        BYTE S3;
        BYTE S4;
        BYTE S5;
        BYTE S6;
        double xy_P1[2];
        double xy_P2[2];
        double xy_P3[2];

    signals:
        void ready_data();

};

#endif // KINETICTHREAD_H
