#include "../BuildMathModel.h"

using namespace BuildMathModel;

MbPlacement3D pl;

MbAxis3D axisX(pl.GetAxisX());
MbAxis3D axisY(pl.GetAxisY());
MbAxis3D axisZ(pl.GetAxisZ());

MbPlane* pPlaneXY = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1, 0, 0), MbCartPoint3D(0, 1, 0));
MbPlane* pPlaneXZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1, 0, 0), MbCartPoint3D(0, 0, 1));
MbPlane* pPlaneYZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0), MbCartPoint3D(0, 0, 1));

//координаты для плоскостей
MbCartPoint3D zero = MbCartPoint3D(0, 0, 0);
MbCartPoint3D x = MbCartPoint3D(1, 0, 0);
MbCartPoint3D y = MbCartPoint3D(0, 1, 0);
MbCartPoint3D z = MbCartPoint3D(0, 0, 1);

const double DEG_TO_RAD = M_PI / 180.0;



//Круговой массив отверстий
static SPtr<MbSolid> CreateMassivOtv_IP(SPtr<MbSolid> Osnova, double b,
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

    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);

    MbDuplicationSolidParams* parameters = new MbDuplicationSolidParams
    (true, vecDir1, step1, num1, vecDir2, step2, num, &p0, false);

    parameters->SetNameMaker(duplNames);

    ::DuplicationSolid(*pPart1, *parameters, duplSolid);

    duplSolid->Rotate(axis, grad * DEG_TO_RAD);
    duplSolid->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(X, Y, 0)));
    BooleanResult(Osnova, cm_Copy, duplSolid, cm_Copy,
        MbBooleanOperationParams(bo_Difference, true, operNames), pSolidMassivOtv);
    return  pSolidMassivOtv;
}

static SPtr<MbSolid> CreateKzhBase(int Dv_Kzh, double L_Base, double s) {

    int Dvne = Dv_Kzh + s * 2;

    //цилиндр с внешним диаметром
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    SolidSPtr pCyl1;
    //MbSolid* pCyl1;
    SpacePointsVector arrPnts1;
    MbCartPoint3D p1, p2, p3;
    p1.Init(0, 0, 0);//основание
    p2.Init(L_Base, 0, 0);//высота
    p3.Init(L_Base, Dvne / 2, 0);//радиус
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
    p6.Init(L_Base, Dvne / 2 - s, 0);
    arrPnts2.clear();
    arrPnts2.push_back(p4);
    arrPnts2.push_back(p5);
    arrPnts2.push_back(p6);
    ::ElementarySolid(MbElementarySolidParams(et_Cylinder, arrPnts2, operNames), pCyl2);

    SolidSPtr pSptrSolid;
    BooleanResult(pCyl1, cm_Copy, pCyl2, cm_Copy, MbBooleanOperationParams(bo_Difference, true, operNames), pSptrSolid);


    return pSptrSolid;
}

// Создание конусообразной части кожуха
static SPtr<MbSolid> CreateLoft(int Dv_Kzh, int DKrvne, int DKr, double L, double s) {

    int Dvne = Dv_Kzh + s * 2;
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

    MbArc* pCircleKozhuhVnutr = new MbArc(MbCartPoint(0, 0), Dv_Kzh / 2);
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


//Создание фланца
static SPtr<MbSolid> CreateTubeOnKzh(int id, double p, int DKr, double s, int Dv_Kzh) {
    MbPlacement3D pl;

    int DKrVne = DKr + 2 * s;

    double Dy = 123;
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
            if ((800 <= Dv_Kzh) && (Dv_Kzh < 1000)) {
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
            else if ((1000 <= Dv_Kzh) && (Dv_Kzh < 1200)) {
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
            else if ((1200 <= Dv_Kzh) && (Dv_Kzh < 1400)) {
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
            else if ((1400 <= Dv_Kzh) && (Dv_Kzh < 1600)) {
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

            else if ((1600 <= Dv_Kzh) && (Dv_Kzh < 1800)) {
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

            else if ((1800 <= Dv_Kzh) && (Dv_Kzh < 2000)) {
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

            else if (2000 <= Dv_Kzh) {
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
            if ((800 <= Dv_Kzh) && (Dv_Kzh < 1000)) {
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

            else if ((1000 <= Dv_Kzh) && (Dv_Kzh < 1200)) {
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

            else if ((1200 <= Dv_Kzh) && (Dv_Kzh < 1400)) {
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
            else if ((1400 <= Dv_Kzh) && (Dv_Kzh < 1600)) {
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

            else if ((1600 <= Dv_Kzh) && (Dv_Kzh < 1800)) {
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

            else if ((1800 <= Dv_Kzh) && (Dv_Kzh < 2000)) {
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

            else if (2000 <= Dv_Kzh) {
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
            if ((800 <= Dv_Kzh) && (Dv_Kzh < 1000)) {
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

            else if ((1000 <= Dv_Kzh) && (Dv_Kzh < 1200)) {
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

            else if ((1200 <= Dv_Kzh) && (Dv_Kzh < 1400)) {
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

            else if ((1400 <= Dv_Kzh) && (Dv_Kzh < 1600)) {
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

            else if ((1600 <= Dv_Kzh) && (Dv_Kzh < 1800)) {
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
            else if ((1800 <= Dv_Kzh) && (Dv_Kzh < 2000)) {
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

            else if (2000 <= Dv_Kzh) {
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
            if ((800 <= Dv_Kzh) && (Dv_Kzh < 1000)) {
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

            else if ((1000 <= Dv_Kzh) && (Dv_Kzh < 1200)) {
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

            else if ((1200 <= Dv_Kzh) && (Dv_Kzh < 1400)) {
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

            else if ((1400 <= Dv_Kzh) && (Dv_Kzh < 1600)) {
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
            else if ((1600 <= Dv_Kzh) && (Dv_Kzh < 1800)) {
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

            else if ((1800 <= Dv_Kzh) && (Dv_Kzh < 2000)) {
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

            else if (2000 <= Dv_Kzh) {
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
            if ((800 <= Dv_Kzh) && (Dv_Kzh < 1000)) {
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

            else if ((1000 <= Dv_Kzh) && (Dv_Kzh < 1200)) {
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

            else if ((1200 <= Dv_Kzh) && (Dv_Kzh < 1400)) {
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

            else if ((1400 <= Dv_Kzh) && (Dv_Kzh < 1600)) {
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

            else if ((1600 <= Dv_Kzh) && (Dv_Kzh < 1800)) {
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

            else if ((1800 <= Dv_Kzh) && (Dv_Kzh < 2000)) {
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

            else if (2000 <= Dv_Kzh) {
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
            if ((800 <= Dv_Kzh) && (Dv_Kzh < 1000)) {
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

            else if ((1000 <= Dv_Kzh) && (Dv_Kzh < 1200)) {
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

            else if ((1200 <= Dv_Kzh) && (Dv_Kzh < 1400)) {
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

            else if ((1400 <= Dv_Kzh) && (Dv_Kzh < 1600)) {
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

            else if ((1600 <= Dv_Kzh) && (Dv_Kzh < 1800)) {
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

            else if ((1800 <= Dv_Kzh) && (Dv_Kzh < 2000)) {
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

            else if (2000 <= Dv_Kzh) {
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
                num = 12;
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
                num = 12;
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
                num = 12;
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
                num = 12;
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
                num = 12;
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
                num = 12;
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
            Dy = 600;
            D = 840;
            D2 = 720;
            b = 41;
            Dm = 660;
            Dn = DKrVne;
            H = 86;
            D1 = 770;
            d = 26;
            num = 24;
        }

        else if ((700 <= DKr) && (DKr < 800)) {
            Dy = 700;
            D = 940;
            D2 = 820;
            b = 41;
            Dm = 760;
            Dn = DKrVne;
            H = 86;
            D1 = 870;
            d = 26;
            num = 24;
        }

        else if ((800 <= DKr) && (DKr < 900)) {
            Dy = 800;
            D = 1020;
            D2 = 900;
            b = 52;
            Dm = 844;
            Dn = DKrVne;
            H = 86;
            D1 = 950;
            d = 26;
            num = 24;
        }

        else if ((900 <= DKr) && (DKr < 1000)) {
            Dy = 900;
            D = 1120;
            D2 = 1000;
            b = 63;
            Dm = 944;
            Dn = DKrVne;
            H = 86;
            D1 = 1050;
            d = 28;
            num = 24;
        }

        else if ((1000 <= DKr) && (DKr < 1100)) {
            Dy = 1000;
            D = 1255;
            D2 = 1110;
            b = 63;
            Dm = 1060;
            Dn = DKrVne;
            H = 86;
            D1 = 1170;
            d = 30;
            num = 24;
        }

        else if ((1100 <= DKr) && (DKr < 1200)) {
            Dy = 1100;
            D = 1355;
            D2 = 1210;
            b = 63;
            Dm = 1160;
            Dn = DKrVne;
            H = 86;
            D1 = 1270;
            d = 32;
            num = 24;
        }

        else if (1200 <= DKr) {
            Dy = DKr;
            D = 1485;
            D2 = 1325;
            b = 75;
            Dm = 1268;
            Dn = DKrVne;
            H = 86;
            D1 = 1390;
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
    arrPntss.Add(MbCartPoint(Dn / 2, H - 6 - b - 41));//тут еще нужно длину скоса
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

    SolidSPtr pSptrSolid = CreateMassivOtv_IP(pSolidBase, b / 2, d / 2, D1, H - 6. - b / 2, num, axisX, 0, 0);

    // arrPntss.clear();
    return pSptrSolid;
}

//Выдавливание между кожухом и фланцем
static SPtr<MbSolid> CreateTubeOnKzhExtr(SPtr<MbSolid> Kzh, SPtr<MbSolid> Tr,
    double Dn, int id, double p, int DKr, int Dv_Kzh) {

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
        if ((800 <= Dv_Kzh) && (Dv_Kzh < 1000)) {
            Dn = 110;
        }

        else if ((1000 <= Dv_Kzh) && (Dv_Kzh < 1200)) {
            Dn = 110;
        }

        else if ((1200 <= Dv_Kzh) && (Dv_Kzh < 1400)) {
            Dn = 161;
        }

        else if ((1400 <= Dv_Kzh) && (Dv_Kzh < 1600)) {
            Dn = 161;
        }

        else if ((1600 <= Dv_Kzh) && (Dv_Kzh < 1800)) {
            Dn = 161;
        }

        else if ((1800 <= Dv_Kzh) && (Dv_Kzh < 2000)) {
            Dn = 161;
        }

        else if (2000 <= Dv_Kzh) {
            Dn = 222;

        }
    }

    if (id == 5) {
        if ((800 <= Dv_Kzh) && (Dv_Kzh < 1000)) {
            Dn = 222;
        }

        else if ((1000 <= Dv_Kzh) && (Dv_Kzh < 1200)) {
            Dn = 222;
        }

        else if ((1200 <= Dv_Kzh) && (Dv_Kzh < 1400)) {
            Dn = 278;
        }

        else if ((1400 <= Dv_Kzh) && (Dv_Kzh < 1600)) {
            Dn = 278;
        }

        else if ((1600 <= Dv_Kzh) && (Dv_Kzh < 1800)) {
            Dn = 278;
        }

        else if ((1800 <= Dv_Kzh) && (Dv_Kzh < 2000)) {
            Dn = 278;
        }

        else if (2000 <= Dv_Kzh) {
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
        if ((800 <= Dv_Kzh) && (Dv_Kzh < 1000)) {
            Dn = 278;
        }

        else if ((1000 <= Dv_Kzh) && (Dv_Kzh < 1200)) {
            Dn = 330;
        }

        else if ((1200 <= Dv_Kzh) && (Dv_Kzh < 1400)) {
            Dn = 382;
        }

        else if ((1400 <= Dv_Kzh) && (Dv_Kzh < 1600)) {
            Dn = 432;
        }

        else if ((1600 <= Dv_Kzh) && (Dv_Kzh < 1800)) {
            Dn = 432;
        }

        else if ((1800 <= Dv_Kzh) && (Dv_Kzh < 2000)) {
            Dn = 432;
        }

        else if (2000 <= Dv_Kzh) {
            Dn = 432;
        }
    }

    MbArc* pArc = new MbArc(MbCartPoint(0, 0), Dn / 2);
    MbContour* ptrContour = new MbContour();
    //ptrContour->Clear();
    ptrContour->AddSegment(pArc);

    //Из-за этого не работала
    //RPArray<MbContour>* ptrContours = new RPArray<MbContour>();
    //ptrContours->clear();
    //ptrContours->Add(ptrContour);

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

    //Kzh->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1000., Dvne / 2, 0)));

    ::DeleteItem(pSolidExtr);
    //::DeleteItem(pPlaneXZ);

    return pSptrSolid;
}

//
static SPtr<MbSolid> CreateHole(SPtr<MbSolid> Base, int Dy, int DKr, double grad, double x, double y, int id, int Dv_Kzh) {

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
        if ((800 <= Dv_Kzh) && (Dv_Kzh < 1000)) {
            Dy = 100;
        }

        else if ((1000 <= Dv_Kzh) && (Dv_Kzh < 1200)) {
            Dy = 100;
        }

        else if ((1200 <= Dv_Kzh) && (Dv_Kzh < 1400)) {
            Dy = 150;
        }

        else if ((1400 <= Dv_Kzh) && (Dv_Kzh < 1600)) {
            Dy = 150;
        }

        else if ((1600 <= Dv_Kzh) && (Dv_Kzh < 1800)) {
            Dy = 150;
        }

        else if ((1800 <= Dv_Kzh) && (Dv_Kzh < 2000)) {
            Dy = 150;
        }

        else if (2000 <= Dv_Kzh) {
            Dy = 200;

        }
    }

    if (id == 5) {

        if ((800 <= Dv_Kzh) && (Dv_Kzh < 1000)) {
            Dy = 200;
        }

        else if ((1000 <= Dv_Kzh) && (Dv_Kzh < 1200)) {
            Dy = 200;
        }

        else if ((1200 <= Dv_Kzh) && (Dv_Kzh < 1400)) {
            Dy = 250;
        }

        else if ((1400 <= Dv_Kzh) && (Dv_Kzh < 1600)) {
            Dy = 250;
        }

        else if ((1600 <= Dv_Kzh) && (Dv_Kzh < 1800)) {
            Dy = 250;
        }

        else if ((1800 <= Dv_Kzh) && (Dv_Kzh < 2000)) {
            Dy = 250;
        }

        else if (2000 <= Dv_Kzh) {
            Dy = 250;

        }
    }

    if (id == 9) {
        if ((800 <= Dv_Kzh) && (Dv_Kzh < 1000)) {
            Dy = 250;
        }

        else if ((1000 <= Dv_Kzh) && (Dv_Kzh < 1200)) {
            Dy = 300;
        }

        else if ((1200 <= Dv_Kzh) && (Dv_Kzh < 1400)) {
            Dy = 350;
        }

        else if ((1400 <= Dv_Kzh) && (Dv_Kzh < 1600)) {
            Dy = 400;
        }

        else if ((1600 <= Dv_Kzh) && (Dv_Kzh < 1800)) {
            Dy = 400;
        }

        else if ((1800 <= Dv_Kzh) && (Dv_Kzh < 2000)) {
            Dy = 400;
        }

        else if (2000 <= Dv_Kzh) {
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

    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    SolidSPtr pCyl, pSptrSolid;
    SpacePointsVector arrPnts1;
    MbCartPoint3D p1, p2, p3;
    p1.Init(0, 0, 0);//основание
    p2.Init(110, 0, 0);//высота
    p3.Init(110, Dy / 2, 0);//радиус
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

//Кожух вместе с фланцами
static SPtr<MbSolid> CreateKzhWithTubes(SPtr<MbSolid> Kzh, double L_Base, double p, int DKr, double Dn, double s, int Dv_Kzh, int l2, int l3) {

    int Dvne = Dv_Kzh + s * 2;

    MbPlacement3D pl;
    MbAxis3D axisX(pl.GetAxisX());
    MbAxis3D axisY(pl.GetAxisY());
    MbAxis3D axisZ(pl.GetAxisZ());

    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    SolidSPtr copyKzh = CreateKzhBase(Dv_Kzh, 5395., 20.);


    int RADA = (int)Dvne / 4;

    SolidSPtr pSptrKzh1;
    Kzh->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(RADA, 0, 0)));//RADA -Elliptic Dno
    Kzh->Rotate(axisZ, 270 * DEG_TO_RAD);
    SolidSPtr TrubaBase1 = CreateTubeOnKzh(1, p, DKr, s, Dv_Kzh);
    SolidSPtr Truba1 = CreateTubeOnKzhExtr(Kzh, TrubaBase1, Dn, 1, p, DKr, Dv_Kzh);
    BooleanResult(Kzh, cm_Copy, Truba1, cm_Copy, MbBooleanOperationParams(bo_Union, true, operNames), pSptrKzh1);
    pSptrKzh1->Rotate(axisZ, -270 * DEG_TO_RAD);
    pSptrKzh1->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(-RADA, 0, 0)));//RADA -Elliptic Dno


    copyKzh->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(-1000., -Dvne / 2, 0)));

    SolidSPtr TrubaBase3 = CreateTubeOnKzh(3, p, DKr, s, Dv_Kzh);
    SolidSPtr Truba3 = CreateTubeOnKzhExtr(copyKzh, TrubaBase3, Dn, 3, p, DKr, Dv_Kzh);
    Truba3->Rotate(MbAxis3D(MbVector3D(MbCartPoint3D(1, 0, 0),
        MbCartPoint3D(0, 0, 1))), 180 * DEG_TO_RAD);
    Truba3->Rotate(axisY, 90 * DEG_TO_RAD);
    Truba3->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(350, -Dvne / 2, 0)));

    SolidSPtr TrubaBase4 = CreateTubeOnKzh(4, p, DKr, s, Dv_Kzh);
    SolidSPtr Truba4 = CreateTubeOnKzhExtr(copyKzh, TrubaBase4, Dn, 4, p, DKr, Dv_Kzh);
    Truba4->Rotate(MbAxis3D(MbVector3D(MbCartPoint3D(1, 0, 0),
        MbCartPoint3D(0, 0, 1))), 180 * DEG_TO_RAD);
    Truba4->Rotate(axisY, 90 * DEG_TO_RAD);
    Truba4->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(l2, -Dvne / 2, 0)));

    SolidSPtr pSptrKzh5;
    pSptrKzh1->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(-l3, Dvne / 2, 0)));//6390 - из ТУ l3
    pSptrKzh1->Rotate(axisX, 180 * DEG_TO_RAD);
    SolidSPtr TrubaBase5 = CreateTubeOnKzh(5, p, DKr, s, Dv_Kzh);
    SolidSPtr Truba5 = CreateTubeOnKzhExtr(pSptrKzh1, TrubaBase5, Dn, 5, p, DKr, Dv_Kzh);
    BooleanResult(pSptrKzh1, cm_Copy, Truba5, cm_Copy, MbBooleanOperationParams(bo_Union, true, operNames), pSptrKzh5);
    pSptrKzh5->Rotate(axisX, 180 * DEG_TO_RAD);
    pSptrKzh5->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(l3, -Dvne / 2, 0)));//6390 - из ТУ l3


    SolidSPtr TrubaBase9 = CreateTubeOnKzh(9, p, DKr, s, Dv_Kzh);
    SolidSPtr Truba9 = CreateTubeOnKzhExtr(copyKzh, TrubaBase9, Dn, 9, p, DKr, Dv_Kzh);
    Truba9->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(2300., Dvne / 2, 0)));

    SolidSPtr TrubaBase10 = CreateTubeOnKzh(10, p, DKr, s, Dv_Kzh);
    SolidSPtr Truba10 = CreateTubeOnKzhExtr(copyKzh, TrubaBase10, Dn, 10, p, DKr, Dv_Kzh);
    Truba10->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1500., Dvne / 2, 0)));

    SolidSPtr TrubaBase11 = CreateTubeOnKzh(11, p, DKr, s, Dv_Kzh);
    SolidSPtr Truba11 = CreateTubeOnKzhExtr(copyKzh, TrubaBase11, Dn, 11, p, DKr, Dv_Kzh);
    Truba11->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(700., Dvne / 2, 0)));

    SolidSPtr pSptrKzh3, pSptrKzh4, pSptrKzh9, pSptrKzh10, pSptrKzh11;
    BooleanResult(pSptrKzh5, cm_Copy, Truba3, cm_Copy, MbBooleanOperationParams(bo_Union, true, operNames), pSptrKzh3);
    BooleanResult(pSptrKzh3, cm_Copy, Truba4, cm_Copy, MbBooleanOperationParams(bo_Union, true, operNames), pSptrKzh4);
    BooleanResult(pSptrKzh4, cm_Copy, Truba9, cm_Copy, MbBooleanOperationParams(bo_Union, true, operNames), pSptrKzh9);
    BooleanResult(pSptrKzh9, cm_Copy, Truba10, cm_Copy, MbBooleanOperationParams(bo_Union, true, operNames), pSptrKzh10);
    BooleanResult(pSptrKzh10, cm_Copy, Truba11, cm_Copy, MbBooleanOperationParams(bo_Union, true, operNames), pSptrKzh11);

    return pSptrKzh11;
}

//Отверстие во фланцах на кожухе
static SPtr<MbSolid> CreateHoleKzh(SPtr<MbSolid> Kzh, double L_Base, double Dy, int DKr, int Dv_Kzh, double s, int l2, int l3) {
    SolidSPtr pSptrKzh1, pSptrKzh3, pSptrKzh4, pSptrKzh5,
        pSptrKzh9, pSptrKzh10, pSptrKzh11;
    int Dvne = Dv_Kzh + s * 2;

    int RADA = (int)Dvne / 4;
    pSptrKzh1 = CreateHole(Kzh, Dy, DKr, 0, -RADA, 0, 1, Dv_Kzh);//Kozhuh, Dy, Rad elDno, x, y
    pSptrKzh3 = CreateHole(pSptrKzh1, Dy, DKr, 90., 350, -Dvne / 2, 3, Dv_Kzh);
    pSptrKzh4 = CreateHole(pSptrKzh3, Dy, DKr, 90., l2, -Dvne / 2, 4, Dv_Kzh);
    pSptrKzh5 = CreateHole(pSptrKzh4, Dy, DKr, 90., l3, -Dvne / 2, 5, Dv_Kzh);
    pSptrKzh9 = CreateHole(pSptrKzh5, Dy, DKr, -90., 2300, Dvne / 2, 9, Dv_Kzh);
    pSptrKzh10 = CreateHole(pSptrKzh9, Dy, DKr, -90., 1500, Dvne / 2, 10, Dv_Kzh);
    pSptrKzh11 = CreateHole(pSptrKzh10, Dy, DKr, -90., 700, Dvne / 2, 11, Dv_Kzh);

    return pSptrKzh11;
}

//Эллиптическое дно
static SPtr<MbSolid> CreateEllipticBottom(int Dv_Kzh, int s) {
    const double DEG_TO_RAD = M_PI / 180.0;

    int Dvne = Dv_Kzh + s * 2;

    //H эллипса
    int RADA = (int)Dvne / 4;
    int RADC = (int)(Dvne - 2 * s) / 4;

    int RADB = Dvne / 2;
    int RADD = Dvne / 2 - s;

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

//Итоговый кожух
SPtr<MbSolid> ParametricModelCreator::CreateUnionKzh_IP(int Dv_Kzh, double L_Base,
    int s, double p, int DKr, double Dn, double Dy, int l2, int l3) {

    int Dvne = Dv_Kzh + s * 2;

    int l5;

    if ((500 <= DKr) && (DKr < 600)) {
        l5 = 460;
    }
    else if ((600 <= DKr) && (DKr < 700)) {
        l5 = 550;
    }
    else if ((700 <= DKr) && (DKr < 800)) {
        l5 = 620;
    }
    else if ((800 <= DKr) && (DKr < 900)) {
        l5 = 700;
    }
    else if ((900 <= DKr) && (DKr < 1000)) {
        l5 = 760;
    }
    else if ((1000 <= DKr) && (DKr < 1100)) {
        l5 = 780;
    }
    else if ((1100 <= DKr) && (DKr < 1200)) {
        l5 = 840;
    }
    else if (1200 <= DKr) {
        l5 = 890;
    }


    double L_Loft = (L_Base - l5 / 2 - 86) / 5.23;
    double L = (L_Base - l5 / 2 - 86) / 1.23;
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    SolidSPtr Kozhuh = CreateKzhBase(Dv_Kzh, L, s);
    SolidSPtr Loft = CreateLoft(Dv_Kzh, DKr + s * 2, DKr, L_Loft, s);
    //5395 - координата где заканчивается кожух
    Loft->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(L, 0, 0)));
    SolidSPtr pSptrKzh, pSptrKzhEl;
    BooleanResult(Kozhuh, cm_Copy, Loft, cm_Copy,
        MbBooleanOperationParams(bo_Union, true, operNames), pSptrKzh);
    SolidSPtr ElDno = CreateEllipticBottom(Dv_Kzh, s);
    BooleanResult(pSptrKzh, cm_Copy, ElDno, cm_Copy,
        MbBooleanOperationParams(bo_Union, true, operNames), pSptrKzhEl);
    SolidSPtr pSptrTr = CreateKzhWithTubes(pSptrKzhEl, L, p, DKr, Dn, s, Dv_Kzh, l2, l3);
    SolidSPtr pKzhWithOtv = CreateHoleKzh(pSptrTr, L, Dy, DKr, Dv_Kzh, s, l2, l3);
    //создание фланца на лофте, который соединяется с камерой - НОМЕР 13 
    SolidSPtr pSptrFlanecOnLoft = CreateTubeOnKzh(13, p, DKr, s, Dv_Kzh);
    pSptrFlanecOnLoft->Rotate(axisZ, 270 * DEG_TO_RAD);
    pSptrFlanecOnLoft->Move(MbVector3D(MbCartPoint3D(0, 0, 0),
        MbCartPoint3D(L + L_Loft, -(Dvne - DKr - s * 2) / 2, 0)));
    SolidSPtr pSptrSolid;
    BooleanResult(pKzhWithOtv, cm_Copy, pSptrFlanecOnLoft, cm_Copy,
        MbBooleanOperationParams(bo_Union, true, operNames), pSptrSolid);
    return pSptrSolid;
}

//Труба
SPtr<MbSolid> ParametricModelCreator::CreatePipe_IP(int Dv_Kzh, double p, int DKr) {
    const int d = 21, dvne = 25; //для основы тела
    const int height = 6000; //высота
    int bigD;


    if ((500 <= DKr) && (DKr < 600)) {
        bigD = 366 + (DKr - 500);
    }
    else if ((600 <= DKr) && (DKr < 700)) {
        bigD = 462;
    }
    else if ((700 <= DKr) && (DKr < 800)) {
        bigD = 560;
    }
    else if ((800 <= DKr) && (DKr < 900)) {
        bigD = 652;
    }
    else if ((900 <= DKr) && (DKr < 1000)) {
        bigD = 750;
    }
    else if ((1000 <= DKr) && (DKr < 1100)) {
        bigD = 854;
    }
    else if ((1100 <= DKr) && (DKr < 1200)) {
        bigD = 948;
    }
    else if (1200 <= DKr) {
        bigD = 1052;
    }


    RPArray<MbContour>* ptrContours = new RPArray<MbContour>();
    double x;
    double y;
    int t = 32;
    double shag = sqrt(2) * t;
    int n = floor(bigD / shag); // Кол-во отверстий на 0 ряду - тот, что в середине
    float h = bigD / 2;

    for (int j = 1; j < n; j++)
    {
        for (int i = 0; i < 1000; i++)
        {
            x = shag * n - i * shag - 0.5 * shag * j;

            y = j * shag / 2 + 8;

            double l = sqrt(pow(x, 2) + pow(y, 2));

            if ((l + d / 2) <= h && y < h)
            {
                MbArc* Circle = new MbArc(MbCartPoint(x, y), dvne / 2);
                MbContour* ptrContour = new MbContour();
                ptrContour->AddSegment(Circle);
                ptrContours->Add(ptrContour);

                MbArc* Circle1 = new MbArc(MbCartPoint(x, y), d / 2);
                MbContour* ptrContour1 = new MbContour();
                ptrContour1->AddSegment(Circle1);
                ptrContours->Add(ptrContour1);

                MbArc* Circle2 = new MbArc(MbCartPoint(x, -y), dvne / 2);
                MbContour* ptrContour2 = new MbContour();
                ptrContour2->AddSegment(Circle2);
                ptrContours->Add(ptrContour2);

                MbArc* Circle3 = new MbArc(MbCartPoint(x, -y), d / 2);
                MbContour* ptrContour3 = new MbContour();
                ptrContour3->AddSegment(Circle3);
                ptrContours->Add(ptrContour3);

            }
        }
    }

    MbPlacement3D pl;
    MbPlane* pPlaneXY = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1, 0, 0), MbCartPoint3D(0, 1, 0));
    MbAxis3D axisY(pl.GetAxisY());

    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);
    MbVector3D dir(0, 0, -1);
    ExtrusionValues extrusionParam(height, 0);
    MbSweptData sweptData(*pPlaneXY, *ptrContours);

    MbSolid* pSolid = nullptr;
    ::ExtrusionSolid(sweptData, dir, nullptr, nullptr, false, extrusionParam, operNames, cNames, pSolid);

    pSolid->Rotate(axisY, -90 * DEG_TO_RAD, nullptr);

    SolidSPtr MainSolid(pSolid);

    return MainSolid;

}

//Крышки
//Основное тело
static SPtr<MbSolid> CreateBaseCover(int Dy, double p) {

    double b;
    double d2;
    double D2;
    double D;
    double d;
    double D1;
    double h;
    double h1;

    if (p == 1.6) {
        switch (Dy) {
        case 50:
            b = 14;
            d2 = 46;
            D2 = 102;
            D = 160;
            d = 18;
            D1 = 125;
            h = 3;
            h1 = 2;
            break;
        case 65:
            b = 14;
            d2 = 60;
            D2 = 122;
            D = 180;
            d = 18;
            D1 = 145;
            h = 3;
            h1 = 2;
            break;
        case 80:
            b = 14;
            d2 = 76;
            D2 = 138;
            D = 195;
            d = 18;
            D1 = 160;
            h = 3;
            h1 = 2;
            break;
        case 100:
            b = 16;
            d2 = 94;
            D2 = 185;
            D = 215;
            d = 18;
            D1 = 180;
            h = 3;
            h1 = 2;
            break;
        case 125:
            b = 16;
            d2 = 118;
            D2 = 188;
            D = 245;
            d = 18;
            D1 = 210;
            h = 3;
            h1 = 2;
            break;
        case 150:
            b = 18;
            d2 = 142;
            D2 = 212;
            D = 280;
            d = 23;
            D1 = 240;
            h = 3;
            h1 = 2;
            break;
        case 200:
            b = 20;
            d2 = 196;
            D2 = 268;
            D = 335;
            d = 23;
            D1 = 295;
            h = 3;
            h1 = 2;
            break;
        case 250:
            b = 24;
            d2 = 244;
            D2 = 320;
            D = 405;
            d = 23;
            D1 = 355;
            h = 3;
            h1 = 2;
            break;
        case 300:
            b = 28;
            d2 = 294;
            D2 = 378;
            D = 460;
            d = 27;
            D1 = 410;
            h = 4;
            h1 = 3;
            break;
        case 350:
            b = 32;
            d2 = 344;
            D2 = 438;
            D = 520;
            d = 27;
            D1 = 470;
            h = 4;
            h1 = 3;
            break;
        case 400:
            b = 34;
            d2 = 390;
            D2 = 490;
            D = 580;
            d = 30;
            D1 = 525;
            h = 4;
            h1 = 3;
            break;
        case 500:
            b = 40;
            d2 = 490;
            D2 = 610;
            D = 710;
            d = 33;
            D1 = 650;
            h = 4;
            h1 = 3;
            break;
        case 600:
            b = 45;
            d2 = 590;
            D2 = 720;
            D = 840;
            d = 40;
            D1 = 770;
            h = 5;
            h1 = 4;
            break;
        case 800:
            b = 52;
            d2 = 780;
            D2 = 900;
            D = 1020;
            d = 40;
            D1 = 950;
            h = 5;
            h1 = 4;
            break;
        }
    }
    else if (p == 2.5) {
        switch (Dy) {
        case 50:
            b = 14;
            d2 = 46;
            D2 = 102;
            D = 160;
            d = 18;
            D1 = 125;
            h = 3;
            h1 = 2;
            break;
        case 65:
            b = 16;
            d2 = 60;
            D2 = 122;
            D = 180;
            d = 18;
            D1 = 145;
            h = 3;
            h1 = 2;
            break;
        case 80:
            b = 18;
            d2 = 76;
            D2 = 138;
            D = 195;
            d = 18;
            D1 = 160;
            h = 3;
            h1 = 2;
            break;
        case 100:
            b = 20;
            d2 = 94;
            D2 = 162;
            D = 230;
            d = 23;
            D1 = 190;
            h = 3;
            h1 = 2;
            break;
        case 125:
            b = 22;
            d2 = 118;
            D2 = 188;
            D = 270;
            d = 27;
            D1 = 220;
            h = 3;
            h1 = 2;
            break;
        case 150:
            b = 24;
            d2 = 142;
            D2 = 218;
            D = 300;
            d = 27;
            D1 = 250;
            h = 3;
            h1 = 2;
            break;
        case 200:
            b = 26;
            d2 = 196;
            D2 = 278;
            D = 360;
            d = 30;
            D1 = 310;
            h = 3;
            h1 = 2;
            break;
        case 250:
            b = 30;
            d2 = 244;
            D2 = 335;
            D = 425;
            d = 30;
            D1 = 370;
            h = 3;
            h1 = 2;
            break;
        case 300:
            b = 34;
            d2 = 294;
            D2 = 390;
            D = 485;
            d = 30;
            D1 = 430;
            h = 4;
            h1 = 3;
            break;
        case 350:
            b = 38;
            d2 = 344;
            D2 = 450;
            D = 550;
            d = 33;
            D1 = 490;
            h = 4;
            h1 = 3;
            break;
        case 400:
            b = 40;
            d2 = 390;
            D2 = 505;
            D = 610;
            d = 33;
            D1 = 550;
            h = 4;
            h1 = 3;
            break;
        case 500:
            b = 48;
            d2 = 490;
            D2 = 615;
            D = 730;
            d = 40;
            D1 = 660;
            h = 4;
            h1 = 3;
            break;
        case 600:
            b = 50;
            d2 = 590;
            D2 = 720;
            D = 840;
            d = 40;
            D1 = 770;
            h = 5;
            h1 = 4;
            break;
        case 800:
            b = 63;
            d2 = 780;
            D2 = 930;
            D = 1075;
            d = 46;
            D1 = 990;
            h = 5;
            h1 = 4;
            break;
        }


    }



    // Построение основы тела
    SArray<MbCartPoint> arrPnts(8);
    arrPnts.Add(MbCartPoint(0, 0));
    arrPnts.Add(MbCartPoint(0, b + h + h1));
    arrPnts.Add(MbCartPoint(d2 / 2, b + h + h1));
    arrPnts.Add(MbCartPoint((d2 + h * 2) / 2, b + h1));
    arrPnts.Add(MbCartPoint(D2 / 2, b + h1));
    arrPnts.Add(MbCartPoint((D2 + h1 * 2) / 2, b));
    arrPnts.Add(MbCartPoint(D / 2, b));
    arrPnts.Add(MbCartPoint(D / 2, 0));

    MbPolyline* pPolyline = new MbPolyline(arrPnts, true);
    MbContour* pContourPolyline = new MbContour(*pPolyline, true);

    MbSweptData sweptData(pl, *pContourPolyline);
    RevolutionValues params(360 * DEG_TO_RAD, 0, 0);

    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);

    MbAxis3D axis(pl.GetAxisY());

    MbSolid* pSolidOsnovaRev = nullptr;
    ::RevolutionSolid(sweptData, axis, params, operNames, cNames, pSolidOsnovaRev);
    SolidSPtr pSptrSolid(pSolidOsnovaRev);
    return pSptrSolid;
}

//Создание центрального цилиндра для устранения отверстия
static SPtr<MbSolid> CreateCylForCover(SPtr<MbSolid> OsnMassiv, double b,
    double d) {

    // заполнение центра
    SpacePointsVector arrPnt2;
    MbCartPoint3D p0, p1, p2;
    p0.Init(0, 0, 0); //центр нижнего основания
    p1.Init(0, b, 0); //центр верхнего основания
    p2.Init(0, 0, d);

    arrPnt2.clear();
    arrPnt2.push_back(p0);
    arrPnt2.push_back(p1);
    arrPnt2.push_back(p2);

    SolidSPtr  pPart2;
    MbSNameMaker elNames(ct_ElementarySolid, MbSNameMaker::i_SideNone);
    ElementarySolid(MbElementarySolidParams(et_Cylinder, arrPnt2, elNames), pPart2);

    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    SolidSPtr pKRnaKamereRes;
    BooleanResult(OsnMassiv, cm_Copy, pPart2, cm_Copy, MbBooleanOperationParams(bo_Union, true, operNames), pKRnaKamereRes);

    return pKRnaKamereRes;
}

//Создание итоговой крышки
SPtr<MbSolid> ParametricModelCreator::CreateUnionCover_IP(int Dy, double p, int id, int Dv_Kzh, int DKr) {

    int b;
    int d;
    int D1;
    int num;

    //сбоку на эллиптическом днище
    if (id == 1) {
        Dy = 200;
    }

    //второй снизу
    if (id == 4) {
        Dy = 50;
    }

    //второй сверху
    if (id == 10) {
        Dy = 80;
    }

    //первый сверху
    if (id == 11) {
        Dy = 500;
    }

    // на лофте
    if (id == 5) {
        if ((800 <= Dv_Kzh) && (Dv_Kzh < 1000)) {
            Dy = 200;
        }

        else if ((1000 <= Dv_Kzh) && (Dv_Kzh < 1200)) {
            Dy = 200;
        }

        else if ((1200 <= Dv_Kzh) && (Dv_Kzh < 1400)) {
            Dy = 250;
        }
        else if ((1400 <= Dv_Kzh) && (Dv_Kzh < 1600)) {
            Dy = 250;
        }

        else if ((1600 <= Dv_Kzh) && (Dv_Kzh < 1800)) {
            Dy = 250;
        }

        else if ((1800 <= Dv_Kzh) && (Dv_Kzh < 2000)) {
            Dy = 250;
        }

        else if (2000 <= Dv_Kzh) {
            Dy = 250;

        }
    }

    //первый снизу
    if (id == 3) {
        if ((800 <= Dv_Kzh) && (Dv_Kzh < 1000)) {
            Dy = 100;
        }

        else if ((1000 <= Dv_Kzh) && (Dv_Kzh < 1200)) {
            Dy = 100;
        }

        else if ((1200 <= Dv_Kzh) && (Dv_Kzh < 1400)) {
            Dy = 150;
        }
        else if ((1400 <= Dv_Kzh) && (Dv_Kzh < 1600)) {
            Dy = 150;
        }

        else if ((1600 <= Dv_Kzh) && (Dv_Kzh < 1800)) {
            Dy = 150;
        }

        else if ((1800 <= Dv_Kzh) && (Dv_Kzh < 2000)) {
            Dy = 150;
        }

        else if (2000 <= Dv_Kzh) {
            Dy = 200;

        }
    }

    //третий сверху
    if (id == 9) {
        if ((800 <= Dv_Kzh) && (Dv_Kzh < 1000)) {
            Dy = 250;
        }

        else if ((1000 <= Dv_Kzh) && (Dv_Kzh < 1200)) {
            Dy = 300;
        }

        else if ((1200 <= Dv_Kzh) && (Dv_Kzh < 1400)) {
            Dy = 350;
        }
        else if ((1400 <= Dv_Kzh) && (Dv_Kzh < 1600)) {
            Dy = 400;
        }

        else if ((1600 <= Dv_Kzh) && (Dv_Kzh < 1800)) {
            Dy = 400;
        }

        else if ((1800 <= Dv_Kzh) && (Dv_Kzh < 2000)) {
            Dy = 400;
        }

        else if (2000 <= Dv_Kzh) {
            Dy = 400;

        }
    }

    //камера
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

    if (p == 1.6) {
        switch (Dy) {
        case 50:
            b = 14;
            d = 18;
            D1 = 125;
            num = 4;
            break;
        case 65:
            b = 14;
            d = 18;
            D1 = 145;
            num = 4;
            break;
        case 80:
            b = 14;
            d = 18;
            D1 = 160;
            num = 4;
            break;
        case 100:
            b = 16;
            d = 18;
            D1 = 180;
            num = 8;
            break;
        case 125:
            b = 16;
            d = 18;
            D1 = 210;
            num = 8;
            break;
        case 150:
            b = 18;
            d = 23;
            D1 = 240;
            num = 8;
            break;
        case 200:
            b = 20;
            d = 23;
            D1 = 295;
            num = 8;
            break;
        case 250:
            b = 24;
            d = 23;
            D1 = 355;
            num = 12;
            break;
        case 300:
            b = 28;
            d = 27;
            D1 = 410;
            num = 12;
            break;
        case 350:
            b = 32;
            d = 27;
            D1 = 470;
            num = 12;
            break;
        case 400:
            b = 34;
            d = 30;
            D1 = 525;
            num = 12;
            break;
        case 500:
            b = 40;
            d = 33;
            D1 = 650;
            num = 20;
            break;
        case 600:
            b = 45;
            d = 40;
            D1 = 770;
            num = 20;
            break;
        case 800:
            b = 52;
            d = 40;
            D1 = 950;
            num = 24;
            break;
        }
    }

    else if (p == 2.5) {
        switch (Dy) {
        case 50:
            b = 14;
            d = 18;
            D1 = 125;
            num = 4;
            break;
        case 65:
            b = 16;
            d = 18;
            D1 = 145;
            num = 4;
            break;
        case 80:
            b = 18;
            d = 18;
            D1 = 160;
            num = 4;
            break;
        case 100:
            b = 20;
            d = 23;
            D1 = 190;
            num = 8;
            break;
        case 125:
            b = 22;
            d = 27;
            D1 = 220;
            num = 8;
            break;
        case 150:
            b = 24;
            d = 27;
            D1 = 250;
            num = 8;
            break;
        case 200:
            b = 26;
            d = 27;
            D1 = 310;
            num = 8;
            break;
        case 250:
            b = 30;
            d = 30;
            D1 = 370;
            num = 12;
            break;
        case 300:
            b = 34;
            d = 30;
            D1 = 410;
            num = 12;
            break;
        case 350:
            b = 38;
            d = 33;
            D1 = 490;
            num = 12;
            break;
        case 400:
            b = 40;
            d = 33;
            D1 = 550;
            num = 12;
            break;
        case 500:
            b = 48;
            d = 40;
            D1 = 660;
            num = 20;
            break;
        case 600:
            b = 50;
            d = 40;
            D1 = 770;
            num = 20;
            break;
        case 800:
            b = 63;
            d = 46;
            D1 = 990;
            num = 24;
            break;
        }

    }

    SolidSPtr KrBase = CreateBaseCover(Dy, p);
    SolidSPtr KrWithOtv = CreateMassivOtv_IP(KrBase, b, d / 2, D1, 0., num, axisX, 0, 0);
    SolidSPtr KrRes = CreateCylForCover(KrWithOtv, b, d);
    return  KrRes;
}

//Кольцо плавающей головки
SPtr<MbSolid> ParametricModelCreator::CreateRingOn_IP(int Dv_Kzh, double p, int DKr) {

    int S;
    int D2;
    int d;
    int num;
    if (p == 1.6) {
        if ((500 <= DKr) && (DKr < 600)) {
            S = 40;
            D2 = 396;
            d = 26;
            num = 12;
        }
        else if ((600 <= DKr) && (DKr < 700)) {
            S = 45;
            D2 = 496;
            d = 26;
            num = 12;
        }
        else if ((700 <= DKr) && (DKr < 800)) {
            S = 55;
            D2 = 592;
            d = 26;
            num = 12;
        }
        else if ((800 <= DKr) && (DKr < 900)) {
            S = 60;
            D2 = 690;
            d = 26;
            num = 12;
        }
        else if ((900 <= DKr) && (DKr < 1000)) {
            S = 65;
            D2 = 790;
            d = 26;
            num = 12;
        }
        else if ((1000 <= DKr) && (DKr < 1100)) {
            S = 70;
            D2 = 890;
            d = 26;
            num = 12;
        }
        else if ((1100 <= DKr) && (DKr < 1200)) {
            S = 75;
            D2 = 990;
            d = 26;
            num = 12;
        }
        else if (1200 <= DKr) {
            S = 75;
            D2 = 1090;
            d = 26;
            num = 12;
        }
    }

    else if (p == 2.5) {
        if ((500 <= DKr) && (DKr < 600)) {
            S = 50;
            D2 = 396 + (DKr - 500);
            d = 26;
            num = 12;
        }
        else if ((600 <= DKr) && (DKr < 700)) {
            S = 45;
            D2 = 496;
            d = 26;
            num = 12;
        }
        else if ((700 <= DKr) && (DKr < 800)) {
            S = 55;
            D2 = 592;
            d = 26;
            num = 12;
        }
        else if ((800 <= DKr) && (DKr < 900)) {
            S = 60;
            D2 = 690;
            d = 26;
            num = 12;
        }
        else if ((900 <= DKr) && (DKr < 1000)) {
            S = 65;
            D2 = 790;
            d = 26;
            num = 12;
        }
        else if (1000 <= DKr < 1100) {
            S = 70;
            D2 = 890;
            d = 26;
            num = 12;
        }
        else if ((1100 <= DKr) && (DKr < 1200)) {
            S = 75;
            D2 = 990;
            d = 26;
            num = 12;
        }
        else if (1200 <= DKr) {
            S = 75;
            D2 = 1090;
            d = 26;
            num = 12;
        }
    }
    //// Создание массива точек для контура.
    SArray<MbCartPoint> arrPnts(6);
    arrPnts.Add(MbCartPoint(0, D2 / 2)); //D2
    arrPnts.Add(MbCartPoint(0, D2 / 2 + S));//S
    arrPnts.Add(MbCartPoint(S - 10, D2 / 2 + S));// S-10
    arrPnts.Add(MbCartPoint(S - 10, D2 / 2 - 12));
    arrPnts.Add(MbCartPoint(6, D2 / 2 - 12));
    arrPnts.Add(MbCartPoint(6, D2 / 2));

    MbPolyline* pPolyline = new MbPolyline(arrPnts, true);
    MbContour* pContourPolyline = new MbContour(*pPolyline, true);

    MbSweptData sweptData(pl, *pContourPolyline);

    RevolutionValues revParms(360 * DEG_TO_RAD, 0, 0);

    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);

    MbSolid* pSolidOsnova = nullptr;
    ::RevolutionSolid(sweptData, axisX, revParms, operNames, cNames, pSolidOsnova);

    SolidSPtr pSolidOsnovaRes(pSolidOsnova);

    SolidSPtr MassivOtv = CreateMassivOtv_IP(pSolidOsnovaRes, (S - 10) * 2,
        d / 2., D2 + 40, 0.0, num, axisZ, 90, 0);

    return MassivOtv;
}

//Базовое тело камеры 
static SPtr<MbSolid> CreateBaseForCamera(int DKr, int DKrvne, int Dv_Kzh, double p, double s) {

    int l5;
    int Dvne = Dv_Kzh + s * 2;
    if ((500 <= DKr) && (DKr < 600)) {
        l5 = 460;
    }
    else if ((600 <= DKr) && (DKr < 700)) {
        l5 = 550;
    }
    else if ((700 <= DKr) && (DKr < 800)) {
        l5 = 620;
    }
    else if ((800 <= DKr) && (DKr < 900)) {
        l5 = 700;
    }
    else if ((900 <= DKr) && (DKr < 1000)) {
        l5 = 760;
    }
    else if ((1000 <= DKr) && (DKr < 1100)) {
        l5 = 780;
    }
    else if ((1100 <= DKr) && (DKr < 1200)) {
        l5 = 840;
    }
    else if (1200 <= DKr) {
        l5 = 890;
    }
    SolidSPtr pSptrFlanecOnLoft = CreateTubeOnKzh(13, p, DKr, s, Dv_Kzh);
    pSptrFlanecOnLoft->Rotate(axisZ, 270 * DEG_TO_RAD);
    //  pSptrFlanecOnLoft->Rotate(axisY, 180 * DEG_TO_RAD);
    pSptrFlanecOnLoft->Move(MbVector3D(MbCartPoint3D(0, 0, 0),
        MbCartPoint3D(l5 - DKrvne / 4 - 86, 0, 0))); // длина цилиндрической части

    //// Создание основы тела (половина)
    SArray<MbCartPoint> arrPnts(5);
    arrPnts.Add(MbCartPoint(l5 - DKrvne / 4 - 86, DKrvne / 2));
    arrPnts.Add(MbCartPoint(0, DKrvne / 2));
    arrPnts.Add(MbCartPoint(0, DKr / 2));
    arrPnts.Add(MbCartPoint(l5 - DKrvne / 4 - 86, DKr / 2));

    MbPolyline* pPolyline = new MbPolyline(arrPnts, true);
    MbContour* pContourPolyline = new MbContour(*pPolyline, true);

    MbSweptData sweptData(pl, *pContourPolyline);
    RevolutionValues revParms(360 * DEG_TO_RAD, 0, 0);

    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);

    MbSolid* pSolidOsnova = nullptr;
    ::RevolutionSolid(sweptData, axisX, revParms, operNames, cNames, pSolidOsnova);

    SolidSPtr pSolidOsnovaRes(pSolidOsnova);
    SolidSPtr pSolidOsnovaRes1;
    BooleanResult(pSolidOsnovaRes, cm_Copy, pSptrFlanecOnLoft, cm_Copy,
        MbBooleanOperationParams(bo_Union, true, operNames), pSolidOsnovaRes1);
    return pSolidOsnovaRes1;
}

//Зеркальный массив
static SPtr<MbSolid> CreateMirror(SPtr<MbSolid> SolidForMirror, double degr, MbAxis3D axis) {

    SolidSPtr uSolid;
    SolidSPtr uSolidRes;
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);
    // Локальная СК, относительно плоскости XY которой будет выполняться симметрия
    MbPlacement3D plSym;
    plSym.Rotate(axis, degr * DEG_TO_RAD);

    ::SymmetrySolid(*SolidForMirror, cm_Copy, MbSymmetrySolidParams
    (plSym, operNames, ts_neutral), uSolidRes);
    BooleanResult(uSolidRes, cm_Copy, SolidForMirror, cm_Copy, MbBooleanOperationParams
    (bo_Union, true, operNames), uSolid);
    return uSolid;
}

//Итоговая камера
SPtr<MbSolid> ParametricModelCreator::CreateUnionKamera_IP(double L, int Dv_Kzh, double p, int DKr, double Dn, double Dy, double s) {
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);


    int Dvne = Dv_Kzh + s * 2;

    //Создание половины камеры
    SolidSPtr HalfKamera = CreateBaseForCamera(DKr, DKr + s * 2, Dv_Kzh, p, s);
    //Создание второй половины камеры с помощью отзеркаливания
    SolidSPtr BaseKamera = CreateMirror(HalfKamera, -90, axisY);
    //Создание фланца 7 (верхний)
    SolidSPtr Truba = CreateTubeOnKzh(7, p, DKr, s, Dv_Kzh);
    BaseKamera->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0., -(DKr + s * 2) / 2, 0)));
    SolidSPtr TrubaExt = CreateTubeOnKzhExtr(BaseKamera, Truba,
        Dn, 7, p, DKr, Dv_Kzh);
    TrubaExt->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0., (DKr + s * 2) / 2, 0)));
    BaseKamera->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0., (DKr + s * 2) / 2, 0)));
    SolidSPtr pSptrSolid;
    SolidSPtr pSptrSolid1;
    BooleanResult(TrubaExt, cm_Copy, BaseKamera, cm_Copy,
        MbBooleanOperationParams(bo_Union, true, operNames), pSptrSolid);
    TrubaExt->Rotate(axisX, 180 * DEG_TO_RAD);
    BooleanResult(pSptrSolid, cm_Copy, TrubaExt, cm_Copy,
        MbBooleanOperationParams(bo_Union, true, operNames), pSptrSolid1);


    SolidSPtr HoleInTruba = CreateHole(pSptrSolid1, Dy, DKr, -90., 0., (DKr + s * 2) / 2., 6, Dv_Kzh);
    SolidSPtr HoleInTruba1 = CreateHole(HoleInTruba, Dy, DKr, 90., 0., -(DKr + s * 2) / 2., 7, Dv_Kzh);

    // перегородка 
    SArray<MbCartPoint> arrPnts1s(4);
    arrPnts1s.Add(MbCartPoint((L - 8) / 2, 0));
    arrPnts1s.Add(MbCartPoint((L - 8) / 2, 12));
    arrPnts1s.Add(MbCartPoint((-L + 8) / 2, 12));
    arrPnts1s.Add(MbCartPoint((-L + 8) / 2, 0));

    MbPolyline* pPolyline1s1 = new MbPolyline(arrPnts1s, true);
    MbContour* pContourPolyline1s1 = new MbContour(*pPolyline1s1, true);

    MbSweptData sweptData11(pl, *pContourPolyline1s1);

    const double HEIGHT_FORWARD = (DKr + 1) / 2, HEIGHT_BACKWARD = (DKr + 1) / 2;

    ExtrusionValues extrusionParams(HEIGHT_FORWARD, HEIGHT_BACKWARD);

    MbVector3D dir(0, 0, 1);

    MbSolid* pSolidPeregorodka = nullptr;
    ::ExtrusionSolid(sweptData11, dir, nullptr, nullptr, false,
        extrusionParams, operNames, cNames, pSolidPeregorodka);

    SolidSPtr pSolidPeregorodkaRes(pSolidPeregorodka);
    SolidSPtr pBaseKameraFin;
    BooleanResult(HoleInTruba1, cm_Copy, pSolidPeregorodkaRes, cm_Copy,
        MbBooleanOperationParams(bo_Union, true, operNames), pBaseKameraFin);
    return pBaseKameraFin;
}

//Отверстия в перегородке
static SPtr<MbSolid> CreateOtvVPeregorodka(SPtr<MbSolid> OsnovaPereg, int Dv_Kzh, double p, int DKr) {
    RPArray<MbContour>* ptrContoursR = new RPArray<MbContour>();

    int bigD;

    if (500 <= DKr && DKr < 600) {
        bigD = 366 + (DKr - 500);
    }
    else if ((600 <= DKr) && (DKr < 700)) {
        bigD = 462;
    }
    else if ((700 <= DKr) && (DKr < 800)) {
        bigD = 560;
    }
    else if ((800 <= DKr) && (DKr < 900)) {
        bigD = 652;
    }
    else if ((900 <= DKr) && (DKr < 1000)) {
        bigD = 750;
    }
    else if ((1000 <= DKr) && (DKr < 1100)) {
        bigD = 854;
    }
    else if ((1100 <= DKr) && (DKr < 1200)) {
        bigD = 948;
    }
    else if (1200 <= DKr) {
        bigD = 1052;
    }


    double x;
    double y;
    int t;
    int d = 26;

    if (d / 2 == 13)
        t = 32; //расстояние между окружностями
    else
        t = 26; //расстояние между окружностями
    double shag = sqrt(2) * t;
    int n = floor(bigD / shag); // Кол-во отверстий на 0 ряду - тот, что в середине
    float h = bigD / 2;

    for (int j = 1; j < n; j++)
    {
        for (int i = 0; i < 1000; i++) //!!!!!!!!!!!
        {
            x = shag * n - i * shag - 0.5 * shag * j;

            y = j * shag / 2 + 8;

            double l = sqrt(pow(x, 2) + pow(y, 2));

            if ((l + d / 2) <= h && y < h)
            {
                MbArc* pBound = new MbArc(MbCartPoint(x, y), d / 2);
                MbContour* ptrContour = new MbContour();
                ptrContour->AddSegment(pBound);
                ptrContoursR->Add(ptrContour);
                ptrContour->Clear();
            }
        }
    }

    MbPlane* pPlaneXZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1, 0, 0),
        MbCartPoint3D(0, 0, 1));

    MbSweptData sweptData1(*pPlaneXZ, *ptrContoursR);

    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);
    const double HEIGHT_FORWARD = 100, HEIGHT_BACKWARD = 100;
    ExtrusionValues extrusionParams(HEIGHT_FORWARD, HEIGHT_BACKWARD);
    MbVector3D dir(0, 1, 0);
    MbSolid* m_pResSolid = nullptr;
    ::ExtrusionResult(*OsnovaPereg, cm_Same, sweptData1, dir, extrusionParams, bo_Difference, operNames, NULL, m_pResSolid);
    SolidSPtr pSolidPeregorodkaRes(m_pResSolid);
    return pSolidPeregorodkaRes;
}

//Создание перегородки для труб
SPtr<MbSolid> ParametricModelCreator::CreateOsnovaPeregorodka_IP(int Dv_Kzh, double p, int DKr /*double S1, double D3*/) {

    int S1;
    int D3;

    if ((500 <= DKr) && (DKr < 600)) {
        S1 = 10;
        D3 = 497 + (DKr - 500);
    }
    else if ((600 <= DKr) && (DKr < 700)) {
        S1 = 10;
        D3 = 597;
    }
    else if ((700 <= DKr) && (DKr < 800)) {
        S1 = 12;
        D3 = 696;
    }
    else if ((800 <= DKr) && (DKr < 900)) {
        S1 = 12;
        D3 = 796;
    }
    else if ((900 <= DKr) && (DKr < 1000)) {
        S1 = 12;
        D3 = 895;
    }
    else  if ((1000 <= DKr) && (DKr < 1100)) {
        S1 = 12;
        D3 = 995;
    }
    else if ((1100 <= DKr) && (DKr < 1200)) {
        S1 = 12;
        D3 = 1095;
    }
    else if (1200 <= DKr) {
        S1 = 12;
        D3 = 1195;
    }



    //// Создание массива точек для контура.
    SArray<MbCartPoint> arrPnts1(4);
    arrPnts1.Add(MbCartPoint(0, 0));
    arrPnts1.Add(MbCartPoint(0, S1));
    arrPnts1.Add(MbCartPoint(D3 / 2, S1));
    arrPnts1.Add(MbCartPoint(D3 / 2, 0));

    MbPolyline* pPolyline1 = new MbPolyline(arrPnts1, true);
    MbContour* pContourPolyline1 = new MbContour(*pPolyline1, true);

    MbSweptData sweptData1(pl, *pContourPolyline1);

    RevolutionValues revParms1(-180 * DEG_TO_RAD, 0, 0);

    MbSNameMaker operNames1(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames1(0, 1, false);

    MbSolid* pSolidOsnova1 = nullptr;
    ::RevolutionSolid(sweptData1, axisY, revParms1, operNames1, cNames1, pSolidOsnova1);

    SolidSPtr pSolidOsnovaRes1(pSolidOsnova1);


    SolidSPtr MassivOtv = CreateOtvVPeregorodka(pSolidOsnovaRes1, Dv_Kzh, p, DKr);
    SolidSPtr MassivOtv2 = CreateMirror(MassivOtv, 180, axisY);

    return MassivOtv2;
}

//Создание решеток
//Итоговая неподвижная решетка
SPtr<MbSolid> ParametricModelCreator::CreateOsnovaNePodResh_IP(int Dv_Kzh, double p, int DKr /*double D1, double S, double D4*/) {

    int S;
    int D1;
    int D4;

    if (p == 1.6) {
        if ((500 <= DKr) && (DKr < 600)) {
            S = 40;
            D1 = 563 + (DKr - 500);
            D4 = 496 + (DKr - 500);
        }
        else if ((600 <= DKr) && (DKr < 700)) {
            S = 45;
            D1 = 663;
            D4 = 596;
        }
        else if ((700 <= DKr) && (DKr < 800)) {
            S = 55;
            D1 = 773;
            D4 = 695;
        }
        else if ((800 <= DKr) && (DKr < 900)) {
            S = 60;
            D1 = 875;
            D4 = 795;
        }
        else if ((900 <= DKr) && (DKr < 1000)) {
            S = 65;
            D1 = 977;
            D4 = 895;
        }
        else if ((1000 <= DKr) && (DKr < 1100)) {
            S = 70;
            D1 = 1078;
            D4 = 995;

        }
        else if ((1100 <= DKr) && (DKr < 1200)) {
            S = 75;
            D1 = 1188;
            D4 = 1095;
        }
        else if (1200 <= DKr) {
            S = 75;
            D1 = 1266;
            D4 = 1060;
        }
    }

    else if (p == 2.5) {
        if ((500 <= DKr) && (DKr < 600)) {
            S = 50;
            D1 = 557;
            D4 = 496;
        }
        else if ((600 <= DKr) && (DKr < 700)) {
            S = 45;
            D1 = 663;
            D4 = 596;
        }
        else if ((700 <= DKr) && (DKr < 800)) {
            S = 55;
            D1 = 773;
            D4 = 695;
        }
        else if ((800 <= DKr) && (DKr < 900)) {
            S = 60;
            D1 = 875;
            D4 = 795;
        }
        else if ((900 <= DKr) && (DKr < 1000)) {
            S = 65;
            D1 = 977;
            D4 = 895;
        }
        else if ((1000 <= DKr) && (DKr < 1100)) {
            S = 70;
            D1 = 1078;
            D4 = 995;
        }
        else if ((1100 <= DKr) && (DKr < 1200)) {
            S = 75;
            D1 = 1188;
            D4 = 1095;
        }
        else if (1200 <= DKr) {
            S = 75;
            D1 = 1266;
            D4 = 1060;
        }
    }

    double LV = (S - 12) / 2 - 0.4;
    //// Создание массива точек для контура.
    SArray<MbCartPoint> arrPnts(12);
    arrPnts.Add(MbCartPoint(0, 0));
    arrPnts.Add(MbCartPoint(0, S));
    arrPnts.Add(MbCartPoint(D4 / 2, S));
    arrPnts.Add(MbCartPoint(D4 / 2, S - 6));
    arrPnts.Add(MbCartPoint(D1 / 2, S - 6));
    arrPnts.Add(MbCartPoint(D1 / 2, S - LV - 6));
    arrPnts.Add(MbCartPoint(D1 / 2 - 0.71, S - LV - 6));
    arrPnts.Add(MbCartPoint(D1 / 2 - 0.71, S - LV - 6 - 0.8));
    arrPnts.Add(MbCartPoint(D1 / 2, S - LV - 6 - 0.8));
    arrPnts.Add(MbCartPoint(D1 / 2, 6));
    arrPnts.Add(MbCartPoint(D4 / 2, 6));
    arrPnts.Add(MbCartPoint(D4 / 2, 0));

    MbPlacement3D pl1;

    //// Построение единой ломаной внешнего контура по точкам.
    MbPolyline* pPolyline = new MbPolyline(arrPnts, true);
    MbContour* pContourPolyline = new MbContour(*pPolyline, true);
    MbSweptData sweptData(pl1, *pContourPolyline);

    RevolutionValues revParms(-180 * DEG_TO_RAD, 0, 0);
    // Именователи для операции построения тела вращения и для контуров образующей
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);

    // Вызов функции-утилиты для построения твердого тела вращения
    MbSolid* pSolid = nullptr;
    MbResultType res = ::RevolutionSolid(sweptData, axisY, revParms, operNames, cNames, pSolid);
    SolidSPtr pSolidOsnovaRes(pSolid);

    SolidSPtr MassivOtv = CreateOtvVPeregorodka(pSolidOsnovaRes, Dv_Kzh, p, DKr);
    SolidSPtr MassivOtv1 = CreateMirror(MassivOtv, 180, axisY);

    return MassivOtv1;
}

//Итоговая подвижная решетка
SPtr<MbSolid> ParametricModelCreator::CreateOsnovaReshPod_IP(int Dv_Kzh, double p, int DKr /*double S, double D2, double D5*/) {

    int S;
    int D2;
    int D5;

    if (p == 1.6) {
        if ((500 <= DKr) && (DKr < 600)) {
            S = 40;
            D2 = 396 + (DKr - 500);
            D5 = 372 + (DKr - 500);
        }
        else if ((600 <= DKr) && (DKr < 700)) {
            S = 45;
            D2 = 496;
            D5 = 470;
        }
        else if ((700 <= DKr) && (DKr < 800)) {
            S = 55;
            D2 = 592;
            D5 = 568;
        }
        else if ((800 <= DKr) && (DKr < 900)) {
            S = 60;
            D2 = 690;
            D5 = 660;
        }
        else if ((900 <= DKr) && (DKr < 1000)) {
            S = 65;
            D2 = 790;
            D5 = 760;
        }
        else if ((1000 <= DKr) && (DKr < 1100)) {
            S = 70;
            D2 = 890;
            D5 = 862;

        }
        else if ((1100 <= DKr) && (DKr < 1200)) {
            S = 75;
            D2 = 990;
            D5 = 962;
        }
        else if (1200 <= DKr) {
            S = 75;
            D2 = 1090;
            D5 = 1060;
        }
    }

    else if (p == 2.5) {
        if ((500 <= DKr) && (DKr < 600)) {
            S = 50;
            D2 = 396;
            D5 = 372;
        }
        else if ((600 <= DKr) && (DKr < 700)) {
            S = 45;
            D2 = 496;
            D5 = 470;
        }
        else if ((700 <= DKr) && (DKr < 800)) {
            S = 55;
            D2 = 592;
            D5 = 568;
        }
        else if ((800 <= DKr) && (DKr < 900)) {
            S = 60;
            D2 = 690;
            D5 = 660;
        }
        else if ((900 <= DKr) && (DKr < 1000)) {
            S = 65;
            D2 = 790;
            D5 = 760;
        }
        else if ((1000 <= DKr) && (DKr < 1100)) {
            S = 70;
            D2 = 890;
            D5 = 862;
        }
        else if ((1100 <= DKr) && (DKr < 1200)) {
            S = 75;
            D2 = 990;
            D5 = 962;
        }
        else if (1200 <= DKr) {
            S = 75;
            D2 = 1090;
            D5 = 1060;
        }
    }
    //// Создание массива точек для контура.
    SArray<MbCartPoint> arrPnts(8);
    arrPnts.Add(MbCartPoint(0, 0));
    arrPnts.Add(MbCartPoint(0, S));
    arrPnts.Add(MbCartPoint(D5 / 2, S));
    arrPnts.Add(MbCartPoint(D5 / 2, S - 6));
    arrPnts.Add(MbCartPoint(D2 / 2, S - 6));
    arrPnts.Add(MbCartPoint(D2 / 2, 6));
    arrPnts.Add(MbCartPoint(D5 / 2, 6));
    arrPnts.Add(MbCartPoint(D5 / 2, 0));

    MbPolyline* pPolyline = new MbPolyline(arrPnts, true);
    MbContour* pContourPolyline = new MbContour(*pPolyline, true);

    MbSweptData sweptData(pl, *pContourPolyline);

    RevolutionValues revParms(-180 * DEG_TO_RAD, 0, 0);

    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);

    MbSolid* pSolid = nullptr;
    MbResultType res = ::RevolutionSolid(sweptData, axisY, revParms, operNames, cNames, pSolid);
    SolidSPtr pSolidRes(pSolid);

    SolidSPtr MassivOtv = CreateOtvVPeregorodka(pSolidRes, Dv_Kzh, p, DKr);
    SolidSPtr MassivOtv1 = CreateMirror(MassivOtv, 180, axisY);

    return MassivOtv1;

}

//Эллиптическая крышка на камере
SPtr<MbSolid> ParametricModelCreator::CreateKrKameraEll_IP(ConfigParams_IP params) {

    MbPlane* pPlaneXY = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1, 0, 0), MbCartPoint3D(0, 1, 0));
    MbPlane* pPlaneXZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1, 0, 0), MbCartPoint3D(0, 0, 1));
    MbPlane* pPlaneYZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0), MbCartPoint3D(0, 0, 1));

    const int DvneKam = params.DKr + params.s * 2;
    int Dvne = params.Dv_Kzh + params.s * 2;
    double l;

    if ((500 <= params.DKr) && (params.DKr < 600)) {
        l = 150;
    }
    else if ((600 <= params.DKr) && (params.DKr < 700)) {
        l = 180;
    }
    else if ((700 <= params.DKr) && (params.DKr < 800)) {
        l = 200;
    }
    else if ((800 <= params.DKr) && (params.DKr < 900)) {
        l = 220;
    }
    else if ((900 <= params.DKr) && (params.DKr < 1000)) {
        l = 240;
    }
    else if ((1000 <= params.DKr) && (params.DKr < 1100)) {
        l = 260;
    }
    else if ((1100 <= params.DKr) && (params.DKr < 1200)) {
        l = 280;
    }
    else if (1200 <= params.DKr) {
        l = 300;
    }

    SolidSPtr pSptrFlanecOnLoft = CreateTubeOnKzh(13, params.p, params.DKr, params.s, params.Dv_Kzh);
    pSptrFlanecOnLoft->Rotate(axisZ, 270 * DEG_TO_RAD);
    pSptrFlanecOnLoft->Rotate(axisY, 180 * DEG_TO_RAD);

    // построение основы без эллипса 
    SArray<MbCartPoint> arrPnts(12);
    arrPnts.Add(MbCartPoint(0, params.DKr / 2));
    arrPnts.Add(MbCartPoint(l, params.DKr / 2));
    arrPnts.Add(MbCartPoint(l, (params.DKr + params.s * 2) / 2));
    arrPnts.Add(MbCartPoint(0, (params.DKr + params.s * 2) / 2));

    MbPolyline* pPolyline = new MbPolyline(arrPnts, true);
    MbContour* pContourPolyline = new MbContour(*pPolyline, true);

    MbSweptData sweptData(pl, *pContourPolyline);

    RevolutionValues revParms(360 * DEG_TO_RAD, 0, 0);

    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);

    MbSolid* pSolid = nullptr;
    ::RevolutionSolid(sweptData, axisX, revParms, operNames, cNames, pSolid);

    SolidSPtr pSolid1(pSolid);
    SolidSPtr uSolid21;
    BooleanResult(pSolid1, cm_Copy, pSptrFlanecOnLoft, cm_Copy, MbBooleanOperationParams(bo_Union, true, operNames), uSolid21);

    SolidSPtr uSolid29;

    //Создание эллиптического днища
    SolidSPtr ElDno = CreateEllipticBottom(params.DKr, params.s);

    // MbPlacement3D* place = new MbPlacement3D();
    ElDno->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(-l, 0, 0)));
    ElDno->Rotate(axisY, 180 * DEG_TO_RAD);

    BooleanResult(uSolid21, cm_Copy, ElDno, cm_Copy,
        MbBooleanOperationParams(bo_Union, true, operNames), uSolid29);

    // создание перегородки в крышке 
    SArray<MbCartPoint> arrPntsPereg(4);
    arrPntsPereg.Add(MbCartPoint(-86, 0));
    arrPntsPereg.Add(MbCartPoint(l, 0));
    arrPntsPereg.Add(MbCartPoint(l, 12));
    arrPntsPereg.Add(MbCartPoint(-86, 12));

    //// Построение единой ломаной внешнего контура по точкам.
    MbPolyline* pPolylinePereg = new MbPolyline(arrPntsPereg, true);
    MbContour* pContourPolylinePereg = new MbContour(*pPolylinePereg, true);

    //// Локальная СК (по умолчанию совпадает с мировой СК).
    MbPlacement3D plPereg;

    //// Данные об образующей операции движения.
    MbSweptData sweptData11(plPereg, *pContourPolylinePereg);

    //// Именователи для операции построения тела вращения и для контуров образующей.
    MbSNameMaker operNames3(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames3(0, 1, false);

    //// Параметры операции выдавливания, задающие свойства тела для построения в прямом и в обратном направлении вдоль (глубина выдавливания).
    const double HEIGHT_FORWARD = (params.DKr + 1) / 2, HEIGHT_BACKWARD = (params.DKr + 1) / 2;

    ExtrusionValues extrusionParams(HEIGHT_FORWARD, HEIGHT_BACKWARD);

    //// Направление для построения тела:
    MbVector3D dir(0, 0, 1);

    //// Твердое тело для модели.
    MbSolid* pSolid10 = nullptr;
    MbResultType res10 = ::ExtrusionSolid(sweptData11, dir, nullptr, nullptr, false, extrusionParams, operNames3, cNames3, pSolid10);

    SolidSPtr pDuplSolid11(pSolid10);

    // вторая часть перегородки (в эллипсе)
    MbPlane* pPlaneYZ1 = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 1, 0), MbCartPoint3D(0, 0, 1));

    MbPlacement3D pl3;
    pPlaneYZ1->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(l, 0, 0)));

    MbCartPoint p1P(0, (params.DKr) / 2);
    MbCartPoint p2P(12, (params.DKr) / 2);
    MbCartPoint p3P(12, -(params.DKr) / 2);
    MbCartPoint p4P(0, -(params.DKr) / 2);

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

    MbSweptData sweptDataP(*pPlaneYZ, *ptrContourPs);

    ExtrusionValues extrusionParam1;
    extrusionParam1.side1.way = sw_shell;
    //// Направление для построения тела:
    MbVector3D dir1(1, 0, 0);

    // Вызов функции-утилиты для построения тела выдавливания
    MbSolid* pSolidProd = nullptr;
    MbResultType resProd = ::ExtrusionSolid(sweptDataP, dir1, ElDno, nullptr, true, extrusionParam1, operNames3, cNames3, pSolidProd);
    SolidSPtr pSolidPeregorodka(pSolidProd);


    SolidSPtr pSolidKrEllRes;
    SolidSPtr uSolidPereg;
    BooleanResult(pDuplSolid11, cm_Copy, pSolidPeregorodka, cm_Copy, MbBooleanOperationParams(bo_Union, true, operNames), uSolidPereg);
    BooleanResult(uSolidPereg, cm_Copy, uSolid29, cm_Copy, MbBooleanOperationParams(bo_Union, true, operNames), pSolidKrEllRes);

    return pSolidKrEllRes;

}

//Стойка
SPtr<MbSolid> ParametricModelCreator::CreateStoyka_IP(int Dv_Kzh, double L_Base, double s)
{
    int Dvne = Dv_Kzh + s * 2;
    int b; //выдавливание нижнего прямоугольника 
    int h; //высота, на которую опускаем стойку 
    int Width;
    if (Dv_Kzh <= 800) {
        b = 18;
        h = 180;
        Width = L_Base * 0.02;
    }
    else if ((800 < Dv_Kzh) && (Dv_Kzh <= 1000)) {
        b = 22;
        h = 190;
        Width = L_Base * 0.02;
    }
    else if ((1000 < Dv_Kzh) && (Dv_Kzh <= 1200)) {
        b = 25;
        h = 200;
        Width = L_Base * 0.025;
    }
    else if ((1200 < Dv_Kzh) && (Dv_Kzh <= 1400)) {
        b = 28;
        h = 210;
        Width = L_Base * 0.025;
    }
    else if ((1400 < Dv_Kzh) && (Dv_Kzh <= 1600)) {
        b = 41;
        h = 220;
        Width = L_Base * 0.03;
    }
    else if ((1600 < Dv_Kzh) && (Dv_Kzh <= 1800)) {
        b = 53;
        h = 250;
        Width = L_Base * 0.03;
    }
    else if ((1800 < Dv_Kzh) && (Dv_Kzh <= 2000)) {
        b = 60;
        h = 270;
        Width = L_Base * 0.03;
    }
    else if (2000 < Dv_Kzh) {
        b = 65;
        h = 280;
        Width = L_Base * 0.03;
    }
    // построение прямоугольника-основы

    MbPlacement3D pl;
    MbPlane* pPlaneXZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1, 0, 0), MbCartPoint3D(0, 0, 1));

    MbCartPoint p1P(-Width, Dvne / 2 - 50);
    MbCartPoint p2P(Width, Dvne / 2 - 50);
    MbCartPoint p3P(Width, -Dvne / 2 - 50);
    MbCartPoint p4P(-Width, -Dvne / 2 - 50);

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

    //// Данные об образующей операции движения.
    MbSweptData sweptDataOsnova(*pPlaneXZ, *ptrContourPs);

    //// Именователи для операции построения тела вращения и для контуров образующей.
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);

    //// Параметры операции выдавливания, задающие свойства тела для построения в прямом и в обратном направлении вдоль (глубина выдавливания).
    const double HEIGHT_FORWARD = b, HEIGHT_BACKWARD = 0;

    ExtrusionValues extrusionParams(HEIGHT_FORWARD, HEIGHT_BACKWARD);

    //// Направление для построения тела:
    MbVector3D dir(0, 1, 0);

    //// Твердое тело для модели.

    MbSolid* pSolid1 = nullptr;
    MbResultType res = ::ExtrusionSolid(sweptDataOsnova, dir, nullptr, nullptr, true, extrusionParams, operNames, cNames, pSolid1);

    SolidSPtr pSolidOsnova(pSolid1);

    MbSurface* pCylSurf = nullptr;
    SArray<MbCartPoint3D> arrPnts21(3);
    arrPnts21.Add(MbCartPoint3D(-300, Dvne / 2 + h, 0));
    arrPnts21.Add(MbCartPoint3D(300, Dvne / 2 + h, 0));
    arrPnts21.Add(MbCartPoint3D(0, Dvne / 2 + h + Dvne / 2, 0));
    ::ElementarySurface(arrPnts21[0], arrPnts21[1], arrPnts21[2], st_CylinderSurface, pCylSurf);

    // построение крестообразного эскиза

    pPlaneXZ->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 18, 0)));

    MbCartPoint p1(-Width + b, Dvne / 2 - 85);
    MbCartPoint p2(Width - b, Dvne / 2 - 85);
    MbCartPoint p3(Width - b, Dvne / 2 - 105);
    MbCartPoint p4(b, Dvne / 2 - 105);
    MbCartPoint p5(b, b);
    MbCartPoint p6(Width - b, b);
    MbCartPoint p7(Width - b, -b);
    MbCartPoint p8(b, -b);
    MbCartPoint p9(b, -Dvne / 2 + 79);
    MbCartPoint p10(Width - b, -Dvne / 2 + 79);
    MbCartPoint p11(Width - b, -Dvne / 2 + 65);
    MbCartPoint p12(-Width + b, -Dvne / 2 + 65);
    MbCartPoint p13(-Width + b, -Dvne / 2 + 79);
    MbCartPoint p14(-b, -Dvne / 2 + 79);
    MbCartPoint p15(-b, -b);
    MbCartPoint p16(-Width + b, -b);
    MbCartPoint p17(-Width + b, b);
    MbCartPoint p18(-b, b);
    MbCartPoint p19(-b, Dvne / 2 - 105);
    MbCartPoint p20(-Width + b, Dvne / 2 - 105);


    //Динамическое создание объектов отрезков
    MbLineSegment* Seg1 = new MbLineSegment(p1, p2);
    MbLineSegment* Seg2 = new MbLineSegment(p2, p3);
    MbLineSegment* Seg3 = new MbLineSegment(p3, p4);
    MbLineSegment* Seg4 = new MbLineSegment(p4, p5);
    MbLineSegment* Seg5 = new MbLineSegment(p5, p6);
    MbLineSegment* Seg6 = new MbLineSegment(p6, p7);
    MbLineSegment* Seg7 = new MbLineSegment(p7, p8);
    MbLineSegment* Seg8 = new MbLineSegment(p8, p9);
    MbLineSegment* Seg9 = new MbLineSegment(p9, p10);
    MbLineSegment* Seg10 = new MbLineSegment(p10, p11);
    MbLineSegment* Seg11 = new MbLineSegment(p11, p12);
    MbLineSegment* Seg12 = new MbLineSegment(p12, p13);
    MbLineSegment* Seg13 = new MbLineSegment(p13, p14);
    MbLineSegment* Seg14 = new MbLineSegment(p14, p15);
    MbLineSegment* Seg15 = new MbLineSegment(p15, p16);
    MbLineSegment* Seg16 = new MbLineSegment(p16, p17);
    MbLineSegment* Seg17 = new MbLineSegment(p17, p18);
    MbLineSegment* Seg18 = new MbLineSegment(p18, p19);
    MbLineSegment* Seg19 = new MbLineSegment(p19, p20);
    MbLineSegment* Seg20 = new MbLineSegment(p20, p1);

    //Динамическое создание контура
    MbContour* ptrContour = new MbContour();

    ptrContour->AddSegment(Seg1);
    ptrContour->AddSegment(Seg2);
    ptrContour->AddSegment(Seg3);
    ptrContour->AddSegment(Seg4);
    ptrContour->AddSegment(Seg5);
    ptrContour->AddSegment(Seg6);
    ptrContour->AddSegment(Seg7);
    ptrContour->AddSegment(Seg8);
    ptrContour->AddSegment(Seg9);
    ptrContour->AddSegment(Seg10);
    ptrContour->AddSegment(Seg11);
    ptrContour->AddSegment(Seg12);
    ptrContour->AddSegment(Seg13);
    ptrContour->AddSegment(Seg14);
    ptrContour->AddSegment(Seg15);
    ptrContour->AddSegment(Seg16);
    ptrContour->AddSegment(Seg17);
    ptrContour->AddSegment(Seg18);
    ptrContour->AddSegment(Seg19);
    ptrContour->AddSegment(Seg20);

    //Создание образующей для тела вращения
    RPArray<MbContour>* ptrContours = new RPArray<MbContour>();
    ptrContours->Add(ptrContour);

    ExtrusionValues extrusionParamSUR;
    // Задание поверхности, до которой будет выполняться выдавливание в прямом направлении
    extrusionParamSUR.SetSurface1(pCylSurf);
    // Указание способа выдавливания в прямом направлении - до пересечения с поверхностью
    extrusionParamSUR.side1.way = sw_surface;
    //// Данные об образующей операции движения.

    MbSweptData sweptDataSUR(*pPlaneXZ, *ptrContours);

    //// Твердое тело для модел
    MbSolid* pSolid3 = nullptr;
    MbResultType res2 = ::ExtrusionSolid(sweptDataSUR, dir, nullptr, nullptr, true, extrusionParamSUR, operNames, cNames, pSolid3);

    SolidSPtr pSolidStenki(pSolid3);
    SolidSPtr Osnova;

    BooleanResult(pSolidOsnova, cm_Copy, pSolidStenki, cm_Copy, MbBooleanOperationParams(bo_Union, true, operNames), Osnova);

    return Osnova;
}

//Крышка плавающей головки
SPtr<MbSolid> ParametricModelCreator::CreateKrPlGol_IP(double p, int Dv_Kzh, int DKr)
{

    int S;
    int D2;
    if (p == 1.6) {
        if ((500 <= DKr) && (DKr < 600)) {
            D2 = 396 + (DKr - 500);
            S = 40;
        }
        else if ((600 <= DKr) && (DKr < 700)) {
            D2 = 496;
            S = 45;
        }
        else if ((700 <= DKr) && (DKr < 800)) {
            D2 = 592;
            S = 55;
        }
        else if ((800 <= DKr) && (DKr < 900)) {
            D2 = 690;
            S = 60;
        }
        else if ((900 <= DKr) && (DKr < 1000)) {
            D2 = 790;
            S = 65;
        }
        else if ((1000 <= DKr) && (DKr < 1100)) {
            D2 = 890;
            S = 70;
        }
        else if ((1100 <= DKr) && (DKr < 1200)) {
            D2 = 990;
            S = 75;
        }
        else if (1200 <= DKr) {
            D2 = 1090;
            S = 75;
        }
    }

    else if (p == 2.5) {
        if ((500 <= DKr) && (DKr < 600)) {
            D2 = 396;
            S = 50;
        }
        else if ((600 <= DKr) && (DKr < 700)) {
            D2 = 496;
            S = 45;
        }
        else if ((700 <= DKr) && (DKr < 800)) {
            D2 = 592;
            S = 55;
        }
        else if ((800 <= DKr) && (DKr < 900)) {
            D2 = 690;
            S = 60;
        }
        else if ((900 <= DKr) && (DKr < 1000)) {
            D2 = 790;
            S = 65;
        }
        else if ((1000 <= DKr) && (DKr < 1100)) {
            D2 = 890;
            S = 70;
        }
        else if ((1100 <= DKr) && (DKr < 1200)) {
            D2 = 990;
            S = 75;
        }
        else if (1200 <= DKr) {
            D2 = 1090;
            S = 75;
        }
    }

    SolidSPtr KrBase = CreateRingOn_IP(Dv_Kzh, p, DKr);
    KrBase->Rotate(axisY, 180 * DEG_TO_RAD);
    //Создание эллиптического днища
    SolidSPtr ElDno = CreateEllipticBottom(D2 - 7 * 2, 7);

    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);

    SolidSPtr uSolid29;

    // MbPlacement3D* place = new MbPlacement3D();
    ElDno->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(-(S - 10), 0, 0)));

    BooleanResult(KrBase, cm_Copy, ElDno, cm_Copy,
        MbBooleanOperationParams(bo_Union, true, operNames), uSolid29);
    return  uSolid29;

}



/*

MbModel* ParametricModelCreator::CreatePneymocylinderModel
(testConfigParams params) {
    MbModel* evaporator_floating_head = new MbModel();
    params.EllhTickness = EllTicknessCalculation(params);
    params.CylhTickness = CylTicknessCalculation(params);
    params.Hydraulic = HydraulicCalculation(params);
    params.Thermal = ThermalCalculation(params);
    auto pneumocylinderAssembly = CreatePneumocylinderAssembly
    (params);
    evaporator_floating_head->AddItem(*pneumocylinderAssembly);
    return evaporator_floating_head;
}

double  EllTicknessCalculation(testConfigParams params) {
    double Dy = params.Dy;
    double C = params.C;
    double G = params.G;
    double Pn = params.Pn;
    double F = params.f;
    double z = params.z;
    double h = params.h;


    z = 1 - params.d0 / Dy;
    double S = ceil(Pn * Dy / (4 * z * G - Pn) * Dy / 2 * h + C);

    return S;
}

double  CylTicknessCalculation(testConfigParams params) {
    double Dy = params.Dy;
    double C = params.C;
    double G = params.G;
    double Pn = params.Pn;
    double F = params.f;

    double S = ceil(Pn * Dy / (2 * F * G - Pn) + C);

    return S;
}

double HydraulicCalculation(testConfigParams params) {

    double Re = ThermalCalculation_Re(params);
    double w1 = params.w1;
    double w2 = params.w2;
    double po1 = params.po1;
    double po2 = params.po2;
    double L = params.L_Base;
    double d = params.D, po = params.po, h = params.h;

    double Re = ThermalCalculation_Re(params);
    double lambda = 1 / pow((1, 8 * log1p(Re) - 1.5), 2);

    double Pt = lambda * L * pow(w1, 2) * po1 / 2 * d;

    double Pm = 5.5 * pow(w1, 2) * po1 / 2;

    double Py = po2 * w2 - po1 * w1;

    double Pg = (po1 - po) * h;

    double P = Pt + Pm + Py + Pg;

    return P;
}


double ThermalCalculation(testConfigParams params) {

    double i1 = params.i1;
    double ik = params.ik;
    double G2 = params.G2;
    double c2 = params.c2;
    double r = params.r;
    double ts = params.ts;
    double t2 = params.t2;


    double Q1 = G2 * c2 * (ts - t2); //

    double Q2 = G2 * r;

    double Qr = Q1 + Q2;

    double D = (G2 * c2 * (ts - t2) + G2 * r) / (i1 - ik) * 0.98;

    double Qpr = D * (i1 - ik);

    return Qpr;
}


double ThermalCalculation_HeatCoefficient
(testConfigParams params) {

    double d = params.D;
    double dvn = params.Dn;
    double lambda = params.lambda;
    double R = params.r, Nu_y = params.Nu_y,
        Nu_g = params.Nu_g;

    double dsr = (dvn + d) / 2;

    double a_n = Nu_y * lambda / dvn;

    double a_g = Nu_g * lambda / dvn;

    double K = 1 / (dsr * (1 / a_n * dvn + 1 / 2 * lambda *
        log(d / dvn) + 1 / a_g * d) + R);
    return K;
}


double ThermalCalculation_Re
(testConfigParams params) {

    double Dy = params.Dy, d = params.D;
    double n = params.n, po1 = params.po1;
    double Gr = params.Gr, w1 = params.w1;

    double Fd = M_PI * pow(Dy, 2) / 4;
    double fm = (M_PI * pow(d, 2) * n) / 4;
    double f1 = Fd - fm;

    double w_g = Gr / f1 * po1;
    double Re = w_g * d / w1;

    return Re;
}*/