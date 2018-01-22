#include "kineticthread.h"

void kineticthread::run()
{
    while(this->isRunning())
    {
        if (changed){
            changed = false;
            /*
             * KINEMATYKA Z MOODLA
             *
            qDebug() << "\nkinetic th";
            qDebug() << X;
            qDebug() << Y;
            qDebug() << Z;
            qDebug() << P;
            Xb = (X-L3*qCos(P*M_PI/180))/(2*L1);
            Zb = (Z-H-L3*qSin(P*M_PI/180))/(2*L1);
            Q = qSqrt(1/(Xb*Xb+Zb*Zb)-1);
            P_1 = qAtan2(Zb + Q * Xb, Xb - Q * Zb)*180/M_PI;
            P_2 = qAtan2(Zb - Q * Xb, Xb + Q * Zb)*180/M_PI;
            T_1 = P_1 - 90;
            T_2 = P_2 - T_1;
            T_3 = P - P_2;
            Ph1 = (T_1+90)*2*M_PI/360;
            Ph2 = (T_2-90)*2*M_PI/360;
            Ph3 = T_3*2*M_PI/360;
            Ph0 = qAtan2(Y,X)*180/M_PI;
            R0[0][0] = qCos(Ph0);
            R0[0][2] = -qSin(Ph0);
            R0[1][1] = 1;
            R0[2][0] = -qSin(Ph0);
            R0[2][2] = qCos(Ph0);
            R0[3][3] = 1;

            R1[0][0] = qCos(Ph1);
            R1[0][1] = -qSin(Ph1);
            R1[1][0] = qSin(Ph1);
            R1[1][1] = qCos(Ph1);
            R1[2][2] = 1;
            R1[3][3] = 1;

            R2[0][0] = qCos(Ph2);
            R2[0][1] = -qSin(Ph2);
            R2[1][0] = qSin(Ph2);
            R2[1][1] = qCos(Ph2);
            R2[2][2] = 1;
            R2[3][3] = 1;

            R3[0][0] = qCos(Ph3);
            R3[0][1] = -qSin(Ph3);
            R3[1][0] = qSin(Ph3);
            R3[1][1] = qCos(Ph3);
            R3[2][2] = 1;
            R3[3][3] = 1;

            if (R3[0][0] != R3[0][0]){
                qDebug() << "R3 = NAN";
            } else{
                qDebug() <<"\nR0=";
                qDebug() << R0[0][0]<< R0[0][1]<< R0[0][2]<< R0[0][3];
                qDebug() << R0[1][0]<< R0[1][1]<< R0[1][2]<< R0[1][3];
                qDebug() << R0[2][0]<< R0[2][1]<< R0[2][2]<< R0[2][3];
                qDebug() << R0[3][0]<< R0[3][1]<< R0[3][2]<< R0[3][3];

                qDebug() <<"\nT0=";
                qDebug() << T0[0][0]<< T0[0][1]<< T0[0][2]<< T0[0][3];
                qDebug() << T0[1][0]<< T0[1][1]<< T0[1][2]<< T0[1][3];
                qDebug() << T0[2][0]<< T0[2][1]<< T0[2][2]<< T0[2][3];
                qDebug() << T0[3][0]<< T0[3][1]<< T0[3][2]<< T0[3][3];
                qDebug() << "\nA0=";
                // A0 A1 A2 A3
                for (int i=0; i<4; i++){
                    for (int j=0; j<4; j++){
                        A0[i][j] = 0;
                        for (int licznik=0; licznik<4; licznik++){
                            A0[i][j] += R0[i][licznik]*T0[licznik][j];
                        }
                    }
                    qDebug() << A0[i][0] << A0[i][1] << A0[i][2] << A0[i][3];
                }
                // qDebug() << "\nA1=";
                for (int i=0; i<4; i++){
                    for (int j=0; j<4; j++){
                        A1[i][j] = 0;
                        for (int licznik=0; licznik<4; licznik++){
                            A1[i][j] += R1[i][licznik]*T1[licznik][j];
                        }
                    }
                    // qDebug() << A1[i][0] << A1[i][1] << A1[i][2] << A1[i][3];
                }
                // qDebug() << "\nA2=";
                for (int i=0; i<4; i++){
                    for (int j=0; j<4; j++){
                        A2[i][j] = 0;
                        for (int licznik=0; licznik<4; licznik++){
                            A2[i][j] += R2[i][licznik]*T2[licznik][j];
                        }
                    }
                    // qDebug() << A2[i][0] << A2[i][1] << A2[i][2] << A2[i][3];
                }
                // qDebug() << "\nA3=";
                for (int i=0; i<4; i++){
                    for (int j=0; j<4; j++){
                        A3[i][j] = 0;
                        for (int licznik=0; licznik<4; licznik++){
                            A3[i][j] += R3[i][licznik]*T3[licznik][j];
                        }
                    }
                    // qDebug() << A3[i][0] << A3[i][1] << A3[i][2] << A3[i][3];
                }


                // P1
                qDebug() << "\nP1=";
                for (int i=0; i<4; i++){
                    for (int j=0; j<4; j++){
                        P1[i][j] = 0;
                        for (int licznik=0; licznik<4; licznik++){
                            P1[i][j] += A0[i][licznik]*A1[licznik][j];
                        }
                    }
                    qDebug() << P1[i][0] << P1[i][1] << P1[i][2] << P1[i][3];
                }

                // P2
                qDebug() << "\nP2=";
                for (int i=0; i<4; i++){
                    for (int j=0; j<4; j++){
                        P2[i][j] = 0;
                        for (int licznik=0; licznik<4; licznik++){
                            P2[i][j] += P1[i][licznik]*A2[licznik][j];
                        }
                    }
                    qDebug() << P2[i][0] << P2[i][1] << P2[i][2] << P2[i][3];
                }

                // P3
                qDebug() << "\nP3=";
                for (int i=0; i<4; i++){
                    for (int j=0; j<4; j++){
                        P3[i][j] = 0;
                        for (int licznik=0; licznik<4; licznik++){
                            P3[i][j] += P2[i][licznik]*A3[licznik][j];
                        }
                    }
                    qDebug() << P3[i][0] << P3[i][1] << P3[i][2] << P3[i][3];
                }

                // S5

                //int s5 = qFloor(128*(P1[0][3]/qSqrt(P1[0][3]*P1[0][3]+P1[1][3]*P1[1][3])))+128;
                // qDebug() << P1[1][4] << P1[2][4] << s5;
                //if (s5 < 0){
                //    S5 = 0;
                //}else if (s5 > 255){
                //    S5 = 255;
                //}else{
                //    S5 = s5;
                //}


                // S6
                S6 = 255*(Ph0/170);

                // S5 - pi do 0
                qDebug() << "Ph1 = " <<Ph1;
                if (Ph1 > M_PI)
                    S5 = 255;
                else if (Ph1 < 0)
                    S5 = 0;
                else
                    S5 = qFloor(81*Ph1);

                // S4
                if (Ph2 > M_PI/2)
                    S4 = 255;
                else if (Ph2 < -M_PI/2)
                    S4 = 0;
                else
                    S4 = qFloor(81*Ph2+128);

                // S3
                if (Ph3 > M_PI/2)
                    S3 = 255;
                else if (Ph3 < -M_PI/2)
                    S3 = 0;
                else
                    S3 = qFloor(81*Ph3+128);
                *
                * KONIEC KINEMATYKI Z MOODLA
                */

            // KINEMATYKA WERSJA INDYWIDUALNA
            // wyliczenie x,y dla z=0

            // przygotowania
            c = qSqrt(X*X + Y*Y);
            if (Y<0)
                rotated = -1;
            else
                rotated = 1;

            // S3, S4, S5
            if (c<12){
                S3 = 0;
                S4 = 255;
                S5 = round( 1.42 * MAX_ALFA );
            } else if (c>36){
                S3 = 128;
                S4 = 128;
                S5 = 0;
            } else{
                tmp_c = round(c-12);
                S3 = round(5.33333 * tmp_c);
                S4 = 255-S3;
                S5 = round((1.42 * MAX_ALFA) - (((1.42 * MAX_ALFA)/24) * tmp_c));
            }

            // S6
            if (X != 0){
                tmp_angle = qAtan2((double)Y*rotated, (double)X);
                //obrot dla uzyskania kierunku
                S6 = round(255 - (tmp_angle * (255/M_PI)));
            }else
                S6 = 128;

            // odwrocenie
            if (Y<0){
                S6 = 255 - S6;
                S5 = 255 - S5;
                S4 = 255 - S4;
                S3 = 255 - S3;
            }

            // obliczanie wysokosci Z
            zS5 = zS4 = zS3 = 0;
            c = qSqrt(c*c + Z*Z);
            dS5 = (128 - S5)/36.0;
            dS4 = (128 - S4)/36.0;
            dS3 = (128 - S3)/36.0;
            for (int i=0; i<Z; i++){
                zS5 += dS5;
                zS4 += dS4;
                zS3 += dS3;
            }
            S5 = round(S5+zS5);
            S4 = round(S4+zS4);
            S3 = round(S3+zS3);


            // kinematyka prosta z moodla
            Ph1 = ((S5-128)*M_PI/256)+(M_PI/2);
            Ph2 = -(S4-128)*(M_PI/256);
            Ph3 = (S3-128)*(M_PI/256);
            Ph0 = 0;

            R0[0][0] = qCos(Ph0);
            R0[0][2] = -qSin(Ph0);
            R0[1][1] = 1;
            R0[2][0] = -qSin(Ph0);
            R0[2][2] = qCos(Ph0);
            R0[3][3] = 1;

            R1[0][0] = qCos(Ph1);
            R1[0][1] = -qSin(Ph1);
            R1[1][0] = qSin(Ph1);
            R1[1][1] = qCos(Ph1);
            R1[2][2] = 1;
            R1[3][3] = 1;

            R2[0][0] = qCos(Ph2);
            R2[0][1] = -qSin(Ph2);
            R2[1][0] = qSin(Ph2);
            R2[1][1] = qCos(Ph2);
            R2[2][2] = 1;
            R2[3][3] = 1;

            R3[0][0] = qCos(Ph3);
            R3[0][1] = -qSin(Ph3);
            R3[1][0] = qSin(Ph3);
            R3[1][1] = qCos(Ph3);
            R3[2][2] = 1;
            R3[3][3] = 1;

            if (R3[0][0] != R3[0][0]){
            } else{
                // A0 A1 A2 A3
                for (int i=0; i<4; i++){
                    for (int j=0; j<4; j++){
                        A0[i][j] = 0;
                        for (int licznik=0; licznik<4; licznik++){
                            A0[i][j] += R0[i][licznik]*T0[licznik][j];
                        }
                    }
                }
                for (int i=0; i<4; i++){
                    for (int j=0; j<4; j++){
                        A1[i][j] = 0;
                        for (int licznik=0; licznik<4; licznik++){
                            A1[i][j] += R1[i][licznik]*T1[licznik][j];
                        }
                    }
                }
                for (int i=0; i<4; i++){
                    for (int j=0; j<4; j++){
                        A2[i][j] = 0;
                        for (int licznik=0; licznik<4; licznik++){
                            A2[i][j] += R2[i][licznik]*T2[licznik][j];
                        }
                    }
                }
                for (int i=0; i<4; i++){
                    for (int j=0; j<4; j++){
                        A3[i][j] = 0;
                        for (int licznik=0; licznik<4; licznik++){
                            A3[i][j] += R3[i][licznik]*T3[licznik][j];
                        }
                    }
                }


                // P1
                for (int i=0; i<4; i++){
                    for (int j=0; j<4; j++){
                        P1[i][j] = 0;
                        for (int licznik=0; licznik<4; licznik++){
                            P1[i][j] += A0[i][licznik]*A1[licznik][j];
                        }
                    }
                }

                // P2
                for (int i=0; i<4; i++){
                    for (int j=0; j<4; j++){
                        P2[i][j] = 0;
                        for (int licznik=0; licznik<4; licznik++){
                            P2[i][j] += P1[i][licznik]*A2[licznik][j];
                        }
                    }
                }

                // P3
                for (int i=0; i<4; i++){
                    for (int j=0; j<4; j++){
                        P3[i][j] = 0;
                        for (int licznik=0; licznik<4; licznik++){
                            P3[i][j] += P2[i][licznik]*A3[licznik][j];
                        }
                    }
                }
                if (P1[0][3] < 0.000001 && P1[0][3] > -0.000001)
                    xy_P1[0] = 0;
                else
                    xy_P1[0] = P1[0][3];
                xy_P1[1] = P1[1][3];
                if (P2[0][3] < 0.000001 && P2[0][3] > -0.000001)
                    xy_P2[0] = 0;
                else
                    xy_P2[0] = P2[0][3];
                xy_P2[1] = P2[1][3];
                if (P3[0][3] < 0.000001 && P3[0][3] > -0.000001)
                    xy_P3[0] = 0;
                else
                    xy_P3[0] = P3[0][3];
                xy_P3[1] = P3[1][3];


                emit ready_data();
                this->msleep(1);
            }

        }else
            this->msleep(20);
    }
}




