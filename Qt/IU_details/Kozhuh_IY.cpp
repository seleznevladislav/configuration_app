#include "../BuildMathModel.h"

using namespace BuildMathModel;

const double DEG_TO_RAD = M_PI / 180.0;

//MbPlacement3D pl;

/*MbAxis3D axisX(pl.GetAxisX());
MbAxis3D axisY(pl.GetAxisY());
MbAxis3D axisZ(pl.GetAxisZ());*/

//Создание цилиндра кожуха
static SPtr<MbSolid> CreateKzhBase(double D, double L_Base, double s) {
    double D_v = D + 2 * s;
    //цилиндр с внешним диаметром
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    SolidSPtr pCyl1;
    //MbSolid* pCyl1;
    SpacePointsVector arrPnts1;
    MbCartPoint3D p1, p2, p3;
    p1.Init(0, 0, 0);//основание
    p2.Init(L_Base, 0, 0);//высота
    p3.Init(L_Base, D_v / 2, 0);//радиус
    arrPnts1.clear();
    arrPnts1.push_back(p1);
    arrPnts1.push_back(p2);
    arrPnts1.push_back(p3);
    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, arrPnts1, operNames), pCyl1);

    //цилиндр с внутренним диаметром
    SolidSPtr pCyl2;
    SpacePointsVector arrPnts2;
    MbCartPoint3D p4, p5, p6;
    p4.Init(0, 0, 0);
    p5.Init(L_Base, 0, 0);
    p6.Init(L_Base, D / 2, 0);
    arrPnts2.clear();
    arrPnts2.push_back(p4);
    arrPnts2.push_back(p5);
    arrPnts2.push_back(p6);
    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, arrPnts2, operNames), pCyl2);

    SolidSPtr pSptrSolid;
    BooleanResult(pCyl1, cm_Copy, pCyl2, cm_Copy, MbBooleanOperationParams(bo_Difference, true, operNames), pSptrSolid);


    return pSptrSolid;
}

//Создание конусообразной части кожуха
static SPtr<MbSolid> CreateLoft(double D_Kzh, double DKr, double L, double s) {
    double Dvne = D_Kzh + 2 * s;
    double DKrvne = DKr + 2 * s;
    //ЛОФТ ПОБОЛЬШЕ
    RPArray<MbSurface> arrSurf;

    MbPlane* pPlaneXZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0),
        MbCartPoint3D(0, 0, 1));
    //pPlaneXZ->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 0, 0)));
    MbPlane* pPlaneXZ1 = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0),
        MbCartPoint3D(0, 0, 1));
    pPlaneXZ1->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(L, 0, 0)));

    arrSurf.push_back(pPlaneXZ);
    arrSurf.push_back(pPlaneXZ1);

    RPArray<MbContour> arrCircles;
    MbArc* pCircleKozhuhVnechn = new MbArc(MbCartPoint(0, 0), Dvne / 2);
    MbArc* pCircleCamereVnechn = new MbArc(MbCartPoint(-(Dvne - DKrvne) / 2, 0), DKrvne / 2);
    arrCircles.push_back(new MbContour(*pCircleKozhuhVnechn, true));
    arrCircles.push_back(new MbContour(*pCircleCamereVnechn, true));

    //твердое тело
    // Объект с параметрами операции заметания.
    LoftedValues params;

    // Объекты для именования элементов модели твердого тела.
    MbSNameMaker names(ct_CurveLoftedSolid, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> contourNames(0, 1, false);

    // Построение твердого тела заметания
    MbSolid* pLoft = nullptr;
    MbResultType resLoft = ::LoftedSolid(arrSurf, arrCircles,
        nullptr, params, nullptr, nullptr, names, contourNames, pLoft);

    //ЛОФТ ПОМЕНЬШЕ
    RPArray<MbSurface> arrSurfSmal;
    RPArray<MbContour> arrCirclesSmal;

    MbArc* pCircleKozhuhVnutr = new MbArc(MbCartPoint(0, 0), D_Kzh / 2);
    MbArc* pCircleCameraVnutr = new MbArc(MbCartPoint(-(Dvne - DKrvne) / 2, 0), DKr / 2);
    arrCirclesSmal.push_back(new MbContour(*pCircleKozhuhVnutr, true));
    arrCirclesSmal.push_back(new MbContour(*pCircleCameraVnutr, true));

    MbPlane* pPlaneXZs = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0), MbCartPoint3D(0, 0, 1));
    MbPlane* pPlaneXZ1s = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0), MbCartPoint3D(0, 0, 1));
    pPlaneXZ1s->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(L, 0, 0)));
    arrSurfSmal.push_back(pPlaneXZs);
    arrSurfSmal.push_back(pPlaneXZ1s);
    //твердое тело

    // Объект с параметрами операции заметания.
    LoftedValues paramSmal;

    // Объекты для именования элементов модели твердого тела.
    MbSNameMaker namesSmal(ct_CurveLoftedSolid, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> contourNamesSmal(0, 1, false);

    // Построение твердого тела заметания
    MbSolid* pLoftSmal = nullptr;
    MbResultType resLoftSmal = ::LoftedSolid(arrSurfSmal, arrCirclesSmal, nullptr, paramSmal, nullptr, nullptr, namesSmal, contourNamesSmal, pLoftSmal);

    //ВЫЧИТАНИЕ ДВУХ ЛОФТОВ
    MbSNameMaker operBoolNames(ct_BooleanSolid, MbSNameMaker::i_SideNone);

    SolidSPtr Loft(pLoft);
    SolidSPtr LoftSmal(pLoftSmal);
    SolidSPtr LOFT;

    BooleanResult(Loft, cm_Copy, LoftSmal, cm_Copy, MbBooleanOperationParams(bo_Difference, true, operBoolNames), LOFT);
    return LOFT;
}

SPtr<MbSolid> ParametricModelCreator::CreateMassivOtv(SPtr<MbSolid> Osnova, double b,
    double d, double D1, double Y, int num, MbAxis3D axis, double grad, double X) {
    // создание отверстия для основы массива
    SpacePointsVector arrPntsOtv;
    MbCartPoint3D p0, p1, p2;
    p0.Init(0, 0, 0); //центр нижнего основания
    p1.Init(0, b * 2, 0); //центр верхнего основания
    p2.Init(0, 0, d);

    arrPntsOtv.clear();
    arrPntsOtv.push_back(p0);
    arrPntsOtv.push_back(p1);
    arrPntsOtv.push_back(p2);

    SolidSPtr  pPart1;
    MbSNameMaker elNames(ct_ElementarySolid, MbSNameMaker::i_SideNone);
    ElementarySolid(MbElementarySolidParams(et_Cylinder, arrPntsOtv, elNames), pPart1);

    SolidSPtr pSolidMassivOtv;

    SolidSPtr duplSolid;

    MbSNameMaker duplNames(ct_DuplicationSolid, MbSNameMaker::i_SideNone, 0);
    MbVector3D vecDir1(MbCartPoint3D(0, 0, 0), MbCartPoint3D(-D1 / 2, 0, 0));
    MbVector3D vecDir2(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, -D1 / 2, 0));
    // Шаг копирования
    const double step1 = D1 / 2;
    const double step2 = 360 / num * DEG_TO_RAD;
    unsigned int num1 = 1;
    //unsigned int num2 = 8;
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);

    MbDuplicationSolidParams* parameters = new MbDuplicationSolidParams(true, vecDir1, step1, num1, vecDir2, step2, num, &p0, false);

    parameters->SetNameMaker(duplNames);

    ::DuplicationSolid(*pPart1, *parameters, duplSolid);
    duplSolid->Rotate(axis, grad * DEG_TO_RAD);
    duplSolid->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(X, Y, 0)));
    BooleanResult(Osnova, cm_Copy, duplSolid, cm_Copy,
        MbBooleanOperationParams(bo_Difference, true, operNames), pSolidMassivOtv);
    return  pSolidMassivOtv;
}


//Создание фланца
static SPtr<MbSolid> CreateTrubaOnKzh(int id, double D_Kzh, double p, double DKr, double s) {
    MbPlacement3D pl;

    double DKrVne = DKr + 2 * s;

    double Dy;
    double D;
    double D2;
    double b;
    double Dm;
    double Dn;
    double H;
    double D1;
    double d;
    int num;


    //сбоку на эллиптическом днище
    if (id == 1) {
        if (p == 1.6) {
            Dy = 200;
            D = 335;
            D2 = 268;
            b = 21;
            Dm = 240;
            Dn = 222;
            H = 170;
            D1 = 295;
            d = 23;
            num = 12;
        }
        else if (p == 2.5) {
            Dy = 200;
            D = 360;
            D2 = 278;
            b = 21;
            Dm = 245;
            Dn = 222;
            H = 180;
            D1 = 310;
            d = 27;
            num = 12;
        }
    }

    //третий снизу
    if (id == 4) {
        if (p == 1.6) {
            Dy = 50;
            D = 160;
            D2 = 102;
            b = 13;
            Dm = 76;
            Dn = 58;
            H = 110;
            D1 = 125;
            d = 18;
            num = 4;
        }
        else if (p == 2.5) {
            Dy = 50;
            D = 160;
            D2 = 102;
            b = 17;
            Dm = 76;
            Dn = 58;
            H = 110;
            D1 = 125;
            d = 18;
            num = 4;
        }
    }

    //второй сверху
    if (id == 10) {
        if (p == 1.6) {
            Dy = 80;
            D = 185;
            D2 = 128;
            b = 17;
            Dm = 110;
            Dn = 90;
            H = 130;
            D1 = 160;
            d = 18;
            num = 4;
        }
        else if (p == 2.5) {
            Dy = 80;
            D = 195;
            D2 = 138;
            b = 19;
            Dm = 110;
            Dn = 90;
            H = 130;
            D1 = 160;
            d = 18;
            num = 4;
        }
    }

    //первый сверху
    if (id == 11) {
        if (p == 1.6) {
            Dy = 500;
            D = 710;
            D2 = 610;
            b = 38;
            Dm = 559;
            Dn = 535;
            H = 220;
            D1 = 650;
            d = 33;
            num = 20;
        }
        else if (p == 2.5) {
            Dy = 500;
            D = 730;
            D2 = 615;
            b = 44;
            Dm = 570;
            Dn = 535;
            H = 220;
            D1 = 660;
            d = 40;
            num = 20;
        }
    }


    // на лофте
    if (id == 5) {
        if (p == 1.6) {
            if ((800 <= D_Kzh) && (D_Kzh < 1000)) {
                Dy = 200;
                D = 335;
                D2 = 268;
                b = 21;
                Dm = 240;
                Dn = 222;
                H = 180;
                D1 = 295;
                d = 23;
                num = 12;
            }
            else if ((1000 <= D_Kzh) && (D_Kzh < 1200)) {
                Dy = 200;
                D = 335;
                D2 = 268;
                b = 21;
                Dm = 240;
                Dn = 222;
                H = 180;
                D1 = 295;
                d = 23;
                num = 12;
            }
            else if ((1200 <= D_Kzh) && (D_Kzh < 1400)) {
                Dy = 250;
                D = 405;
                D2 = 320;
                b = 23;
                Dm = 292;
                Dn = 278;
                H = 180;
                D1 = 355;
                d = 23;
                num = 12;
            }
            else if ((1400 <= D_Kzh) && (D_Kzh < 1600)) {
                Dy = 250;
                D = 405;
                D2 = 320;
                b = 23;
                Dm = 292;
                Dn = 278;
                H = 180;
                D1 = 355;
                d = 23;
                num = 12;
            }

            else if ((1600 <= D_Kzh) && (D_Kzh < 1800)) {
                Dy = 250;
                D = 405;
                D2 = 320;
                b = 23;
                Dm = 292;
                Dn = 278;
                H = 180;
                D1 = 355;
                d = 23;
                num = 12;
            }

            else if ((1800 <= D_Kzh) && (D_Kzh < 2000)) {
                Dy = 250;
                D = 405;
                D2 = 320;
                b = 23;
                Dm = 292;
                Dn = 278;
                H = 180;
                D1 = 355;
                d = 23;
                num = 12;
            }

            else if (2000 <= D_Kzh) {
                Dy = 250;
                D = 405;
                D2 = 320;
                b = 23;
                Dm = 292;
                Dn = 278;
                H = 180;
                D1 = 355;
                d = 23;
                num = 12;

            }
        }

        else if (p == 2.5) {
            if ((800 <= D_Kzh) && (D_Kzh < 1000)) {
                Dy = 200;
                D = 360;
                D2 = 278;
                b = 21;
                Dm = 245;
                Dn = 222;
                H = 180;
                D1 = 310;
                d = 27;
                num = 12;
            }

            else if ((1000 <= D_Kzh) && (D_Kzh < 1200)) {
                Dy = 200;
                D = 360;
                D2 = 278;
                b = 27;
                Dm = 245;
                Dn = 222;
                H = 180;
                D1 = 310;
                d = 27;
                num = 12;
            }

            else if ((1200 <= D_Kzh) && (D_Kzh < 1400)) {
                Dy = 250;
                D = 425;
                D2 = 335;
                b = 29;
                Dm = 300;
                Dn = 278;
                H = 180;
                D1 = 370;
                d = 30;
                num = 12;
            }
            else if ((1400 <= D_Kzh) && (D_Kzh < 1600)) {
                Dy = 250;
                D = 425;
                D2 = 335;
                b = 29;
                Dm = 300;
                Dn = 278;
                H = 180;
                D1 = 370;
                d = 30;
                num = 12;
            }

            else if ((1600 <= D_Kzh) && (D_Kzh < 1800)) {
                Dy = 250;
                D = 425;
                D2 = 335;
                b = 29;
                Dm = 300;
                Dn = 278;
                H = 180;
                D1 = 370;
                d = 30;
                num = 12;
            }

            else if ((1800 <= D_Kzh) && (D_Kzh < 2000)) {
                Dy = 250;
                D = 425;
                D2 = 335;
                b = 29;
                Dm = 300;
                Dn = 278;
                H = 180;
                D1 = 370;
                d = 30;
                num = 12;
            }

            else if (2000 <= D_Kzh) {
                Dy = 250;
                D = 425;
                D2 = 335;
                b = 29;
                Dm = 300;
                Dn = 278;
                H = 180;
                D1 = 370;
                d = 30;
                num = 12;

            }
        }
    }

    //первый снизу
    if (id == 3) {
        if (p == 1.6) {
            if ((800 <= D_Kzh) && (D_Kzh < 1000)) {
                Dy = 100;
                D = 215;
                D2 = 158;
                b = 16;
                Dm = 130;
                Dn = 110;
                H = 180;
                D1 = 180;
                d = 18;
                num = 8;
            }

            else if ((1000 <= D_Kzh) && (D_Kzh < 1200)) {
                Dy = 100;
                D = 215;
                D2 = 158;
                b = 17;
                Dm = 130;
                Dn = 110;
                H = 180;
                D1 = 180;
                d = 18;
                num = 8;
            }

            else if ((1200 <= D_Kzh) && (D_Kzh < 1400)) {
                Dy = 150;
                D = 280;
                D2 = 212;
                b = 18;
                Dm = 180;
                Dn = 161;
                H = 180;
                D1 = 240;
                d = 22;
                num = 8;
            }

            else if ((1400 <= D_Kzh) && (D_Kzh < 1600)) {
                Dy = 150;
                D = 280;
                D2 = 212;
                b = 19;
                Dm = 180;
                Dn = 161;
                H = 180;
                D1 = 240;
                d = 22;
                num = 8;
            }

            else if ((1600 <= D_Kzh) && (D_Kzh < 1800)) {
                Dy = 150;
                D = 280;
                D2 = 212;
                b = 19;
                Dm = 180;
                Dn = 161;
                H = 180;
                D1 = 240;
                d = 22;
                num = 8;
            }
            else if ((1800 <= D_Kzh) && (D_Kzh < 2000)) {
                Dy = 150;
                D = 280;
                D2 = 212;
                b = 19;
                Dm = 180;
                Dn = 161;
                H = 180;
                D1 = 240;
                d = 22;
                num = 8;
            }

            else if (2000 <= D_Kzh) {
                Dy = 200;
                D = 335;
                D2 = 268;
                b = 21;
                Dm = 240;
                Dn = 222;
                H = 170;
                D1 = 295;
                d = 23;
                num = 8;

            }
        }
        else if (p == 2.5) {
            if ((800 <= D_Kzh) && (D_Kzh < 1000)) {
                Dy = 100;
                D = 230;
                D2 = 162;
                b = 21;
                Dm = 132;
                Dn = 110;
                H = 180;
                D1 = 190;
                d = 23;
                num = 8;
            }

            else if ((1000 <= D_Kzh) && (D_Kzh < 1200)) {
                Dy = 100;
                D = 230;
                D2 = 162;
                b = 21;
                Dm = 132;
                Dn = 110;
                H = 180;
                D1 = 190;
                d = 23;
                num = 8;
            }

            else if ((1200 <= D_Kzh) && (D_Kzh < 1400)) {
                Dy = 150;
                D = 300;
                D2 = 218;
                b = 25;
                Dm = 186;
                Dn = 161;
                H = 180;
                D1 = 250;
                d = 27;
                num = 8;
            }

            else if ((1400 <= D_Kzh) && (D_Kzh < 1600)) {
                Dy = 150;
                D = 300;
                D2 = 218;
                b = 25;
                Dm = 186;
                Dn = 161;
                H = 180;
                D1 = 250;
                d = 27;
                num = 8;
            }
            else if ((1600 <= D_Kzh) && (D_Kzh < 1800)) {
                Dy = 150;
                D = 300;
                D2 = 218;
                b = 25;
                Dm = 186;
                Dn = 161;
                H = 180;
                D1 = 250;
                d = 27;
                num = 8;
            }

            else if ((1800 <= D_Kzh) && (D_Kzh < 2000)) {
                Dy = 150;
                D = 300;
                D2 = 218;
                b = 25;
                Dm = 186;
                Dn = 161;
                H = 180;
                D1 = 250;
                d = 27;
                num = 8;
            }

            else if (2000 <= D_Kzh) {
                Dy = 200;
                D = 360;
                D2 = 278;
                b = 21;
                Dm = 245;
                Dn = 222;
                H = 180;
                D1 = 310;
                d = 27;
                num = 12;
            }
        }
    }

    //третий сверху
    if (id == 9) {
        if (p == 1.6) {
            if ((800 <= D_Kzh) && (D_Kzh < 1000)) {
                Dy = 250;
                D = 405;
                D2 = 320;
                b = 23;
                Dm = 292;
                Dn = 278;
                H = 200;
                D1 = 355;
                d = 23;
                num = 12;
            }

            else if ((1000 <= D_Kzh) && (D_Kzh < 1200)) {
                Dy = 300;
                D = 460;
                D2 = 378;
                b = 24;
                Dm = 346;
                Dn = 330;
                H = 180;
                D1 = 410;
                d = 27;
                num = 12;
            }

            else if ((1200 <= D_Kzh) && (D_Kzh < 1400)) {
                Dy = 350;
                D = 520;
                D2 = 438;
                b = 28;
                Dm = 400;
                Dn = 382;
                H = 180;
                D1 = 470;
                d = 27;
                num = 12;
            }

            else if ((1400 <= D_Kzh) && (D_Kzh < 1600)) {
                Dy = 400;
                D = 580;
                D2 = 490;
                b = 32;
                Dm = 450;
                Dn = 432;
                H = 220;
                D1 = 525;
                d = 30;
                num = 12;
            }

            else if ((1600 <= D_Kzh) && (D_Kzh < 1800)) {
                Dy = 400;
                D = 580;
                D2 = 490;
                b = 32;
                Dm = 450;
                Dn = 432;
                H = 220;
                D1 = 525;
                d = 30;
                num = 12;
            }

            else if ((1800 <= D_Kzh) && (D_Kzh < 2000)) {
                Dy = 400;
                D = 580;
                D2 = 490;
                b = 32;
                Dm = 450;
                Dn = 432;
                H = 220;
                D1 = 525;
                d = 30;
                num = 12;
            }

            else if (2000 <= D_Kzh) {
                Dy = 400;
                D = 580;
                D2 = 490;
                b = 32;
                Dm = 450;
                Dn = 432;
                H = 220;
                D1 = 525;
                d = 30;
                num = 12;

            }
        }
        else if (p == 2.5) {
            if ((800 <= D_Kzh) && (D_Kzh < 1000)) {
                Dy = 250;
                D = 425;
                D2 = 335;
                b = 30;
                Dm = 300;
                Dn = 278;
                H = 180;
                D1 = 370;
                d = 30;
                num = 12;
            }

            else if ((1000 <= D_Kzh) && (D_Kzh < 1200)) {
                Dy = 300;
                D = 485;
                D2 = 390;
                b = 32;
                Dm = 352;
                Dn = 330;
                H = 180;
                D1 = 430;
                d = 30;
                num = 12;
            }

            else if ((1200 <= D_Kzh) && (D_Kzh < 1400)) {
                Dy = 350;
                D = 550;
                D2 = 450;
                b = 36;
                Dm = 406;
                Dn = 382;
                H = 180;
                D1 = 490;
                d = 33;
                num = 12;
            }

            else if ((1400 <= D_Kzh) && (D_Kzh < 1600)) {
                Dy = 400;
                D = 610;
                D2 = 505;
                b = 40;
                Dm = 464;
                Dn = 432;
                H = 220;
                D1 = 550;
                d = 33;
                num = 12;
            }

            else if ((1600 <= D_Kzh) && (D_Kzh < 1800)) {
                Dy = 400;
                D = 610;
                D2 = 505;
                b = 40;
                Dm = 464;
                Dn = 432;
                H = 220;
                D1 = 550;
                d = 33;
                num = 12;
            }

            else if ((1800 <= D_Kzh) && (D_Kzh < 2000)) {
                Dy = 400;
                D = 610;
                D2 = 505;
                b = 40;
                Dm = 464;
                Dn = 432;
                H = 220;
                D1 = 550;
                d = 30;
                num = 12;
            }

            else if (2000 <= D_Kzh) {
                Dy = 400;
                D = 610;
                D2 = 505;
                b = 40;
                Dm = 464;
                Dn = 432;
                H = 220;
                D1 = 550;
                d = 33;
                num = 12;

            }
        }
    }

    //камера
    if ((id == 6) || (id == 7)) {
        if (p == 1.6) {
            if ((500 <= DKr) && (DKr < 600)) {
                Dy = 150;
                D = 280;
                D2 = 212;
                b = 19;
                Dm = 180;
                Dn = 161;
                H = 180;
                D1 = 240;
                d = 23;
                num = 8;
            }

            else if ((600 <= DKr) && (DKr < 700)) {
                Dy = 200;
                D = 335;
                D2 = 268;
                b = 21;
                Dm = 240;
                Dn = 222;
                H = 170;
                D1 = 295;
                d = 23;
                num = 8;
            }

            else if ((700 <= DKr) && (DKr < 800)) {
                Dy = 250;
                D = 405;
                D2 = 320;
                b = 23;
                Dm = 292;
                Dn = 278;
                H = 180;
                D1 = 355;
                d = 23;
                num = 8;
            }
            else if ((800 <= DKr) && (DKr < 900)) {
                Dy = 250;
                D = 405;
                D2 = 320;
                b = 23;
                Dm = 292;
                Dn = 278;
                H = 180;
                D1 = 355;
                d = 23;
                num = 8;
            }

            else if ((900 <= DKr) && (DKr < 1000)) {
                Dy = 300;
                D = 460;
                D2 = 378;
                b = 24;
                Dm = 346;
                Dn = 330;
                H = 200;
                D1 = 410;
                d = 27;
                num = 8;
            }

            else if ((1000 <= DKr) && (DKr < 1100)) {
                Dy = 250;
                D = 405;
                D2 = 320;
                b = 23;
                Dm = 292;
                Dn = 278;
                H = 180;
                D1 = 355;
                d = 23;
                num = 8;
            }
            else if ((1100 <= DKr) && (DKr < 1200)) {
                Dy = 250;
                D = 405;
                D2 = 320;
                b = 23;
                Dm = 292;
                Dn = 278;
                H = 180;
                D1 = 355;
                d = 23;
                num = 8;
            }
            else if (1200 <= DKr) {
                Dy = 250;
                D = 405;
                D2 = 320;
                b = 23;
                Dm = 292;
                Dn = 278;
                H = 180;
                D1 = 355;
                d = 23;
                num = 8;
            }
        }
        else if (p == 2.5) {
            if ((500 <= DKr) && (DKr < 600)) {
                Dy = 150;
                D = 300;
                D2 = 218;
                b = 25;
                Dm = 186;
                Dn = 161;
                H = 180;
                D1 = 250;
                d = 27;
                num = 8;
            }

            else if ((600 <= DKr) && (DKr < 700)) {
                Dy = 200;
                D = 360;
                D2 = 278;
                b = 27;
                Dm = 245;
                Dn = 222;
                H = 180;
                D1 = 310;
                d = 27;
                num = 8;
            }

            else if ((700 <= DKr) && (DKr < 800)) {
                Dy = 250;
                D = 425;
                D2 = 335;
                b = 29;
                Dm = 300;
                Dn = 278;
                H = 180;
                D1 = 370;
                d = 30;
                num = 8;
            }

            else if ((800 <= DKr) && (DKr < 900)) {
                Dy = 250;
                D = 425;
                D2 = 335;
                b = 29;
                Dm = 300;
                Dn = 278;
                H = 180;
                D1 = 370;
                d = 30;
                num = 8;
            }

            else if ((900 <= DKr) && (DKr < 1000)) {
                Dy = 300;
                D = 485;
                D2 = 390;
                b = 32;
                Dm = 352;
                Dn = 330;
                H = 200;
                D1 = 430;
                d = 30;
                num = 8;
            }

            else if ((1000 <= DKr) && (DKr < 1100)) {
                Dy = 250;
                D = 425;
                D2 = 335;
                b = 29;
                Dm = 300;
                Dn = 278;
                H = 180;
                D1 = 370;
                d = 30;
                num = 8;
            }

            else if ((1100 <= DKr) && (DKr < 1200)) {
                Dy = 250;
                D = 425;
                D2 = 335;
                b = 29;
                Dm = 300;
                Dn = 278;
                H = 180;
                D1 = 370;
                d = 30;
                num = 8;
            }
            else if (1200 <= DKr) {
                Dy = 250;
                D = 425;
                D2 = 335;
                b = 29;
                Dm = 300;
                Dn = 278;
                H = 180;
                D1 = 370;
                d = 30;
                num = 8;
            }
        }
    }

    //фланец на кожухе для соединения с камерой
    if (id == 13) {
        if ((500 <= DKr) && (DKr < 600)) {
            Dy = DKr;
            D = 723 + (DKr - 500);
            D2 = 605 + (DKr - 500);
            b = 40;
            Dm = 616 + (DKr - 500);
            Dn = DKrVne;
            H = 86;
            D1 = 650 + (DKr - 500);
            d = 26;
            num = 24;
        }
        else if ((600 <= DKr) && (DKr < 700)) {
            Dy = DKr;
            D = 840 + (DKr - 600);
            D2 = 720 + (DKr - 600);
            b = 41;
            Dm = 660 + (DKr - 600);
            Dn = DKrVne;
            H = 86;
            D1 = 770 + (DKr - 600);
            d = 26;
            num = 24;
        }

        else if ((700 <= DKr) && (DKr < 800)) {
            Dy = DKr;
            D = 940 + (DKr - 700);
            D2 = 820 + (DKr - 700);
            b = 41;
            Dm = 760 + (DKr - 700);
            Dn = DKrVne;
            H = 86;
            D1 = 870 + (DKr - 700);
            d = 26;
            num = 24;
        }

        else if ((800 <= DKr) && (DKr < 900)) {
            Dy = DKr;
            D = 1020 + (DKr - 800);
            D2 = 900 + (DKr - 800);
            b = 52;
            Dm = 844 + (DKr - 800);
            Dn = DKrVne;
            H = 86;
            D1 = 950 + (DKr - 800);
            d = 26;
            num = 24;
        }

        else if ((900 <= DKr) && (DKr < 1000)) {
            Dy = DKr;
            D = 1120 + (DKr - 900);
            D2 = 1000 + (DKr - 900);
            b = 63;
            Dm = 944 + (DKr - 900);
            Dn = DKrVne;
            H = 86;
            D1 = 1050 + (DKr - 900);
            d = 28;
            num = 24;
        }

        else if ((1000 <= DKr) && (DKr < 1100)) {
            Dy = DKr;
            D = 1255 + (DKr - 1000);
            D2 = 1110 + (DKr - 1000);
            b = 63;
            Dm = 1060 + (DKr - 1000);
            Dn = DKrVne;
            H = 86;
            D1 = 1170 + (DKr - 1000);
            d = 30;
            num = 24;
        }

        else if ((1100 <= DKr) && (DKr < 1200)) {
            Dy = DKr;
            D = 1355 + (DKr - 1100);
            D2 = 1210 + (DKr - 1100);
            b = 63;
            Dm = 1160 + (DKr - 1100);
            Dn = DKrVne;
            H = 86;
            D1 = 1270 + (DKr - 1100);
            d = 32;
            num = 24;
        }

        else if (1200 <= DKr) {
            Dy = DKr;
            D = 1485 + (DKr - 1200);
            D2 = 1325 + (DKr - 1200);
            b = 75;
            Dm = 1268 + (DKr - 1200);
            Dn = DKrVne;
            H = 86;
            D1 = 1390 + (DKr - 1200);
            d = 34;
            num = 24;
        }
    }




    SArray<MbCartPoint> arrPntss(9);
    arrPntss.Add(MbCartPoint(Dy / 2, H));
    arrPntss.Add(MbCartPoint(D2 / 2, H));
    arrPntss.Add(MbCartPoint(D2 / 2, H - 6.));
    arrPntss.Add(MbCartPoint(D / 2, H - 6));
    arrPntss.Add(MbCartPoint(D / 2, H - 6 - b));
    arrPntss.Add(MbCartPoint(Dm / 2, H - 6 - b));
    //2*(Dm-Dn)
    arrPntss.Add(MbCartPoint(Dn / 2, H - 6 - b - 41.));//тут еще нужно длину скоса, было 41
    arrPntss.Add(MbCartPoint(Dn / 2, 0));
    arrPntss.Add(MbCartPoint(Dy / 2, 0));


    MbPolyline* pPolyline = new MbPolyline(arrPntss, true);
    MbContour* pContourPolyline = new MbContour(*pPolyline, true);

    //// Данные об образующей операции движения.
    const double DEG_TO_RAD = M_PI / 180.0;
    MbSweptData sweptData(pl, *pContourPolyline);
    RevolutionValues revParms(360 * DEG_TO_RAD, 0, 0);

    // Именователи для операции построения тела вращения и для контуров образующей
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);

    MbAxis3D axis(pl.GetAxisY());
    MbSolid* pSolid = nullptr;
    MbResultType res = ::RevolutionSolid(sweptData, axis, revParms, operNames, cNames, pSolid);

    SolidSPtr pSolidBase(pSolid);

    MbAxis3D axisX(pl.GetAxisX());

    SolidSPtr pSptrSolid = ParametricModelCreator::CreateMassivOtv(pSolidBase, b / 2, d / 2, D1, H - 6. - b / 2, num, axisX, 0, 0);

    // arrPntss.clear();
    return pSptrSolid;
}


//Выдавливание между кожухом и фланцем
static SPtr<MbSolid> CreateTrubaOnKzhExtr(SPtr<MbSolid> Kzh, SPtr<MbSolid> Tr,
    double Dn, int id, double p, double D_Kzh, double DKr) {

    if (id == 1) {
        Dn = 222;
    }
    if (id == 4) {
        Dn = 58;
    }
    if (id == 10) {
        Dn = 90;
    }
    if (id == 11) {
        Dn = 535;
    }

    if (id == 3) {
        if ((800 <= D_Kzh) && (D_Kzh < 1000)) {
            Dn = 110;
        }

        else if ((1000 <= D_Kzh) && (D_Kzh < 1200)) {
            Dn = 110;
        }

        else if ((1200 <= D_Kzh) && (D_Kzh < 1400)) {
            Dn = 161;
        }

        else if ((1400 <= D_Kzh) && (D_Kzh < 1600)) {
            Dn = 161;
        }

        else if ((1600 <= D_Kzh) && (D_Kzh < 1800)) {
            Dn = 161;
        }

        else if ((1800 <= D_Kzh) && (D_Kzh < 2000)) {
            Dn = 161;
        }

        else if (2000 <= D_Kzh) {
            Dn = 222;
        }
    }

    if (id == 5) {
        if ((800 <= D_Kzh) && (D_Kzh < 1000)) {
            Dn = 222;
        }

        else if ((1000 <= D_Kzh) && (D_Kzh < 1200)) {
            Dn = 222;
        }

        else if ((1200 <= D_Kzh) && (D_Kzh < 1400)) {
            Dn = 278;
        }

        else if ((1400 <= D_Kzh) && (D_Kzh < 1600)) {
            Dn = 278;
        }

        else if ((1600 <= D_Kzh) && (D_Kzh < 1800)) {
            Dn = 278;
        }

        else if ((1800 <= D_Kzh) && (D_Kzh < 2000)) {
            Dn = 278;
        }

        else if (2000 <= D_Kzh) {
            Dn = 278;

        }
    }

    if ((id == 6) || (id == 7)) {
        if ((500 <= DKr) && (DKr < 600)) {
            Dn = 161;
        }
        else if ((600 <= DKr) && (DKr < 700)) {
            Dn = 222;
        }
        else if ((700 <= DKr) && (DKr < 800)) {
            Dn = 278;
        }
        else if ((800 <= DKr) && (DKr < 900)) {
            Dn = 278;
        }
        else if ((900 <= DKr) && (DKr < 1000)) {
            Dn = 330;
        }
        else if ((1000 <= DKr) && (DKr < 1100)) {
            Dn = 278;
        }
        else if ((1100 <= DKr) && (DKr < 1200)) {
            Dn = 278;
        }
        else if (1200 <= DKr) {
            Dn = 278;
        }
    }

    if (id == 9) {
        if ((800 <= D_Kzh) && (D_Kzh < 1000)) {
            Dn = 278;
        }

        else if ((1000 <= D_Kzh) && (D_Kzh < 1200)) {
            Dn = 330;
        }

        else if ((1200 <= D_Kzh) && (D_Kzh < 1400)) {
            Dn = 382;
        }

        else if ((1400 <= D_Kzh) && (D_Kzh < 1600)) {
            Dn = 432;
        }

        else if ((1600 <= D_Kzh) && (D_Kzh < 1800)) {
            Dn = 432;
        }

        else if ((1800 <= D_Kzh) && (D_Kzh < 2000)) {
            Dn = 432;
        }

        else if (2000 <= D_Kzh) {
            Dn = 432;
        }
    }

    MbArc* pArc = new MbArc(MbCartPoint(0, 0), Dn / 2.);
    MbContour* ptrContour = new MbContour();
    ptrContour->AddSegment(pArc);

    MbPlane* pPlaneXZ = new MbPlane(MbCartPoint3D(0, 0, 0),
        MbCartPoint3D(1, 0, 0), MbCartPoint3D(0, 0, 1));

    RPArray<MbContour> ptrConturs;
    ptrConturs.Add(ptrContour);
    MbSweptData sweptData(*pPlaneXZ, ptrConturs);

    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    //RPArray<MbSNameMaker> cNames(1, false);
    PArray<MbSNameMaker> cNames(0, 1, false);

    ExtrusionValues extrusionParam;
    extrusionParam.side1.way = sw_shell;

    //// Направление для построения тела:
    MbVector3D dir(0, -1, 0);

    //Kzh->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(-1000., -Dvne / 2, 0)));

    // Вызов функции-утилиты для построения тела выдавливания
    MbSolid* pSolidExtr = nullptr;

    MbResultType resProd = ::ExtrusionSolid(sweptData, dir, Kzh,
        nullptr, false, extrusionParam, operNames, cNames, pSolidExtr);

    SolidSPtr pSptrExtr(pSolidExtr);

    SolidSPtr pSptrSolid;
    BooleanResult(Tr, cm_Copy, pSptrExtr, cm_Copy, MbBooleanOperationParams(bo_Union, true, operNames), pSptrSolid);

    ::DeleteItem(pSolidExtr);
    //::DeleteItem(pPlaneXZ);

    return pSptrSolid;
}

static SPtr<MbSolid> CreateHole(SPtr<MbSolid> Base, double Dy, double D_Kzh, double DKr, double grad, double x, double y, int id) {
    if (id == 1) {
        Dy = 200;
    }
    if (id == 4) {
        Dy = 50;
    }
    if (id == 10) {
        Dy = 80;
    }
    if (id == 11) {
        Dy = 500;
    }

    if (id == 3) {
        if ((800 <= D_Kzh) && (D_Kzh < 1000)) {
            Dy = 100;
        }

        else if ((1000 <= D_Kzh) && (D_Kzh < 1200)) {
            Dy = 100;
        }

        else if ((1200 <= D_Kzh) && (D_Kzh < 1400)) {
            Dy = 150;
        }

        else if ((1400 <= D_Kzh) && (D_Kzh < 1600)) {
            Dy = 150;
        }

        else if ((1600 <= D_Kzh) && (D_Kzh < 1800)) {
            Dy = 150;
        }

        else if ((1800 <= D_Kzh) && (D_Kzh < 2000)) {
            Dy = 150;
        }

        else if (2000 <= D_Kzh) {
            Dy = 200;

        }
    }

    if (id == 5) {

        if ((800 <= D_Kzh) && (D_Kzh < 1000)) {
            Dy = 200;
        }

        else if ((1000 <= D_Kzh) && (D_Kzh < 1200)) {
            Dy = 200;
        }

        else if ((1200 <= D_Kzh) && (D_Kzh < 1400)) {
            Dy = 250;
        }

        else if ((1400 <= D_Kzh) && (D_Kzh < 1600)) {
            Dy = 250;
        }

        else if ((1600 <= D_Kzh) && (D_Kzh < 1800)) {
            Dy = 250;
        }

        else if ((1800 <= D_Kzh) && (D_Kzh < 2000)) {
            Dy = 250;
        }

        else if (2000 <= D_Kzh) {
            Dy = 250;

        }
    }

    if (id == 9) {
        if ((800 <= D_Kzh) && (D_Kzh < 1000)) {
            Dy = 250;
        }

        else if ((1000 <= D_Kzh) && (D_Kzh < 1200)) {
            Dy = 300;
        }

        else if ((1200 <= D_Kzh) && (D_Kzh < 1400)) {
            Dy = 350;
        }

        else if ((1400 <= D_Kzh) && (D_Kzh < 1600)) {
            Dy = 400;
        }

        else if ((1600 <= D_Kzh) && (D_Kzh < 1800)) {
            Dy = 400;
        }

        else if ((1800 <= D_Kzh) && (D_Kzh < 2000)) {
            Dy = 400;
        }

        else if (2000 <= D_Kzh) {
            Dy = 400;

        }
    }

    if ((id == 6) || (id == 7)) {
        if ((500 <= DKr) && (DKr < 600)) {
            Dy = 150;
        }
        else if ((600 <= DKr) && (DKr < 700)) {
            Dy = 200;
        }
        else if ((700 <= DKr) && (DKr < 800)) {
            Dy = 250;
        }
        else if ((800 <= DKr) && (DKr < 900)) {
            Dy = 250;
        }
        else if ((900 <= DKr) && (DKr < 1000)) {
            Dy = 300;
        }
        else if ((1000 <= DKr) && (DKr < 1100)) {
            Dy = 250;
        }
        else if ((1100 <= DKr) && (DKr < 1200)) {
            Dy = 250;
        }
        else if (1200 <= DKr) {
            Dy = 250;
        }
    }

    MbPlacement3D pl;
    MbAxis3D axisZ(pl.GetAxisZ());
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    SolidSPtr pCyl, pSptrSolid;
    SpacePointsVector arrPnts1;
    MbCartPoint3D p1, p2, p3;
    p1.Init(0, 0, 0);//основание
    p2.Init(110, 0, 0);//высота
    p3.Init(110, Dy / 2., 0);//радиус
    arrPnts1.clear();
    arrPnts1.push_back(p1);
    arrPnts1.push_back(p2);
    arrPnts1.push_back(p3);
    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, arrPnts1, operNames), pCyl);
    pCyl->Rotate(axisZ, grad * DEG_TO_RAD);
    pCyl->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(x, y, 0)));
    BooleanResult(Base, cm_Copy, pCyl, cm_Copy,
        MbBooleanOperationParams(bo_Difference, true, operNames), pSptrSolid);
    return pSptrSolid;
}


//DKr здесь внутренний диаметр камеры
static SPtr<MbSolid> CreateKzhWithTr(SPtr<MbSolid> Kzh, double D_Kzh, double L_Base, double p, double DKr, double s, double l, double l2, double l3) {
    MbPlacement3D pl;
    MbAxis3D axisX(pl.GetAxisX());
    MbAxis3D axisY(pl.GetAxisY());
    MbAxis3D axisZ(pl.GetAxisZ());

    double D_Kzh_V = D_Kzh + 2 * s;


    double l0 = 460;
    //int l = 350;
    //int l2 = 1750;
    //int l3 = 6390;

    /*
    if ((800 <= D_Kzh) && (D_Kzh < 1000)) {
        l0 = 460;
        l = 350;
        l2 = 1750;
        l3 = 6390;
    }
    else if ((1000 <= D_Kzh) && (D_Kzh < 1200)) {
        l0 = 525;
        l = 350;
        l2 = 1750;
        l3 = 6400;
    }
    else if ((1200 <= D_Kzh) && (D_Kzh < 1400)) {
        l0 = 525;
        l = 350;
        l2 = 1750;
        l3 = 6400;
    }

    else if ((1400 <= D_Kzh) && (D_Kzh < 1600)) {
        l0 = 525;
        l = 350;
        l2 = 1750;
        l3 = 6400;
    }

    else if ((1600 <= D_Kzh) && (D_Kzh < 1800)) {
        l0 = 525;
        l = 350;
        l2 = 1750;
        l3 = 6400;
    }

    else if ((1800 <= D_Kzh) && (D_Kzh < 2000)) {
        l0 = 525;
        l = 350;
        l2 = 1750;
        l3 = 6400;
    }

    else if (2000 <= D_Kzh) {
        l0 = 790;
        l = 400;
        l2 = 1610;
        l3 = 6560;
    }
   */

    double Dn = 100;//тоже условный

    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    SolidSPtr copyKzh = CreateKzhBase(D_Kzh, L_Base, 20.);

    //SolidSPtr Truba6 = CreateTrubaOnKzh(150., 280, 212, 19, 180, 161, ?, 240, 23, 8);
    int RADA = (int)D_Kzh_V / 4;

    SolidSPtr pSptrKzh1;
    Kzh->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(RADA, 0, 0)));//RADA -Elliptic Dno
    Kzh->Rotate(axisZ, 270 * DEG_TO_RAD);
    SolidSPtr TrubaBase1 = CreateTrubaOnKzh(1, D_Kzh, p, DKr, s);
    SolidSPtr Truba1 = CreateTrubaOnKzhExtr(Kzh, TrubaBase1, Dn, 1, p, D_Kzh, DKr);
    BooleanResult(Kzh, cm_Copy, Truba1, cm_Copy, MbBooleanOperationParams(bo_Union, true, operNames), pSptrKzh1);
    pSptrKzh1->Rotate(axisZ, -270 * DEG_TO_RAD);
    pSptrKzh1->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(-RADA, 0, 0)));//RADA -Elliptic Dno


    copyKzh->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(-1000., -D_Kzh_V / 2, 0)));

    SolidSPtr TrubaBase3 = CreateTrubaOnKzh(3, D_Kzh, p, DKr, s);
    SolidSPtr Truba3 = CreateTrubaOnKzhExtr(copyKzh, TrubaBase3, Dn, 3, p, D_Kzh, DKr);
    Truba3->Rotate(MbAxis3D(MbVector3D(MbCartPoint3D(1, 0, 0),
        MbCartPoint3D(0, 0, 1))), 180 * DEG_TO_RAD);
    Truba3->Rotate(axisY, 90 * DEG_TO_RAD);
    Truba3->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(l, -D_Kzh_V / 2, 0)));

    SolidSPtr TrubaBase4 = CreateTrubaOnKzh(4, D_Kzh, p, DKr, s);
    SolidSPtr Truba4 = CreateTrubaOnKzhExtr(copyKzh, TrubaBase4, Dn, 4, p, D_Kzh, DKr);
    Truba4->Rotate(MbAxis3D(MbVector3D(MbCartPoint3D(1, 0, 0),
        MbCartPoint3D(0, 0, 1))), 180 * DEG_TO_RAD);
    Truba4->Rotate(axisY, 90 * DEG_TO_RAD);
    Truba4->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(l2, -D_Kzh_V / 2, 0)));

    SolidSPtr pSptrKzh5;
    pSptrKzh1->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(-l3, D_Kzh_V / 2, 0)));//6390 - из ТУ l3
    pSptrKzh1->Rotate(axisX, 180 * DEG_TO_RAD);
    SolidSPtr TrubaBase5 = CreateTrubaOnKzh(5, D_Kzh, p, DKr, s);
    SolidSPtr Truba5 = CreateTrubaOnKzhExtr(pSptrKzh1, TrubaBase5, Dn, 5, p, D_Kzh, DKr);
    BooleanResult(pSptrKzh1, cm_Copy, Truba5, cm_Copy, MbBooleanOperationParams(bo_Union, true, operNames), pSptrKzh5);
    pSptrKzh5->Rotate(axisX, 180 * DEG_TO_RAD);
    pSptrKzh5->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(l3, -D_Kzh_V / 2, 0)));//6390 - из ТУ l3


    SolidSPtr TrubaBase9 = CreateTrubaOnKzh(9, D_Kzh, p, DKr, s);
    SolidSPtr Truba9 = CreateTrubaOnKzhExtr(copyKzh, TrubaBase9, Dn, 9, p, D_Kzh, DKr);
    Truba9->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(2300., D_Kzh_V / 2, 0)));

    SolidSPtr TrubaBase10 = CreateTrubaOnKzh(10, D_Kzh, p, DKr, s);
    SolidSPtr Truba10 = CreateTrubaOnKzhExtr(copyKzh, TrubaBase10, Dn, 10, p, D_Kzh, DKr);
    Truba10->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1500., D_Kzh_V / 2, 0)));

    SolidSPtr TrubaBase11 = CreateTrubaOnKzh(11, D_Kzh, p, DKr, s);
    SolidSPtr Truba11 = CreateTrubaOnKzhExtr(copyKzh, TrubaBase11, Dn, 11, p, D_Kzh, DKr);
    Truba11->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(700., D_Kzh_V / 2, 0)));

    SolidSPtr pSptrKzh3, pSptrKzh4, pSptrKzh9, pSptrKzh10, pSptrKzh11;
    BooleanResult(pSptrKzh5, cm_Copy, Truba3, cm_Copy, MbBooleanOperationParams(bo_Union, true, operNames), pSptrKzh3);
    BooleanResult(pSptrKzh3, cm_Copy, Truba4, cm_Copy, MbBooleanOperationParams(bo_Union, true, operNames), pSptrKzh4);
    BooleanResult(pSptrKzh4, cm_Copy, Truba9, cm_Copy, MbBooleanOperationParams(bo_Union, true, operNames), pSptrKzh9);
    BooleanResult(pSptrKzh9, cm_Copy, Truba10, cm_Copy, MbBooleanOperationParams(bo_Union, true, operNames), pSptrKzh10);
    BooleanResult(pSptrKzh10, cm_Copy, Truba11, cm_Copy, MbBooleanOperationParams(bo_Union, true, operNames), pSptrKzh11);

    return pSptrKzh11;
}

static SPtr<MbSolid> CreateHoleKzh(SPtr<MbSolid> Kzh, double D_Kzh, double L_Base, double DKr, double s, double l, double l2, double l3) {
    SolidSPtr pSptrKzh1, pSptrKzh3, pSptrKzh4, pSptrKzh5,
        pSptrKzh9, pSptrKzh10, pSptrKzh11;

    double D_Kzh_V = D_Kzh + 2 * s;

    double l0 = 800;

    /*
    if ((800 <= D_Kzh) && (D_Kzh < 1000)) {
        l0 = 460;
        l = 350;
        l2 = 1750;
        l3 = 6390;
    }
    else if ((1000 <= D_Kzh) && (D_Kzh < 1200)) {
        l0 = 525;
        l = 350;
        l2 = 1750;
        l3 = 6400;
    }
    else if ((1200 <= D_Kzh) && (D_Kzh < 1400)) {
        l0 = 525;
        l = 350;
        l2 = 1750;
        l3 = 6400;
    }

    else if ((1400 <= D_Kzh) && (D_Kzh < 1600)) {
        l0 = 525;
        l = 350;
        l2 = 1750;
        l3 = 6400;
    }

    else if ((1600 <= D_Kzh) && (D_Kzh < 1800)) {
        l0 = 525;
        l = 350;
        l2 = 1750;
        l3 = 6400;
    }

    else if ((1800 <= D_Kzh) && (D_Kzh < 2000)) {
        l0 = 525;
        l = 350;
        l2 = 1750;
        l3 = 6400;
    }

    else if (2000 <= D_Kzh) {
        l0 = 790;
        l = 400;
        l2 = 1610;
        l3 = 6560;
    }*/

    double Dy = 100;//условно, внутри функций будет меняться все равно

   // int RADA = (int)D_Kzh_V / 4;
    double RADA = D_Kzh_V / 4.;
    pSptrKzh1 = CreateHole(Kzh, Dy, D_Kzh, DKr, 0, -RADA, 0, 1);//Kozhuh, Dy, Rad elDno, x, y
    pSptrKzh3 = CreateHole(pSptrKzh1, Dy, D_Kzh, DKr, 90., l, -D_Kzh_V / 2, 3);
    pSptrKzh4 = CreateHole(pSptrKzh3, Dy, D_Kzh, DKr, 90., l2, -D_Kzh_V / 2, 4);
    pSptrKzh5 = CreateHole(pSptrKzh4, Dy, D_Kzh, DKr, 90., l3, -D_Kzh_V / 2, 5);
    pSptrKzh9 = CreateHole(pSptrKzh5, Dy, D_Kzh, DKr, -90., 2300, D_Kzh_V / 2, 9);
    pSptrKzh10 = CreateHole(pSptrKzh9, Dy, D_Kzh, DKr, -90., 1500, D_Kzh_V / 2, 10);
    pSptrKzh11 = CreateHole(pSptrKzh10, Dy, D_Kzh, DKr, -90., 700, D_Kzh_V / 2, 11);

    return pSptrKzh11;
}

static SPtr<MbSolid> CreateEllipticDno(double D, double s) {
    const double DEG_TO_RAD = M_PI / 180.0;

    double D_v = D + 2 * s;

    //H эллипса
    double RADA = D_v / 4.;
    double RADC = D / 4.;
    //int RADA = (int)D_v / 4;
    //int RADC = (int)D / 4;

    double RADB = D_v / 2.;
    double RADD = D / 2.;

    //внешняя дуга
    MbCartPoint p1(-RADA, 0);
    MbCartPoint p2(0, RADB);
    // const double RADA = 227, RADB = 420;

    MbPlacement pl;
    MbArc* Seg1 = new MbArc(RADA, RADB, pl, p1, p2, -1);
    MbContour* ptrContour = new MbContour();
    ptrContour->AddSegment(Seg1);

    //внутренняя дуга
    //const double RADC = 203, RADD = 400;
    MbCartPoint p5(0, RADD);
    MbCartPoint p6(-RADC, 0);
    MbArc* Seg5 = new MbArc(RADC, RADD, pl, p5, p6, 1);
    MbContour* ptrContour5 = new MbContour();
    ptrContour5->AddSegment(Seg5);

    MbPlacement3D place;
    MbPlane* ptrSurface = new MbPlane(place);

    //Создание образующей для тела вращения
    RPArray<MbContour> ptrContours;
    ptrContours.Add(ptrContour);

    RPArray<MbContour> ptrContours5;
    ptrContours5.Add(ptrContour5);

    //объект, в котором хранятся сведения об образующей
    MbSweptData* pCurves;
    pCurves = new MbSweptData(*ptrSurface, ptrContours);

    MbSweptData* pCurves5;
    pCurves5 = new MbSweptData(*ptrSurface, ptrContours5);

    // Объект параметров для построения тел вращения.
    RevolutionValues revParms(360 * DEG_TO_RAD, 0, 0);

    //Именователи для операции построения тела вращения и для контуров образующей
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);

    //Ось вращения для построения тела
    MbAxis3D axis(place.GetAxisX());

    // Вызов функции-утилиты для построения твердого тела вращения
    MbSolid* m_pResSolid = nullptr;
    ::RevolutionSolid(*pCurves, axis, revParms, operNames, cNames, m_pResSolid);

    MbSolid* m_pResSolid5 = nullptr;
    ::RevolutionSolid(*pCurves5, axis, revParms, operNames, cNames, m_pResSolid5);

    // Уменьшение счетчиков ссылок динамических объектов ядра
    c3d::SolidSPtr MainSolid(m_pResSolid);
    c3d::SolidSPtr MainSolid5(m_pResSolid5);
    c3d::SolidSPtr EllipticDno;

    BooleanResult(MainSolid, cm_Copy, MainSolid5, cm_Copy,
        MbBooleanOperationParams(bo_Difference, true, operNames), EllipticDno);

    SolidSPtr Elliptic(EllipticDno);


    ::DeleteItem(m_pResSolid);
    ::DeleteItem(m_pResSolid5);

    return Elliptic;
}


SPtr<MbSolid> ParametricModelCreator::CreateUnionKozhuh_IU(ConfigParams_IU params) {
    const double s = params.S;
    const double D_Kzh = params.diam;
    const double D_Kam = params.D_Kam;
    const double D_Kam_V = D_Kam + 2 * s;
    const double D_Kzh_V = D_Kzh + 2 * s;
    const double L = params.Length;
    const double DKr = params.D_Kam;

    double l5 = 320.;

    if ((500 <= DKr) && (DKr < 600)) {
        l5 = 320.;
    }
    else if ((600 <= DKr) && (DKr < 700)) {
        l5 = 380.;
    }
    else if ((700 <= DKr) && (DKr < 800)) {
        l5 = 440.;
    }
    else if ((800 <= DKr) && (DKr < 900)) {
        l5 = 500.;
    }
    else if ((900 <= DKr) && (DKr < 1000)) {
        l5 = 560.;
    }
    else if ((1000 <= DKr) && (DKr < 1100)) {
        l5 = 570.;
    }
    else if ((1100 <= DKr) && (DKr < 1200)) {
        l5 = 600.;
    }
    else if (1200 <= DKr) {
        l5 = 610.;
    }

    double pol_Kam = l5 - params.D_Kam / 4;
    double L_Kzh = L - pol_Kam;
    double L_Loft = (L - pol_Kam) / 5.23;
    double L_Base = (L - pol_Kam) / 1.23;

    MbPlacement3D pl;
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    //Создание обечайки
    SolidSPtr Kozhuh = CreateKzhBase(D_Kzh, L_Base, s);
    //Лофт
    SolidSPtr Loft = CreateLoft(D_Kzh, DKr, L_Loft, s);
    Loft->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(L_Base, 0, 0)));
    SolidSPtr pSptrKzh, pSptrKzhEl;
    //Объединение кожуха и лофта
    BooleanResult(Kozhuh, cm_Copy, Loft, cm_Copy,
        MbBooleanOperationParams(bo_Union, true, operNames), pSptrKzh);
    //Создание эллиптического днища
    SolidSPtr ElDno = CreateEllipticDno(D_Kzh, s);
    BooleanResult(pSptrKzh, cm_Copy, ElDno, cm_Copy,
        MbBooleanOperationParams(bo_Union, true, operNames), pSptrKzhEl);
    //Создание фланцев
    SolidSPtr pSptrTr = CreateKzhWithTr(pSptrKzhEl, D_Kzh, L_Base, params.p, DKr, s,
        params.l, params.l2, params.l3);
    //Создани отверстий в фланцах
    SolidSPtr pKzhWithOtv = CreateHoleKzh(pSptrTr, D_Kzh, L_Base, DKr, s, params.l,
        params.l2, params.l3);
    //Создание фланца на лофте, который соединяется с камерой
    SolidSPtr pSptrFlanecOnLoft = CreateTrubaOnKzh(13, D_Kzh, params.p, DKr, s);
    MbAxis3D axisZ(pl.GetAxisZ());
    pSptrFlanecOnLoft->Rotate(axisZ, 270 * DEG_TO_RAD);
    pSptrFlanecOnLoft->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(L_Base +
        L_Loft, -(D_Kzh_V - D_Kam_V) / 2., 0)));
    SolidSPtr pSptrSolid;
    BooleanResult(pKzhWithOtv, cm_Copy, pSptrFlanecOnLoft, cm_Copy,
        MbBooleanOperationParams(bo_Union, true, operNames), pSptrSolid);
    return pSptrSolid;
}


static SPtr<MbSolid> CreatePeregorodkaForKamera(double D, double L, SolidSPtr kamera) {
    MbPlane* pPlaneZY = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 0, 1), MbCartPoint3D(0, 1, 0));
    pPlaneZY->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(L, 0, 0)));//86 это H у присоединительного фланца

    MbCartPoint p1P(D / 2, 0);
    MbCartPoint p2P(D / 2, 12);//12 это толщина перегородки
    MbCartPoint p3P(-D / 2, 12);
    MbCartPoint p4P(-D / 2, 0);


    //Динамическое создание объектов отрезков
    MbLineSegment* Seg1P = new MbLineSegment(p1P, p2P);
    MbLineSegment* Seg2P = new MbLineSegment(p2P, p3P);
    MbLineSegment* Seg3P = new MbLineSegment(p3P, p4P);
    MbLineSegment* Seg4P = new MbLineSegment(p4P, p1P);

    //Динамическое создание контура
    MbContour* ptrContourP = new MbContour();

    ptrContourP->AddSegment(Seg1P);
    ptrContourP->AddSegment(Seg2P);
    ptrContourP->AddSegment(Seg3P);
    ptrContourP->AddSegment(Seg4P);


    //Создание образующей для тела вращения
    RPArray<MbContour>* ptrContourPs = new RPArray<MbContour>();
    ptrContourPs->Add(ptrContourP);

    MbSweptData sweptDataP(*pPlaneZY, *ptrContourPs);

    ExtrusionValues extrusionParam1;
    extrusionParam1.side1.way = sw_shell;
    //// Направление для построения тела:
    MbVector3D dir1(-1, 0, 0);

    MbSNameMaker operNames3(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames3(0, 1, false);



    // Вызов функции-утилиты для построения тела выдавливания
    MbSolid* pSolidProd = nullptr;
    MbResultType resProd = ::ExtrusionSolid(sweptDataP, dir1, kamera, nullptr, true, extrusionParam1, operNames3, cNames3, pSolidProd);
    SolidSPtr pSolidPeregorodka(pSolidProd);

    return pSolidPeregorodka;
}

//double Dvne, double L, double s
SPtr<MbSolid> ParametricModelCreator::CreateKamera(ConfigParams_IU params) {
    const double s = params.S;
    const double D_Kzh = params.diam;
    const double D_Kam = params.D_Kam;
    const double D_Kam_V = params.D_Kam + 2 * s;
    const double D_Kr = params.D_Kam;

    double l5;

    if ((500 <= D_Kam) && (D_Kam < 600)) {
        l5 = 320;
    }
    else if ((600 <= D_Kam) && (D_Kam < 700)) {
        l5 = 380;
    }
    else if ((700 <= D_Kam) && (D_Kam < 800)) {
        l5 = 440;
    }
    else if ((800 <= D_Kam) && (D_Kam < 900)) {
        l5 = 500;
    }
    else if ((900 <= D_Kam) && (D_Kam < 1000)) {
        l5 = 560;
    }
    else if ((1000 <= D_Kam) && (D_Kam < 1100)) {
        l5 = 570;
    }
    else if ((1100 <= D_Kam) && (D_Kam < 1200)) {
        l5 = 600;
    }
    else if (1200 <= D_Kam) {
        l5 = 610;
    }

    double pol_Kam = l5 - params.D_Kam / 4;


    const double L = 2 * pol_Kam;
    MbPlacement3D pl;

    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);

    SolidSPtr pKzhEl;

    SolidSPtr pBase = CreateKzhBase(D_Kr, L, s);

    //Создание эллиптического днища 
    SolidSPtr ElDno = CreateEllipticDno(D_Kr, s);
    BooleanResult(pBase, cm_Copy, ElDno, cm_Copy,
        MbBooleanOperationParams(bo_Union, true, operNames), pKzhEl);

    //создание присоединительного фланца
    SolidSPtr pPrisFlanec = CreateTrubaOnKzh(13, D_Kam_V, params.p, D_Kr, s);

    MbAxis3D axisZ(pl.GetAxisZ());
    pPrisFlanec->Rotate(axisZ, 270 * DEG_TO_RAD);
    pPrisFlanec->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(L, 0, 0)));
    SolidSPtr pKzhPrisFl;
    BooleanResult(pKzhEl, cm_Copy, pPrisFlanec, cm_Copy,
        MbBooleanOperationParams(bo_Union, true, operNames), pKzhPrisFl);



    //Создание фланца 7 (верхний)
    SolidSPtr TrubaBase = CreateTrubaOnKzh(7, D_Kam_V, params.p, D_Kr, s);
    // TrubaBase->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(L/2, Dvne / 2, 0)));

    pKzhPrisFl->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(-L / 2, -D_Kam_V / 2, 0)));//здесь вышла ошибка
    SolidSPtr Truba = CreateTrubaOnKzhExtr(pKzhPrisFl, TrubaBase, 161, 7, params.p, D_Kam_V, D_Kr);
    SolidSPtr pKzhFl1;
    SolidSPtr pKzhFl2;
    BooleanResult(Truba, cm_Copy, pKzhPrisFl, cm_Copy,
        MbBooleanOperationParams(bo_Union, true, operNames), pKzhFl1);
    pKzhFl1->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(L / 2, D_Kam_V / 2, 0)));

    MbAxis3D axisX(pl.GetAxisX());

    Truba->Rotate(axisX, 180 * DEG_TO_RAD);
    Truba->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(L / 2, -D_Kam_V / 2, 0)));
    BooleanResult(pKzhFl1, cm_Copy, Truba, cm_Copy,
        MbBooleanOperationParams(bo_Union, true, operNames), pKzhFl2);



    SolidSPtr HoleInTruba = CreateHole(pKzhFl2, 150, D_Kam_V, D_Kr, -90., L / 2, D_Kam_V / 2., 6);//axisZ
    SolidSPtr HoleInTruba1 = CreateHole(HoleInTruba, 150., D_Kam_V, D_Kr, 90., L / 2, -D_Kam_V / 2., 7);

    //Создание перегородки
    SolidSPtr Peregorodka = CreatePeregorodkaForKamera(D_Kam_V - 2 * s, L + 86, HoleInTruba1);//86 это высота фланца
    SolidSPtr pKzhPer;
    BooleanResult(HoleInTruba1, cm_Copy, Peregorodka, cm_Copy,
        MbBooleanOperationParams(bo_Union, true, operNames), pKzhPer);

    return pKzhPer;
}