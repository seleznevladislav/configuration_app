#include "../BuildMathModel.h"

using namespace BuildMathModel;

const double DEG_TO_RAD = M_PI / 180.0;

static SPtr<MbSolid> Create_U_Pipe(double p, double D_Kzh, double D_Kr) {
    MbPlacement3D pl;
    const int d = 20; //внутренний диаметр трубы
    const int d_v = d + 2; // внешний диаметр трубы
    //const double d = RAD_INT, dvne = RAD_EXT; //для основы тела
    const double height = 6000; //высота

    RPArray<MbContour>* ptrContours = new RPArray<MbContour>();

    double bigD;//диаметр ограничительной окружности
    if ((500 <= D_Kr) && (D_Kr < 600)) {
        bigD = 490;
    }
    else if ((600 <= D_Kr) && (D_Kr < 700)) {
        bigD = 590;
    }
    else if ((700 <= D_Kr) && (D_Kr < 800)) {
        bigD = 688;
    }
    else if ((800 <= D_Kr) && (D_Kr < 900)) {
        bigD = 788;
    }
    else if ((900 <= D_Kr) && (D_Kr < 1000)) {
        bigD = 888; //755
    }
    else if ((1000 <= D_Kr) && (D_Kr < 1100)) {
        bigD = 988;
    }
    else if ((1100 <= D_Kr) && (D_Kr < 1200)) {
        bigD = 1088;
    }
    else if (1200 <= D_Kr) {
        bigD = 1188;
    }


    //bigD = bigD - 2;
    double x;
    double y;
    int t = 26;//было 32
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
                MbArc* Circle = new MbArc(MbCartPoint(x, y), d_v / 2);
                ptrContours->push_back(new MbContour(*Circle, true));


                MbArc* Circle1 = new MbArc(MbCartPoint(x, y), d / 2);
                ptrContours->push_back(new MbContour(*Circle1, true));

            }
        }

    }
    MbPlane* pPlaneXY = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1, 0, 0),
        MbCartPoint3D(0, 1, 0));
    MbSweptData sweptData(*pPlaneXY, *ptrContours);

    RevolutionValues revParms(180 * DEG_TO_RAD, 0, 0);
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);


    MbAxis3D axis(pl.GetAxisX());
    axis.Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(-366 / 2., 0, 0)));

    //U-образная часть трубы
    MbSolid* pSolid = nullptr;
    MbResultType res = ::RevolutionSolid(sweptData, axis, revParms, operNames, cNames, pSolid);

    SolidSPtr MainSolid(pSolid);


    return MainSolid;
}

SPtr<MbSolid> ParametricModelCreator::CreatePipe(double p, double D_Kzh, double D_Kr) {
    MbPlacement3D pl;
    const int d = 20; //внутренний диаметр трубы
    const int d_v = d + 2; // внешний диаметр трубы
    //const double d = RAD_INT, dvne = RAD_EXT; //для основы тела
    const double height = 6000; //высота

    RPArray<MbContour>* ptrContours = new RPArray<MbContour>();
    // double bigD = 366;


    double bigD;//диаметр ограничительной окружности
    if ((500 <= D_Kr) && (D_Kr < 600)) {
        bigD = 490;
    }
    else if ((600 <= D_Kr) && (D_Kr < 700)) {
        bigD = 590;
    }
    else if ((700 <= D_Kr) && (D_Kr < 800)) {
        bigD = 688;
    }
    else if ((800 <= D_Kr) && (D_Kr < 900)) {
        bigD = 788;
    }
    else if ((900 <= D_Kr) && (D_Kr < 1000)) {
        bigD = 888; //755
    }
    else if ((1000 <= D_Kr) && (D_Kr < 1100)) {
        bigD = 988;
    }
    else if ((1100 <= D_Kr) && (D_Kr < 1200)) {
        bigD = 1088;
    }
    else if (1200 <= D_Kr) {
        bigD = 1188;
    }

    double x;
    double y;
    int t = 26;
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
                MbArc* Circle = new MbArc(MbCartPoint(x, y), d_v / 2);
                ptrContours->push_back(new MbContour(*Circle, true));


                MbArc* Circle1 = new MbArc(MbCartPoint(x, y), d / 2);
                ptrContours->push_back(new MbContour(*Circle1, true));



                MbArc* Circle2 = new MbArc(MbCartPoint(x, -y), d_v / 2);
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

    MbPlane* pPlaneXY = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1, 0, 0), MbCartPoint3D(0, 1, 0));
    MbSweptData sweptData(*pPlaneXY, *ptrContours);

    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);

    MbVector3D dir(0, 0, -1);
    ExtrusionValues extrusionParam(height, 0);

    MbSolid* pSolid = nullptr;
    ::ExtrusionSolid(sweptData, dir, nullptr, nullptr, false, extrusionParam, operNames, cNames, pSolid);

    MbAxis3D axisY(pl.GetAxisY());


    //U-образная часть трубы
    SolidSPtr pU = Create_U_Pipe(p, D_Kzh, D_Kr);

    SolidSPtr MainSolid(pSolid);
    SolidSPtr pUnion;
    BooleanResult(pU, cm_Copy, MainSolid, cm_Copy, MbBooleanOperationParams(bo_Union, true, operNames), pUnion);

    pUnion->Rotate(axisY, -90 * DEG_TO_RAD, nullptr);


    return pUnion;
}

//Отверстия в перегородке
static SPtr<MbSolid> CreateOtvVPeregorodka(SPtr<MbSolid> OsnovaPereg, double D_Kzh, double p, double D_Kr) {
    RPArray<MbContour>* ptrContoursR = new RPArray<MbContour>();

    double bigD;//диаметр ограничительной окружности
    if ((500 <= D_Kr) && (D_Kr < 600)) {
        bigD = 490;
    }
    else if ((600 <= D_Kr) && (D_Kr < 700)) {
        bigD = 590;
    }
    else if ((700 <= D_Kr) && (D_Kr < 800)) {
        bigD = 688;
    }
    else if ((800 <= D_Kr) && (D_Kr < 900)) {
        bigD = 788;
    }
    else if ((900 <= D_Kr) && (D_Kr < 1000)) {
        bigD = 888; //755
    }
    else if ((1000 <= D_Kr) && (D_Kr < 1100)) {
        bigD = 988;
    }
    else if ((1100 <= D_Kr) && (D_Kr < 1200)) {
        bigD = 1088;
    }
    else if (1200 <= D_Kr) {
        bigD = 1188;
    }

    double x;
    double y;
    int t;
    double d = 20.6;


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

//Зеркальный массив
static SPtr<MbSolid> CreateMirror(SPtr<MbSolid> SolidForMirror, double degr, MbAxis3D axis) {

    SolidSPtr uSolid;
    SolidSPtr uSolidRes;
    MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames(0, 1, false);
    // Локальная СК, относительно плоскости XY которой будет выполняться симметрия
    MbPlacement3D plSym;
    plSym.Rotate(axis, degr * DEG_TO_RAD);

    ::SymmetrySolid(*SolidForMirror, cm_Copy, MbSymmetrySolidParams(plSym, operNames, ts_neutral), uSolidRes);
    BooleanResult(uSolidRes, cm_Copy, SolidForMirror, cm_Copy, MbBooleanOperationParams(bo_Union, true, operNames), uSolid);
    return uSolid;
}

//Создание перегородки для труб
SPtr<MbSolid> ParametricModelCreator::CreatePeregorodka(double p, double D_Kzh, double D_Kr) {

    double S1;
    double D3;
    double dop = 12 * 2;

    if ((500 <= D_Kr) && (D_Kr < 600)) {
        S1 = 10;
        D3 = 497 + dop;
    }
    else if ((600 <= D_Kr) && (D_Kr < 700)) {
        S1 = 10;
        D3 = 597 + dop;
    }
    else if ((700 <= D_Kr) && (D_Kr < 800)) {
        S1 = 12;
        D3 = 696 + dop;
    }
    else if ((800 <= D_Kr) && (D_Kr < 900)) {
        S1 = 12;
        D3 = 796 + dop;
    }
    else if ((900 <= D_Kr) && (D_Kr < 1000)) {
        S1 = 12;
        D3 = 895 + dop;
    }
    else  if ((1000 <= D_Kr) && (D_Kr < 1100)) {
        S1 = 12;
        D3 = 995 + dop;
    }
    else if ((1100 <= D_Kr) && (D_Kr < 1200)) {
        S1 = 12;
        D3 = 1095 + dop;
    }
    else if (1200 <= D_Kr) {
        S1 = 12;
        D3 = 1195 + dop;
    }

    //// Создание массива точек для контура.
    SArray<MbCartPoint> arrPnts1(4);
    arrPnts1.Add(MbCartPoint(0, 0));
    arrPnts1.Add(MbCartPoint(0, S1));
    arrPnts1.Add(MbCartPoint(D3 / 2, S1));
    arrPnts1.Add(MbCartPoint(D3 / 2, 0));

    MbPolyline* pPolyline1 = new MbPolyline(arrPnts1, true);
    MbContour* pContourPolyline1 = new MbContour(*pPolyline1, true);

    MbPlacement3D pl;
    MbSweptData sweptData1(pl, *pContourPolyline1);

    RevolutionValues revParms1(-180 * DEG_TO_RAD, 0, 0);

    MbSNameMaker operNames1(1, MbSNameMaker::i_SideNone, 0);
    PArray<MbSNameMaker> cNames1(0, 1, false);

    MbAxis3D axisY(pl.GetAxisY());

    MbSolid* pSolidOsnova1 = nullptr;
    ::RevolutionSolid(sweptData1, axisY, revParms1, operNames1, cNames1, pSolidOsnova1);

    SolidSPtr pSolidOsnovaRes1(pSolidOsnova1);


    SolidSPtr MassivOtv = CreateOtvVPeregorodka(pSolidOsnovaRes1, D_Kzh, p, D_Kr);
    SolidSPtr MassivOtv2 = CreateMirror(MassivOtv, 180, axisY);

    return MassivOtv2;
}


//Создание неподвижной решетки
SPtr<MbSolid> ParametricModelCreator::CreateOsnovaNePodResh(double p, double D_Kzh, double D_Kr) {

    double S;
    double D1;
    double D4;
    //добавить зависимость от давления

    if ((500 <= D_Kr) && (D_Kr < 600)) {
        S = 50;
        D1 = 563;
        D4 = 496;
    }
    else if ((600 <= D_Kr) && (D_Kr < 700)) {
        S = 65;
        D1 = 663;
        D4 = 596;
    }
    else if ((700 <= D_Kr) && (D_Kr < 800)) {
        S = 75;
        D1 = 773;
        D4 = 695;
    }
    else if ((800 <= D_Kr) && (D_Kr < 900)) {
        S = 85;
        D1 = 875;
        D4 = 795;
    }
    else if ((900 <= D_Kr) && (D_Kr < 1000)) {
        S = 65;
        D1 = 977;
        D4 = 895;
    }
    else if ((1000 <= D_Kr) && (D_Kr < 1100)) {
        S = 90;
        D1 = 1078;
        D4 = 995;
    }
    else if ((1100 <= D_Kr) && (D_Kr < 1200)) {
        S = 100;
        D1 = 1188;
        D4 = 1095;
        /*
        S = 105;
        D1 = 1188;
        D4 = 1095;*/
    }
    else if (1200 <= D_Kr) {
        S = 95;
        D1 = 1266;
        D4 = 1195;
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
    MbAxis3D axisY(pl1.GetAxisY());
    MbResultType res = ::RevolutionSolid(sweptData, axisY, revParms, operNames, cNames, pSolid);
    SolidSPtr pSolidOsnovaRes(pSolid);

    SolidSPtr MassivOtv = CreateOtvVPeregorodka(pSolidOsnovaRes, D_Kzh, p, D_Kr);
    SolidSPtr MassivOtv1 = CreateMirror(MassivOtv, 180, axisY);

    return MassivOtv1;
}

//Стойка
SPtr<MbSolid> ParametricModelCreator::CreateStoyka(double D_Kzh, double L_Base, double s)
{
    double b; //выдавливание нижнего прямоугольника 
    double h; //высота, на которую опускаем стойку 
    double Width;
    if (D_Kzh <= 800) {
        b = 18;
        h = 180;
        Width = L_Base * 0.02;
    }
    else if ((800 < D_Kzh) && (D_Kzh <= 1000)) {
        b = 22;
        h = 190;
        Width = L_Base * 0.02;
    }
    else if ((1000 < D_Kzh) && (D_Kzh <= 1200)) {
        b = 25;
        h = 200;
        Width = L_Base * 0.025;
    }
    else if ((1200 < D_Kzh) && (D_Kzh <= 1400)) {
        b = 28;
        h = 210;
        Width = L_Base * 0.025;
    }
    else if ((1400 < D_Kzh) && (D_Kzh <= 1600)) {
        b = 41;
        h = 220;
        Width = L_Base * 0.03;
    }
    else if ((1600 < D_Kzh) && (D_Kzh <= 1800)) {
        b = 53;
        h = 250;
        Width = L_Base * 0.03;
    }
    else if ((1800 < D_Kzh) && (D_Kzh <= 2000)) {
        b = 60;
        h = 270;
        Width = L_Base * 0.03;
    }
    else if (2000 < D_Kzh) {
        b = 65;
        h = 280;
        Width = L_Base * 0.03;
    }
    // построение прямоугольника-основы

    MbPlacement3D pl;
    MbPlane* pPlaneXZ = new MbPlane(MbCartPoint3D(0, 0, 0), MbCartPoint3D(1, 0, 0), MbCartPoint3D(0, 0, 1));

    double D_Kzh_V = D_Kzh + 2 * s;
    MbCartPoint p1P(-Width, D_Kzh_V / 2 - 50);
    MbCartPoint p2P(Width, D_Kzh_V / 2 - 50);
    MbCartPoint p3P(Width, -D_Kzh_V / 2 - 50);
    MbCartPoint p4P(-Width, -D_Kzh_V / 2 - 50);

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
    arrPnts21.Add(MbCartPoint3D(-300, D_Kzh_V / 2 + h, 0));
    arrPnts21.Add(MbCartPoint3D(300, D_Kzh_V / 2 + h, 0));
    arrPnts21.Add(MbCartPoint3D(0, D_Kzh_V / 2 + h + D_Kzh_V / 2, 0));
    ::ElementarySurface(arrPnts21[0], arrPnts21[1], arrPnts21[2], st_CylinderSurface, pCylSurf);

    // построение крестообразного эскиза

    pPlaneXZ->Move(MbVector3D(MbCartPoint3D(0, 0, 0), MbCartPoint3D(0, 18, 0)));

    MbCartPoint p1(-Width + b, D_Kzh_V / 2 - 85);
    MbCartPoint p2(Width - b, D_Kzh_V / 2 - 85);
    MbCartPoint p3(Width - b, D_Kzh_V / 2 - 105);
    MbCartPoint p4(b, D_Kzh_V / 2 - 105);
    MbCartPoint p5(b, b);
    MbCartPoint p6(Width - b, b);
    MbCartPoint p7(Width - b, -b);
    MbCartPoint p8(b, -b);
    MbCartPoint p9(b, -D_Kzh_V / 2 + 79);
    MbCartPoint p10(Width - b, -D_Kzh_V / 2 + 79);
    MbCartPoint p11(Width - b, -D_Kzh_V / 2 + 65);
    MbCartPoint p12(-Width + b, -D_Kzh_V / 2 + 65);
    MbCartPoint p13(-Width + b, -D_Kzh_V / 2 + 79);
    MbCartPoint p14(-b, -D_Kzh_V / 2 + 79);
    MbCartPoint p15(-b, -b);
    MbCartPoint p16(-Width + b, -b);
    MbCartPoint p17(-Width + b, b);
    MbCartPoint p18(-b, b);
    MbCartPoint p19(-b, D_Kzh_V / 2 - 105);
    MbCartPoint p20(-Width + b, D_Kzh_V / 2 - 105);


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

/*

static SPtr<MbSolid> CreateSechenie(SPtr<MbSolid> solid) {
    SpacePointsVector points;
    {
        points.push_back(MbCartPoint3D(0, 0, 0)); // p0
        points.push_back(MbCartPoint3D(0, 0, 60)); // pz
        points.push_back(MbCartPoint3D(60, 0, 0)); // px
    }

    c3d::SurfacesSPtrVector cutters;
    { // create 3 cutting planes
        MbSurface* cutter = nullptr;
        points[0].SetZero(); // p0
        points[1].Init(1, 0, 0); // px
        points[2].Init(0, 1, 0); // py
        ::ElementarySurface(points[0], points[1], points[2], st_Plane, cutter);
        if (cutter != nullptr)
            cutters.push_back(SPtr<MbSurface>(cutter));
    }

    std::vector< SPtr<MbSolid> > parts;
    parts.push_back(solid);
    solid = nullptr;

    // copy mode: full
    MbeCopyMode copyMode = cm_Copy; // don't use other copy modes if you are going to move parts

    const bool doClosed = true;
    const MbMergingFlags mergeFlags(true, true);

    size_t i, cuttersCnt = cutters.size();
    for (i = 0; i < cuttersCnt; ++i) { // cut by planes
        size_t partsCnt = parts.size();
        for (size_t j = 0; j < partsCnt; ++j) { // process parts
            {
                SolidsSPtrVector newParts;
                newParts.reserve(2);

                MbSNameMaker snMaker;

                MbShellCuttingParams cuttingParams(*cutters[i], false, mergeFlags, doClosed, snMaker);
                MbResultType res = ::SolidCutting(parts[j], copyMode, cuttingParams, newParts);

                if (res == rt_Success) {
                    size_t addCnt = newParts.size();
                    for (size_t k = 0; k < addCnt; ++k) {
                        if (newParts[k] != nullptr) {
                            parts.push_back(SolidSPtr(newParts[k]));
                            // below (optional): you can delete solid history if it's not necessary or some memory's required
                            parts.back()->DeleteCreators();
                        }
                    }
                }
                parts[j] = nullptr;
            }
        }
        // delete processed parts
        for (size_t k = partsCnt; k--; )
            parts.erase(parts.begin() + k);
    }

    // add resulting parts to the model
    size_t partsCnt = parts.size();
    bool showRightNow = false;
    //TestVariables::viewManager->AddObject(TestVariables::SOLID_Style, parts[0]);

    return parts[0];
}
*/


//Крышки
 //Основное тело
static SPtr<MbSolid> CreateBaseCover(double Dy, double p) {

    double b;
    double d2;
    double D2;
    double D;
    double d;
    double D1;
    double h;
    double h1;

    if (p == 1.6) {
        switch ((int)Dy) {
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
            h = 4;
            h1 = 4;
            break;
        }
    }
     else if (p == 2.5) {
     switch ((int)Dy) {
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

    const double DEG_TO_RAD = M_PI / 180.0;
    MbPlacement3D pl;

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



SPtr<MbSolid> ParametricModelCreator::CreateUnionCover(double Dy, double p, int id, double D_Kzh, double DKr) {

    double b;
    double d;
    double D1;
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

    //первый снизу
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

    //третий сверху
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


    // if (p == 1.6) {
    if (p == 1.6) {
        switch ((int)Dy) {
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
        switch ((int)Dy) {
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


    if ((id == 6) || (id == 7)) {
        num = 8;
        d = 23;
    }
    if ((id == 5)) {
        num = 12;
        d = 23;
    }
    // }

    MbAxis3D axisX(MbVector3D(1, 0, 0));

    SolidSPtr KrBase = CreateBaseCover(Dy, p);
    SolidSPtr KrWithOtv = CreateMassivOtv(KrBase, b, d / 2, D1, 0., num, axisX, 0, 0);
    SolidSPtr KrRes = CreateCylForCover(KrWithOtv, b, d);
    return  KrRes;
}